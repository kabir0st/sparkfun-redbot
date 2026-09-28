#include <Arduino.h>
#include <RedBot.h>
#include <types.hpp>

constexpr int STATUS_LED = 13;

RedBotMotors motors;
RedBotButton button;

State current_state = State::STOP;
State last_state = State::STOP;

constexpr Step test_plan[] = {
  {Action::FORWARD,    1000},
  {Action::STOP,        0},
  {Action::ROTATE,  90},
  {Action::STOP,        0},
  {Action::FORWARD,    500},
  {Action::STOP,        0},
  {Action::ROTATE,  90},
  {Action::STOP,        0},
};

void setup()
{
	Serial.begin(9600);
	pinMode(STATUS_LED, OUTPUT);
	Serial.println("RedBot Setup Complete.");
}

Robot bot;

int id = 0;

void loop()
{
	// start end setup
	if (button.read())
	{
		if (current_state == State::STOP)
		{
			current_state = State::START;
		}
		else
		{
			current_state = State::STOP;
		}
	}

	if (current_state == State::STOP) {
		motors.brake();
		return;
	};

	// main start loop
	if (current_state == State::START) {
		if (test_plan[id].action == Action::FORWARD) {
			bot.drive(test_plan[id].value);
			id = id + 1;
		} else if (test_plan[id].action == Action::STOP ) {
			bot.stop();
		} else if (test_plan[id].action == Action::ROTATE) {
			bot.rotate(test_plan[id].value);
		}
	}
}