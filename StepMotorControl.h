// StepMotorControl.h

#ifndef _STEPMOTORCONTROL_h
#define _STEPMOTORCONTROL_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif



class StepMotorControl
{
	enum StepMotorStates
	{
		IDLE,
		LEFT,
		RIGHT,
		AUTOMATIC
	};

private:
	byte notEnabledPin;
	byte stepPin;
	byte directionPin;

	bool serialAvailable;

	byte input;

	unsigned int defaultInterval;
	unsigned int interval;
	unsigned long lastExecutionTime;

	StepMotorStates currentState;
	StepMotorStates lastState;

	void step();

	void idle_update();
	void left_update();
	void right_update();
	void automatic_update();

public:
	void setInput(byte input);
	void init(unsigned int defaultInterval);
	void update();

	void setMotorHeadPins(byte notEnabledPin, byte stepPin, byte directionPin);
};

#endif

