// UNO R4 WiFi: serves a web page with W A S D controls and passes each
// command to the RedBot over the serial wires.
//
//   Browser --GET /cmd?c=w--> UNO R4 --Serial1: 'w'--> RedBot (motors)
//   Browser <--sensor text--- UNO R4 <--Serial1: line-- RedBot (IR + encoders)
//
// The page repeats the current command every 200 ms ('x' = stop), so the
// RedBot keeps moving only while a key is held.
//
// Wiring (both boards are 5 V):
//   UNO pin 1 (TX) -> RedBot A1 (RX)
//   UNO pin 0 (RX) <- RedBot A0 (TX)
//   UNO GND        -- RedBot GND
//
// To use: join Wi-Fi "RedBot" (password "redbot123"), open http://192.168.4.1

#include <Arduino.h>
#include <WiFiS3.h>
#include "page.h"

const char WIFI_NAME[] = "RedBot";
const char WIFI_PASS[] = "redbot123";  // at least 8 characters

WiFiServer server(80);
String sensorLine = "waiting for RedBot...";  // last full line from the RedBot
String incoming = "";                         // line being received right now

void setup()
{
	Serial.begin(115200);  // USB, for debug messages
	Serial1.begin(9600);   // pins 0/1, to the RedBot

	// Create our own Wi-Fi network and start the web server.
	WiFi.config(IPAddress(192, 168, 4, 1));
	if (WiFi.beginAP(WIFI_NAME, WIFI_PASS) != WL_AP_LISTENING) {
		Serial.println("Creating the Wi-Fi network failed");
		while (true) {}
	}
	delay(10000);  // give the Wi-Fi module time to start (as in the Arduino example)
	server.begin();

	Serial.print("Join Wi-Fi '");
	Serial.print(WIFI_NAME);
	Serial.print("' and open http://");
	Serial.println(WiFi.localIP());
}

// Collect characters from the RedBot; each '\n' completes a sensor line.
void readFromRedBot()
{
	while (Serial1.available() > 0) {
		char c = Serial1.read();
		if (c == '\n') {
			sensorLine = incoming;
			incoming = "";
		} else if (c != '\r') {
			incoming += c;
		}
	}
}

void sendResponse(WiFiClient& client, const char* contentType, const String& body)
{
	client.println("HTTP/1.1 200 OK");
	client.print("Content-Type: ");
	client.println(contentType);
	client.println("Connection: close");
	client.println();
	client.print(body);
}

// Answer one browser request: either a command or the page itself.
void handleBrowser()
{
	WiFiClient client = server.available();
	if (!client) return;

	String request = client.readStringUntil('\n');  // e.g. "GET /cmd?c=w HTTP/1.1"
	while (client.available() > 0) client.read();   // ignore the rest of the request

	if (request.startsWith("GET /cmd?c=")) {
		char cmd = request.charAt(11);  // the letter right after "c="
		Serial1.write(cmd);             // pass it on to the RedBot
		sendResponse(client, "text/plain", sensorLine);
	} else {
		sendResponse(client, "text/html", PAGE);
	}
	client.stop();
}

void loop()
{
	readFromRedBot();
	handleBrowser();
}
