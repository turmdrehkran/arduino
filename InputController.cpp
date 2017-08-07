// 
// 
// 

#include "InputController.h"

void InputController::init()
{
	debounceTime = 10;
	lasttime = 0UL;
}

void InputController::initButtons(byte buttonPins[MAX_NUMBER_OF_BUTTONS], byte buttonPinsSize)
{
	this->buttonPinsSize = buttonPinsSize;
	for (byte i = 0; i < buttonPinsSize; i++)
	{
		this->buttonPins[i] = buttonPins[i];
		pinMode(this->buttonPins[i], INPUT_PULLUP);
	}
}

void InputController::initLightBarriers(byte pins[MAX_NUMBER_OF_LIGHTBARRIERS], byte lightBarrierPinsSize)
{
	this->lightBarrierPinsSize = lightBarrierPinsSize;
	for (byte i = 0; i < lightBarrierPinsSize; i++)
	{
		this->lightBarrierPins[i] = pins[i];
		pinMode(this->lightBarrierPins[i], INPUT);
	}
}

void InputController::update()
{
	if ((lasttime + debounceTime) >= millis())
	{
		return;
	}

	lasttime = millis();

	currentInputState = B0;

	update_Buttons();
	update_LightBarriers(); 
	//update_SerialInterface();
}

byte InputController::getInput()
{
	return currentInputState;
}

void InputController::update_Buttons()
{
	for (byte i = 0; i < buttonPinsSize; i++)
	{
		byte currentState = digitalRead(buttonPins[i]);
		currentInputState |= (currentState << (8 - i - 1));
	}
}

void InputController::update_LightBarriers()
{
	for (byte i = 0; i < lightBarrierPinsSize; i++)
	{
		byte currentState = digitalRead(lightBarrierPins[i]);
		currentInputState |= (currentState << (6 - i - 1));
	}
}

void InputController::update_SerialInterface()
{
	/*bool hasData = CommandTransceiver.isAvailable();*/
	byte currentState = LOW;	// TODO hier commandtransceiver fragen, ob eine serielle Schnittstelle aufgebaut wurde,
								// wenn ja, ändern alle Motoren ihren State nach AUTOMATIC_IDLE
	currentInputState |= (currentState << (4 - 1));
}

