#include <Arduino.h>
#include <RedBot.h>

constexpr int STATUS_LED = 13;

RedBotMotors motors;

RedBotEncoder encoder = RedBotEncoder(A2, A3);

RedBotSensor left_sensor(A6);
RedBotSensor center_sensor(A5);
RedBotSensor right_sensor(A7);


enum class State  {
  STOP,
  START
};

State current_state = State::STOP;
State last_state = State::STOP;

class Sensors {
  public:
    // for ir sensors
    int ir_left;
    int ir_cent;
    int ir_right;
    // for motors
    int32_t tick_left;
    int32_t tick_right;
  
    void read_data(){
      ir_left = left_sensor.read();
      ir_cent = center_sensor.read();
      ir_right = right_sensor.read();

      tick_left = encoder.getTicks(LEFT);
      tick_right = encoder.getTicks(RIGHT);
      return;
    }
};


void setup()
{
  Serial.begin(9600);
  pinMode(STATUS_LED, OUTPUT);
  encoder.clearEnc(BOTH);
  Serial.println("RedBot Sensor Test");
}

void loop() {


  if (current_state = State::STOP)

  Serial.print("Sensor Readings: ");
  Serial.print("L=");
  Serial.print(left_value);
  Serial.print("  C=");
  Serial.print(center_value);
  Serial.print("  R=");
  Serial.println(right_value);


  Serial.print("Encoder: ");
  Serial.print("L=");
  Serial.print(left_ticks);
  Serial.print("  R=");
  Serial.println(right_ticks);
  delay(200);
}