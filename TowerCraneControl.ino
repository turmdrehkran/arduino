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
	winch.init(2);
	winch.setMotorHeadPins(5, 4, 3);
	inputWinch.init();
	byte btpins[] = { 28, 29 };
	inputWinch.initButtons(btpins, 2);
	byte lspins[] = { 38 };
	inputWinch.initLightBarriers(lspins, 1);

	trolley.init(3);
	trolley.setMotorHeadPins(16, 15, 14);
	inputTrolley.init();
	byte btpins_trolley[] = { 30, 31 };
	inputTrolley.initButtons(btpins_trolley, 2);
	byte lspins_trolley[] = { 41, 39 };
	inputTrolley.initLightBarriers(lspins_trolley, 2);

	rotator.init(10);
	rotator.setMotorHeadPins(46, 45, 44);
	inputRotator.init();
	byte btpins_rotator[] = { 32, 33 };
	inputRotator.initButtons(btpins_rotator, 2);
	byte lspins_rotator[] = { 40 };
	inputRotator.initLightBarriers(lspins_rotator, 1);

	//Serial.begin(9600);
}

void loop()
{
	updateWinch();
	updateTrolley();
	updateRotator();
}

void updateWinch() 
{
	inputWinch.update();
	winch.setInput(inputWinch.getInput());
	winch.update();
}

void updateTrolley() 
{
	inputTrolley.update();
	trolley.setInput(inputTrolley.getInput());
	trolley.update();
}

void updateRotator()
{
	// TODO Lichtschranke soll nur bei einer Kalibrierung auf diesen Motor angewendet werden, ansonsten LB-Bit auf Null
	inputRotator.update();
	byte input = inputRotator.getInput();
	input &= ~(1 << 5);
	rotator.setInput(input);
	rotator.update();
}