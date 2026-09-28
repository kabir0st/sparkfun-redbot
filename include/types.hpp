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

class Robot {
	public:
		int x;
		int y;
        RedBotMotors motors;

        RedBotEncoder encoder = RedBotEncoder(A2, A3);

        // RedBotSensor left_sensor(A6);
        // RedBotSensor center_sensor(A5);
        // RedBotSensor right_sensor(A7);

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
        Serial.println("driving distance : ");
        Serial.println(dist_mm); 
    }

    void stop(int ms = 0) {
        motors.stop();
        delay(ms);
    }

    void rotate(int deg) {
        motors.brake();
        delay(100);
        Serial.println("Flow flow ? ");
        // pivot (+ve = counter clockwise)
        // motors.pivot(100);
        // long target_count = 155;
        // long right_count_diff = 0;
        // while (abs(right_count_diff) < target_count) {
        //     right_count = encoder.getTicks(RIGHT);
        // }
    }
};
