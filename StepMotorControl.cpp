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

	input = B0;
}

void StepMotorControl::setInput(byte input)
{
	input = input;
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

void StepMotorControl::setMotorHeadPins(byte notEnabledPin, byte stepPin, byte directionPin)
{
	this->notEnabledPin = notEnabledPin;
	pinMode(notEnabledPin, OUTPUT);

	this->stepPin = stepPin;
	pinMode(stepPin, OUTPUT);

	this->directionPin = directionPin;
	pinMode(directionPin, OUTPUT);
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
	//if (digitalRead(btnLeftPin) == HIGH
	//	&& digitalRead(btnRightPin) == LOW
	//	&& digitalRead(lightBarrierLeftPin) == LOW) // LEFT 100xx
	if (interpretInput(Kleenean::True, Kleenean::False, Kleenean::False, Kleenean::Maybe, Kleenean::Maybe))
	{
		lastState = currentState;
		currentState = StepMotorStates::LEFT;
	}
	//else if (digitalRead(btnLeftPin) == LOW
	//	&& digitalRead(btnRightPin) == HIGH
	//	&& digitalRead(lightBarrierRightPin) == LOW) // RIGHT 01x0x
	if (interpretInput(Kleenean::False, Kleenean::True, Kleenean::Maybe, Kleenean::False, Kleenean::Maybe))
	{
		lastState = currentState;
		currentState = StepMotorStates::RIGHT;
	}
	//else if (digitalRead(btnLeftPin) == LOW
	//	&& digitalRead(btnRightPin) == LOW
	//	&& serialAvailable) // AUTOMATIC 00xx1 
	if (interpretInput(Kleenean::False, Kleenean::False, Kleenean::Maybe, Kleenean::Maybe, Kleenean::True))
	{
		lastState = currentState;
		currentState = StepMotorStates::AUTOMATIC;
	}
	else
	{
		// stay in IDLE
		lastState = currentState;
		currentState = StepMotorStates::IDLE;
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

	//if (digitalRead(btnLeftPin) == LOW ||
	//	digitalRead(lightBarrierLeftPin) == HIGH) // 0xxxx || xx1xx
	if (interpretInput(Kleenean::False, Kleenean::Maybe, Kleenean::Maybe, Kleenean::Maybe, Kleenean::Maybe) 
		| interpretInput(Kleenean::Maybe, Kleenean::Maybe, Kleenean::True, Kleenean::Maybe, Kleenean::Maybe))
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
	//if (digitalRead(btnRightPin) == LOW ||
	//	digitalRead(lightBarrierRightPin) == HIGH) // x0xxx || xxx1x
	if (interpretInput(Kleenean::Maybe, Kleenean::False, Kleenean::Maybe, Kleenean::Maybe, Kleenean::Maybe)
		| interpretInput(Kleenean::Maybe, Kleenean::Maybe, Kleenean::Maybe, Kleenean::True, Kleenean::Maybe))
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

bool StepMotorControl::interpretInput(Kleenean btnLeft, Kleenean btnRight, Kleenean lbLeft, Kleenean lbRight, Kleenean automatic)
{
	bool result = true;
	if (btnLeft != Kleenean::Maybe) 
	{
		result &= (btnLeft == (input & 1 << 7));
	}

	if (btnRight != Kleenean::Maybe)
	{
		result &= (btnLeft == (input & 1 << 6));
	}

	if (lbLeft != Kleenean::Maybe)
	{
		result &= (btnLeft == (input & 1 << 5));
	}

	if (lbRight != Kleenean::Maybe)
	{
		result &= (btnLeft == (input & 1 << 4));
	}

	if (automatic != Kleenean::Maybe)
	{
		//result |= (btnLeft == (input & 1 << 7));
	}

	return result;
}


