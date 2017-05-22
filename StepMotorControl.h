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

	byte btnLeftPin;
	byte btnRightPin;

	byte lightBarrierLeftPin;
	byte lightBarrierRightPin;

	bool serialAvailable;

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
	void init(unsigned int defaultInterval);
	void update();

	void setMotorHeadPins(byte notEnabledPin, byte stepPin, byte directionPin)
	{
		this->notEnabledPin = notEnabledPin;
		pinMode(notEnabledPin, OUTPUT);

		this->stepPin = stepPin;
		pinMode(stepPin, OUTPUT);

		this->directionPin = directionPin;
		pinMode(directionPin, OUTPUT);
	}

	void setBtnPins(byte btnLeftPin, byte btnRightPin)
	{
		this->btnLeftPin = btnLeftPin;
		pinMode(btnLeftPin, INPUT);

		this->btnRightPin = btnRightPin;
		pinMode(btnRightPin, INPUT);
	}

	void setLightBarrierPins(byte lightBarrierLeftPin, byte lightBarrierRightPin)
	{
		this->lightBarrierLeftPin = lightBarrierLeftPin;
		pinMode(lightBarrierLeftPin, INPUT);

		this->lightBarrierRightPin = lightBarrierRightPin;
		pinMode(lightBarrierRightPin, INPUT);
	}
};

#endif

