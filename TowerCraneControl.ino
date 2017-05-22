#include "StepMotorControl.h"

StepMotorControl stepMotorControl;

void setup()
{
	//stepMotorControl.setBtnPins()
	stepMotorControl.init(100);
}

void loop()
{
	// read serial
	stepMotorControl.update();
}
