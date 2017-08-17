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
		AUTOMATIC_IDLE,
		AUTOMATIC_LEFT,
		AUTOMATIC_RIGHT
	};

private:
	byte motorId;

	byte notEnabledPin;
	byte stepPin;
	byte directionPin;

	unsigned int defaultInterval;
	unsigned int interval;
	unsigned long lastExecutionTime;

	StepMotorStates currentState;
	StepMotorStates lastState;

	int currentSteps;

	void step();

	// Vom State abhängige Methoden
	void idle_update(byte input);
	void left_update(byte input);
	void right_update(byte input);
	void automatic_idle_update(byte input);
	void automatic_left_update(byte input);
	void automatic_right_update(byte input);

public:
	void init(byte identifier, unsigned int defaultInterval);
	void update(byte input);

	void setMotorHeadPins(byte notEnabledPin, byte stepPin, byte directionPin);
};

#endif

