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
	stop();
}

void Task::start()
{
	start(maxNumberOfPerformings);
}

void Task::start(uint16_t numberOfPerformings)
{
	this->numberOfPerformings = numberOfPerformings;
	isRunning = true;
}

bool Task::stop()
{
	if (!isRunning) 
	{
		return false;
	}

	isRunning = false;
	lastExecutionTime = 0L;
	numberOfPerformings = 0;
	return true;
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
	return lastExecutionTime + delayTime < millis() && isRunning && canExecuted && numberOfPerformings > 0;
}

bool Task::onAfterExecution()
{
	lastExecutionTime = millis();
	numberOfPerformings--;
	if (numberOfPerformings <= 0) 
	{
		return stop();
	}
	return false;
}

