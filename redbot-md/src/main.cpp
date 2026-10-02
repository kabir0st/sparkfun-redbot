// RedBot: takes one-letter commands from the UNO R4, drives the motors, and
// answers each command with one line of sensor readings.
//
//   UNO R4 ---- 'w' 'a' 's' 'd' or 'x' (stop) ----> RedBot   (about every 200 ms)
//   UNO R4 <--- "IR L=.. C=.. R=.. | Enc L=.. R=.." -- RedBot   (reply to each command)
//
// The UNO is wired to A0/A1 (RedBot's software serial port):
//   RedBot A0 (TX) -> UNO pin 0 (RX)
//   RedBot A1 (RX) <- UNO pin 1 (TX)

#include <Arduino.h>
#include <RedBot.h>

RedBotSoftwareSerial uno;       // always uses A0 = TX, A1 = RX
RedBotMotors motors;
RedBotEncoder encoder(A2, A3);  // left, right
RedBotSensor leftIR(A6);
RedBotSensor centerIR(A5);
RedBotSensor rightIR(A7);

const int DRIVE_SPEED = 150;  // 0..255
const int TURN_SPEED = 100;
const unsigned long TIMEOUT_MS = 500;  // stop if no command arrives for this long

unsigned long lastCommandTime = 0;

void sendSensors()
{
	uno.print("IR L=");
	uno.print(leftIR.read());
	uno.print(" C=");
	uno.print(centerIR.read());
	uno.print(" R=");
	uno.print(rightIR.read());
	uno.print(" | Enc L=");
	uno.print(encoder.getTicks(LEFT));
	uno.print(" R=");
	uno.println(encoder.getTicks(RIGHT));
}

void setup()
{
	uno.begin(9600);
}

void loop()
{
	// 1. If the UNO sent a command, act on it and reply with the sensors.
	if (uno.available() > 0) {
		char cmd = uno.read();

		if (cmd == 'w')      motors.drive(DRIVE_SPEED);   // forward
		else if (cmd == 's') motors.drive(-DRIVE_SPEED);  // backward
		else if (cmd == 'a') motors.pivot(-TURN_SPEED);   // turn left
		else if (cmd == 'd') motors.pivot(TURN_SPEED);    // turn right
		else if (cmd == 'x') motors.brake();              // stop
		else return;                                      // garbled byte: ignore it

		lastCommandTime = millis();
		sendSensors();
	}

	// 2. Safety: if commands stop coming (page closed, Wi-Fi dropped), stop.
	if (millis() - lastCommandTime > TIMEOUT_MS) {
		motors.brake();
	}
}
