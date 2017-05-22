// 
// 
// 

#include "StepMotorControl.h"

void StepMotorControl::init(unsigned int defaultInterval)
{
	serialAvailable = false;

	this->defaultInterval = defaultInterval;
	interval = defaultInterval;

	lastExecutionTime = 0L;
	currentState = StepMotorStates::IDLE; // TODO Überlegen wegen einem State CALIB
}

void StepMotorControl::update()
{
	if ((millis() - lastExecutionTime) > interval)
	{
		lastExecutionTime = millis();

		switch (currentState)
		{
		case StepMotorStates::IDLE:
			idle_update();
			break;
		case StepMotorStates::LEFT:
			left_update();
			break;
		case StepMotorStates::RIGHT:
			right_update();
			break;
		case StepMotorStates::AUTOMATIC:
			automatic_update();
			break;
		default:
			break;
		}
	}
}

void StepMotorControl::step()
{
}

void StepMotorControl::idle_update()
{
	if (lastState != currentState)
	{
		digitalWrite(notEnabledPin, HIGH);
	}
	
	// left, right, lbLeft, lbRight, Serial
	if (digitalRead(btnLeftPin) == HIGH
		&& digitalRead(btnRightPin) == LOW
		&& digitalRead(lightBarrierLeftPin) == LOW) // LEFT 100xx
	{
		lastState = currentState;
		currentState = StepMotorStates::LEFT;
	}
	else if (digitalRead(btnLeftPin) == LOW
		&& digitalRead(btnRightPin) == HIGH
		&& digitalRead(lightBarrierRightPin) == LOW) // RIGHT 01x0x
	{
		lastState = currentState;
		currentState = StepMotorStates::RIGHT;
	}
	else if (digitalRead(btnLeftPin) == LOW
		&& digitalRead(btnRightPin) == LOW
		&& serialAvailable) // AUTOMATIC 00xx1 
	{
		lastState = currentState;
		currentState = StepMotorStates::AUTOMATIC;
	}
	else
	{
		// stay in IDLE
	}

	if (currentState != lastState)
	{
		digitalWrite(notEnabledPin, LOW);
	}
}

void StepMotorControl::left_update()
{
	if (lastState != currentState)
	{
		digitalWrite(directionPin, HIGH);
	}

	step();

	if (digitalRead(btnLeftPin) == LOW ||
		digitalRead(lightBarrierLeftPin) == HIGH) // 0xxxx || xx1xx
	{
		// exit action

		lastState = currentState;
		currentState = StepMotorStates::IDLE;
	}
}

void StepMotorControl::right_update()
{
	if (lastState != currentState)
	{
		digitalWrite(directionPin, LOW);
	}

	step();

	// left, right, lbLeft, lbRight, Serial
	if (digitalRead(btnRightPin) == LOW ||
		digitalRead(lightBarrierRightPin) == HIGH) // x0xxx || xxx1x
	{
		// exit action

		lastState = currentState;
		currentState = StepMotorStates::IDLE;
	}
}

void StepMotorControl::automatic_update()
{
	if (lastState != currentState)
	{
		// entry Action
		// set interval
	}

	// do it!
	// automatic states?

	if (true) // !(00000)
	{
		// exit action
		interval = defaultInterval;

		lastState = currentState;
		currentState = StepMotorStates::IDLE;
	}
}

