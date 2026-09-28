#pragma once
#include <RedBot.h>
#include <Arduino.h>

enum class State  {
	STOP,
	START
};

enum class Action {
	FORWARD,
	ROTATE,
	STOP
};

struct  Step
{
	Action action;
	int value;
};
class StateManager {
	public:
		static constexpr int8_t kHistory = 2;

		void update(State s) noexcept {
			history_[next_] = s;
			next_ = (next_ + 1) % kHistory;
		}

		State current() const noexcept {
			return history_[(next_ + kHistory - 1) % kHistory];
		}

		State previous() const noexcept {
			return history_[next_];
		}

	private:
		State history_[kHistory] = { State::STOP, State::STOP };
    	int8_t next_ = 0;
};

inline float ticksToMm(long ticks)
{
    constexpr float WHEEL_DIAMETER_MM = 65.0;
    constexpr float TICKS_PER_REV = 192.0;
    return (ticks * PI * WHEEL_DIAMETER_MM) / TICKS_PER_REV;
}

inline const char* stateToString(State state)
{
    switch (state) {
        case State::STOP:
            return "STOP";

        case State::START:
            return "START";

        default:
            return "UNKNOWN";
    }
}

class LogManager {
	public:
		LogManager(StateManager& state)
			: state_(state) {}
		
		void refresh() {
			Serial.print("\r");
			Serial.print("Current: ");
			Serial.print(stateToString(state_.current()));
			Serial.print(" | Last: ");
			Serial.print(stateToString(state_.previous()));
			Serial.print(" | ");
			Serial.print(current_info_);
			Serial.print("        ");
		}
		void inform(String info) {
			current_info_ = info;
			refresh();
		}
	private:
		StateManager &state_;
		String current_info_;
};


class Robot {
	public:
		int x;
		int y;
        RedBotMotors motors;

        RedBotEncoder encoder;

        RedBotSensor left_sensor;
        RedBotSensor center_sensor;
        RedBotSensor right_sensor;
        LogManager &logger;

        Robot(LogManager& logger)
            : encoder(A2, A3),
                left_sensor(A6),
                center_sensor(A5),
                right_sensor(A7),
                logger(logger)
                {}


        void turn_90_deg()
        {
            motors.brake();
            delay(100);
            // 2. Start Pivot (Positive = Clockwise)
            int power = 100;
            motors.pivot(power);
            long targetCount = 115;
            long rightCount = 0;
            while (abs(rightCount) < targetCount)
            {
                rightCount = encoder.getTicks(RIGHT);
                delay(10);
            }
            motors.brake();
            delay(250);
        }

    void drive(int dist_mm) {
        logger.inform("Driving forward");
        encoder.clearEnc(BOTH);
        motors.drive(100);
        float forward = 0;
        while (forward < dist_mm) {
            delay(10);
            float left = ticksToMm(encoder.getTicks(LEFT));
            float right = ticksToMm(encoder.getTicks(RIGHT));
            forward = (left + right) / 2;
        }
        motors.brake();
    }

    void stop(int ms = 0) {
        motors.stop();
        delay(ms);
    }
    void rotate(int deg)
    {
        logger.inform("Rotating");
        motors.brake();
        delay(100);
        constexpr float TICKS_PER_90_DEG = 115.0;
        long target_ticks =
            (long)(abs(deg) * TICKS_PER_90_DEG / 90.0);
        // Remember encoder position when rotation starts
        long start_ticks = encoder.getTicks(RIGHT);
        // +deg = clockwise
        // -deg = counter-clockwise
        int power = (deg > 0) ? 100 : -100;

        motors.pivot(power);
        while (true)
        {
            long current_ticks = encoder.getTicks(RIGHT);
            long ticks_moved = abs(current_ticks - start_ticks);
            if (ticks_moved >= target_ticks)
                break;
            delay(5);
        }
        motors.brake();
        delay(100);
    }
};
