// 
// 
// 

#include "StepMotorControl.h"

void StepMotorControl::init(unsigned int defaultInterval)
{
	serialAvailable = false;

	this->defaultInterval = defaultInterval;
	interval = defaultInterval;

	lastExecutionTime = 0UL;
	currentState = StepMotorStates::IDLE; // TODO Überlegen wegen einem State CALIB

	input = B0;
}

void StepMotorControl::setInput(byte input)
{
	this->input = input;
}

void StepMotorControl::update() // TODO in Update-Methode die Eingabe übergeben
{
	if ((lastExecutionTime + interval) < millis())
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
	digitalWrite(stepPin, HIGH);
	digitalWrite(stepPin, LOW);
}

void StepMotorControl::idle_update()
{
	if (lastState != currentState)
	{
		digitalWrite(notEnabledPin, LOW);
	}

	// left, right, lbLeft, lbRight, Serial, reserved,  reserved,  reserved
	if (input == B10000000 || input == B10010000) // LEFT 10000 ||
	{
		lastState = currentState;
		currentState = StepMotorStates::LEFT;
	}
	else if (input == B01000000 || input == B01100000) // RIGHT 01x0x
	{
		lastState = currentState;
		currentState = StepMotorStates::RIGHT;
	}
	else if ((input & B00001000) == B00001000) // AUTOMATIC 00xx1 
											   // TODO StateMachine erweitern mit Automatic_Left und Automatic_Right. Automatic ist A_IDLE. 
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
		digitalWrite(directionPin, LOW);
	}

	step();

	if ((input & B10000000) != B10000000 || (input & B00100000) == B00100000)  // 0xxxx || xx1xx
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
		digitalWrite(directionPin, HIGH);
	}

	step();

	// left, right, lbLeft, lbRight, Serial
	if ((input & B01000000) != B01000000 || (input & B00010000) == B00010000) // x0xxx || xxx1x
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
		// TODO set interval
		// TODO Wo werden die Anzahl an Steps gespeichert, welche gefahren werden müssen
	}

	// do it!
	// automatic states?

	if (true) // !(00000)
	{
		// exit action
		interval = defaultInterval; // reset interval

		lastState = currentState;
		currentState = StepMotorStates::IDLE;
	}
}
