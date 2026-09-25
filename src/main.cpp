#include <Arduino.h>
#include <RedBot.h>

constexpr int STATUS_LED = 13;

RedBotMotors motors;

RedBotEncoder encoder = RedBotEncoder(A2, A3);

RedBotSensor left_sensor(A6);
RedBotSensor center_sensor(A5);
RedBotSensor right_sensor(A7);

void setup() {
  Serial.begin(9600);

  pinMode(STATUS_LED, OUTPUT);

  encoder.clearEnc(BOTH);

  Serial.println("RedBot Sensor Test");
}

void loop() {
  int left_value = left_sensor.read();
  int center_value = center_sensor.read();
  int right_value = right_sensor.read();

  Serial.print("Sensor Readings: ");
  Serial.print("L=");
  Serial.print(left_value);
  Serial.print("  C=");
  Serial.print(center_value);
  Serial.print("  R=");
  Serial.println(right_value);

  int left_ticks = encoder.getTicks(LEFT);
  int right_ticks = encoder.getTicks(RIGHT);

  Serial.print("Encoder: ");
  Serial.print("L=");
  Serial.print(left_ticks);
  Serial.print("  R=");
  Serial.println(right_ticks);
  delay(200);
}