// LedBlinkTask.h

#ifndef _LEDBLINKTASK_h
#define _LEDBLINKTASK_h

#include "Task.h"

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

class LedBlinkTask : public Task
{
protected:
	byte ledPin;
	bool toggle;

public:
	LedBlinkTask();

	virtual void init(uint16_t delay, uint16_t pin);
	virtual void start();
	virtual void start(uint16_t numberOfPerformings);
	virtual bool stop();
	virtual bool update();
};


#endif

