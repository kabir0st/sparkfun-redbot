#include <Arduino.h>
#include <RedBot.h>
#include <types.hpp>

constexpr int STATUS_LED = 13;

RedBotButton button;

StateManager state;
LogManager logger(state);
Robot bot(logger);

constexpr Step test_plan[] = {
  {Action::FORWARD,    1000},
  {Action::STOP,        100},
  {Action::ROTATE,  90},
  {Action::STOP,        100},
  {Action::FORWARD,    500},
  {Action::STOP,        100},
  {Action::ROTATE,  90},
  {Action::STOP,        100},
};
constexpr int plan_len = sizeof(test_plan) / sizeof(test_plan[0]);

void setup()
{
	Serial.begin(9600);
	pinMode(STATUS_LED, OUTPUT);
	Serial.println("RedBot Setup Complete.");
}

int id = 0;
bool last_read = 0;
int pressed = 0;

void loop()
{
    bool current_read = button.read();
	if (last_read != current_read && current_read) {
		pressed = pressed + 1;
		if (state.current() == State::STOP) {
			state.update(State::START);
			logger.inform("starting bot");
		} else {
			state.update(State::STOP);
			logger.inform("stopping bot");
		}
	}

	if (state.current() == State::STOP && state.previous() == State::START) {
		bot.motors.brake();
		state.update(State::STOP);
		logger.inform("Stopping everything");
	}
	else if (state.current() == State::START)
	{
		const Step& step = test_plan[id];
		if (step.action == Action::FORWARD) {
			bot.drive(step.value);
		} else if (step.action == Action::STOP) {
			bot.stop(step.value);
		} else if (step.action == Action::ROTATE) {
			bot.rotate(step.value);
		}
		id = id + 1;
		if (id >= plan_len) {
			id = 0;
			state.update(State::STOP);
			logger.inform("Plan complete");
		}
	}
	else
	{
		logger.inform("No Command.");
	}
	delay(80);
	last_read = current_read;
}