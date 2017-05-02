// 
// 
// 

#include "Task.h"

Task::Task()
{
}

void Task::init(uint16_t delay, uint16_t pin)
{
	this->delayTime = delay;
	lastExecutionTime = 0L;
	isRunning = false;
}

void Task::start()
{
	isRunning = true;
}

void Task::stop()
{
	isRunning = false;
	lastExecutionTime = 0L;
}

void Task::lock()
{
	if (!isRunning)
		canExecuted = false;
}

void Task::unlock()
{
	if (!isRunning)
		canExecuted = true;
}

bool Task::isExecutionTime()
{
	return lastExecutionTime + delayTime < millis() && isRunning && canExecuted;
}

void Task::onAfterExecution()
{
	lastExecutionTime = millis();
}

