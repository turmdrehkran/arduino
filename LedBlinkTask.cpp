// 
// 
// 

#include "LedBlinkTask.h"

LedBlinkTask::LedBlinkTask()
{
}

void LedBlinkTask::init(uint16_t delay, uint16_t pin)
{
	Task::init(delay, pin);
	ledPin = pin;
	pinMode(ledPin, OUTPUT);
}

void LedBlinkTask::start()
{
	toggle = true;
	Task::start();
}

void LedBlinkTask::start(uint16_t numberOfPerformings)
{
	toggle = true;
	Task::start(2 * numberOfPerformings);
}

bool LedBlinkTask::stop()
{
	digitalWrite(ledPin, false);
	return Task::stop();
}

bool LedBlinkTask::update()
{
	if (isExecutionTime()) {
		digitalWrite(ledPin, toggle);
		toggle = !toggle;
		return onAfterExecution();
	}
	return false;
}
