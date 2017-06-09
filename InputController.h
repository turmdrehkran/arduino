// ButtonController.h

#ifndef _INPUTCONTROLLER_h
#define _INPUTCONTROLLER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#define MAX_NUMBER_OF_BUTTONS 2
#define MAX_NUMBER_OF_LIGHTBARRIERS 2

class InputController
{
private:
	byte buttonPins[MAX_NUMBER_OF_BUTTONS];
	byte buttonPinsSize;

	byte lightBarrierPins[MAX_NUMBER_OF_LIGHTBARRIERS];
	byte lightBarrierPinsSize;

	unsigned long lasttime;
	byte debounceTime;

	byte currentInputState;

	void update_Buttons();
	void update_LightBarriers();
	void update_SerialInterface();



public:
	void init();
	void initButtons(byte buttons[MAX_NUMBER_OF_BUTTONS], byte buttonPinsSize);
	void initLightBarriers(byte lightBarriers[MAX_NUMBER_OF_LIGHTBARRIERS], byte lightBarrerPinsSize);

	void update();

	byte getInput();
};

#endif

