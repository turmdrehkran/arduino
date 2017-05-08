// Task.h

#ifndef _TASK_h
#define _TASK_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

class Task
{
protected:
	uint16_t delayTime;
	unsigned long lastExecutionTime;
	bool isRunning;
	bool canExecuted;

	bool isExecutionTime();

	// Returned true by last execution
	bool onAfterExecution();

	uint16_t numberOfPerformings;
	const uint16_t maxNumberOfPerformings = 65000;

public:
	Task();

	virtual void init(uint16_t delay, uint16_t pin);
	virtual void start();
	virtual void start(uint16_t numberOfPerformings);
	virtual bool stop();

	// Returned true by last execution
	virtual bool update() = 0;

	virtual void lock();
	virtual void unlock();
};

#endif

