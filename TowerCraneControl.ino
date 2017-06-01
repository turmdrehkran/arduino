#include "InputController.h"
#include "StepMotorControl.h"

StepMotorControl stepMotorControl;
InputController input;

void setup()
{
	stepMotorControl.init(100);
	input.init();

	byte btpins[] = { 8, 9 };
	input.initButtons(btpins, 2);

	byte lspins[] = { 6, 7 };
	input.initLightBarriers(lspins, 2);

	Serial.begin(9600);
}

void loop()
{
	// read serial
	
	input.update();
	stepMotorControl.update();
}
