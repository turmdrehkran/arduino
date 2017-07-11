// 
// 
// 

#include "StepMotorControl.h"
#include "CommandTransceiver.h"

void StepMotorControl::init(byte identifier, unsigned int defaultInterval)
{
	this->identifier = identifier;
	this->defaultInterval = defaultInterval;
	interval = defaultInterval;

	lastExecutionTime = 0UL;
	currentSteps = 0;
	currentState = StepMotorStates::IDLE; // TODO Überlegen wegen einem State CALIB
}

void StepMotorControl::update(byte input) // TODO in Update-Methode die Eingabe übergeben
{
	if ((lastExecutionTime + interval) < millis())
	{
		lastExecutionTime = millis();

		switch (currentState)
		{
		case StepMotorStates::IDLE:
			idle_update(input);
			break;
		case StepMotorStates::LEFT:
			left_update(input);
			break;
		case StepMotorStates::RIGHT:
			right_update(input);
			break;
		case StepMotorStates::AUTOMATIC_IDLE:
			automatic_idle_update(input);
			break;
		case StepMotorStates::AUTOMATIC_LEFT:
			automatic_left_update(input);
			break;
		case StepMotorStates::AUTOMATIC_RIGHT:
			automatic_right_update(input);
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

	// TODO count steps: left -1 and right +1
}

void StepMotorControl::idle_update(byte input)
{
	if (lastState != currentState)
	{
		digitalWrite(notEnabledPin, LOW);
	}

	// left, right, lbLeft, lbRight, Serial, reserved,  reserved,  reserved
	if (input == B10000000 || input == B10010000) // LEFT 10000 || 10010
	{
		lastState = currentState;
		currentState = StepMotorStates::LEFT;
	}
	else if (input == B01000000 || input == B01100000) // RIGHT 01000 || 01100
	{
		lastState = currentState;
		currentState = StepMotorStates::RIGHT;
	}
	else if (CommandTransceiver.isAvailable()) // AUTOMATIC 00xx1
											   // TODO StateMachine erweitern mit Automatic_Left und Automatic_Right. Automatic ist A_IDLE. 
	{
		lastState = currentState;
		currentState = StepMotorStates::AUTOMATIC_IDLE;
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

void StepMotorControl::left_update(byte input)
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

void StepMotorControl::right_update(byte input)
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

void StepMotorControl::automatic_idle_update(byte input)
{
	if (lastState != currentState)
	{
		// entry Action
		// TODO set interval
		// TODO Wo werden die Anzahl an Steps gespeichert, welche gefahren werden müssen
	}

	// do it!
	Command* command = CommandTransceiver.getCommand(identifier);

	if (command != nullptr && !command->Delivered) 
	{
		command->Delivered = true;
		interval = command->Speed;
		currentSteps = command->NumberOfSteps;

		if (command->Direction == 'L') 
		{
			lastState = currentState;
			currentState = StepMotorStates::AUTOMATIC_LEFT;
		}
		else if (command->Direction == 'R') 
		{
			lastState = currentState;
			currentState = StepMotorStates::AUTOMATIC_RIGHT;
		}
	}

	if (!CommandTransceiver.isAvailable())
	{
		// exit action
		interval = defaultInterval; // reset interval

		lastState = currentState;
		currentState = StepMotorStates::IDLE;
	}
}

void StepMotorControl::automatic_left_update(byte input)
{
	if (lastState != currentState)
	{
		digitalWrite(directionPin, LOW);
	}

	step();
	currentSteps--;

	// TODO neuer Befehl unterbricht aktuellen
	if (currentSteps <= 0 || (input & B00100000) == B00100000)  // 0xxxx || xx1xx
	{
		// exit action
		lastState = currentState;
		currentState = StepMotorStates::AUTOMATIC_IDLE;
	}
}

void StepMotorControl::automatic_right_update(byte input)
{
	if (lastState != currentState)
	{
		digitalWrite(directionPin, HIGH);
	}

	step();
	currentSteps--;

	// left, right, lbLeft, lbRight, Serial
	if (currentSteps <= 0 || (input & B00010000) == B00010000) // x0xxx || xxx1x
	{
		// exit action
		lastState = currentState;
		currentState = StepMotorStates::AUTOMATIC_IDLE;
	}
}
