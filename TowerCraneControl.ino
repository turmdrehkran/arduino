#include "InputController.h"
#include "StepMotorControl.h"
#include "CommandTransceiver.h"

StepMotorControl winchMotor;
InputController winchInput;

StepMotorControl trolleyMotor;
InputController trolleyInput;

StepMotorControl rotatorMotor;
InputController rotatorInput;

void setup()
{
	CommandTransceiver.init();
	winchMotor.init(0, 2);
	winchMotor.setMotorHeadPins(5, 4, 3);
	winchInput.init();
	byte winchButtonPins[] = { 28, 29 };
	winchInput.initButtons(winchButtonPins, 2);
	byte winchLightBarrierPins[] = { 38 };
	winchInput.initLightBarriers(winchLightBarrierPins, 1);

	trolleyMotor.init(1, 3);
	trolleyMotor.setMotorHeadPins(16, 15, 14);
	trolleyInput.init();
	byte trolleyButtonPins[] = { 30, 31 };
	trolleyInput.initButtons(trolleyButtonPins, 2);
	byte trolleyLightBarrierPins[] = { 41, 39 };
	trolleyInput.initLightBarriers(trolleyLightBarrierPins, 2);

	rotatorMotor.init(2, 10);
	rotatorMotor.setMotorHeadPins(46, 45, 44);
	rotatorInput.init();
	byte rotatorButtonPins[] = { 32, 33 };
	rotatorInput.initButtons(rotatorButtonPins, 2);
	byte rotatorLightBarrierPins[] = { 40 };
	rotatorInput.initLightBarriers(rotatorLightBarrierPins, 1);
}

void loop()
{
	CommandTransceiver.update();
	updateWinch();
	updateTrolley();
	updateRotator();
}

void updateWinch() 
{
	winchInput.update();
	winchMotor.update(winchInput.getInput());
}

void updateTrolley() 
{
	trolleyInput.update();
	trolleyMotor.update(trolleyInput.getInput());
}

void updateRotator()
{
	// Lichtschranke soll nur bei einer Kalibrierung auf diesen Motor angewendet werden, ansonsten LB-Bit auf Null
	rotatorInput.update();
	byte input = rotatorInput.getInput();
	if (!CommandTransceiver.isCalibrating()) 
	{
		input &= ~(1 << 5);
	}
	rotatorMotor.update(input);
}