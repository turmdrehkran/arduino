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

void LedBlinkTask::stop()
{
	digitalWrite(ledPin, false);
	Task::stop();
}

void LedBlinkTask::update()
{
	if (isExecutionTime()) {
		digitalWrite(ledPin, toggle);
		toggle = !toggle;
		onAfterExecution();
	}
}
