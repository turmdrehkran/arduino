#include "InputController.h"
#include "StepMotorControl.h"

StepMotorControl winch;
InputController inputWinch;

StepMotorControl trolley;
InputController inputTrolley;

StepMotorControl rotator;
InputController inputRotator;

void setup()
{
	winch.init(1);
	winch.setMotorHeadPins(5, 4, 3);
	inputWinch.init();

	byte btpins[] = { 28, 29 };
	inputWinch.initButtons(btpins, 2);

	byte lspins[] = { 8 };
	inputWinch.initLightBarriers(lspins, 1);

	trolley.init(2);
	trolley.setMotorHeadPins(16, 15, 14);
	inputTrolley.init();
	byte btpins_trolley[] = { 30, 31 };
	inputTrolley.initButtons(btpins_trolley, 2);
	byte lspins_trolley[] = { 8 };
	inputTrolley.initLightBarriers(lspins_trolley, 1);

	rotator.init(1);
	rotator.setMotorHeadPins(46, 45, 44);
	inputRotator.init();
	byte btpins_rotator[] = { 32, 33 };
	inputRotator.initButtons(btpins_rotator, 2);
	byte lspins_rotator[] = { 8 };
	inputRotator.initLightBarriers(lspins_rotator, 1);

	Serial.begin(9600);
}

void loop()
{
	inputWinch.update();
	winch.setInput(inputWinch.getInput());
	winch.update();

	inputTrolley.update();
	trolley.setInput(inputTrolley.getInput());
	trolley.update();

	inputRotator.update();
	rotator.setInput(inputRotator.getInput());
	rotator.update();
}
