#include "InputController.h"
#include "StepMotorControl.h"

StepMotorControl stepMotorControl;
InputController input;

void setup()
{
	stepMotorControl.init(1);
	stepMotorControl.setMotorHeadPins(5, 4, 3);
	input.init();

	byte btpins[] = { 28, 29 };
	input.initButtons(btpins, 2);

	byte lspins[] = { 8 };
	input.initLightBarriers(lspins, 1);

	Serial.begin(9600);
}

void loop()
{
	// read serial
	
	input.update();
	byte i = input.getInput();
	//Serial.println(i, BIN);

	stepMotorControl.setInput(i);
	stepMotorControl.update();
}
