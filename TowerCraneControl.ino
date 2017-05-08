#include "CommandTransceiver.h"
#include <Keypad.h>
#include "LedBlinkTask.h"
#include "Task.h"

byte keys[2][3]{
	{ 1, 3, 5 },
	{ 2, 4, 6 }
};

byte rowPins[2] = { 6, 7 };
byte colPins[3] = { 3, 4, 5 };

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, 2, 3);

const byte MAX_TASKS = 6;
Task* tasks[MAX_TASKS];

bool isTaskSet;

void setup()
{
	CommandTransceiver.init();

	tasks[0] = new LedBlinkTask();
	tasks[0]->init(50, 11);
	tasks[1] = new LedBlinkTask();
	tasks[1]->init(100, 13);
	tasks[2] = new LedBlinkTask();
	tasks[2]->init(100, 12);

	tasks[3] = NULL;
	tasks[4] = NULL;
	tasks[5] = NULL;

	Serial.begin(9600);

	isTaskSet = false;
}

void loop()
{
	updateTasks();
	updateKeys();

	if (keypad.getState() == IDLE && CommandTransceiver.isAvailable()) 
	{
		stopTasks();
		isTaskSet = CommandTransceiver.setTasks(tasks, MAX_TASKS);
	}
	else if (keypad.getState() != IDLE && isTaskSet)
	{
		isTaskSet = false;
		stopTasks();
	}
}

void stopTasks() 
{
	for (byte i = 0; i < MAX_TASKS; ++i)
	{
		if (tasks[i] != NULL && tasks[i]->stop())
		{
			CommandTransceiver.send(MessageType::Cancelation, i);
		}
	}
}

void updateTasks()
{
	for (byte i = 0; i < MAX_TASKS; ++i)
	{
		if (tasks[i] != NULL) 
		{
			if (tasks[i]->update())
			{
				CommandTransceiver.send(MessageType::Response, i);
			}
		}
	}
}

void updateKeys()
{
	if (keypad.getKeys())
	{
		for (int i = 0; i < LIST_MAX; ++i)
		{
			if (keypad.key[i].stateChanged)
			{
				/*
					Das Keypad bindet die Keys an ein Character.
					Da der Character \0 als NO_KEY definiert ist, 
					beginnt die Nummerierung der Tasten bei 1.
					Da die Task-Liste null basierenden ist, wird bei vor der Interpretation
					der übergebene Index um 1 dekrementiert.
				*/
				interpretInteraction(keypad.key[i].kchar - 1, keypad.key[i].kstate);
			}
		}
	}
}

void interpretInteraction(char command, KeyState state)
{
	if (tasks[command] == NULL) {
		return;
	}

	switch (state)
	{
	case PRESSED:
		lockTask(command);
		tasks[command]->start();
		break;
	case HOLD:
		break;
	case RELEASED:
		break;
	case IDLE:
		tasks[command]->stop();
		unlockTask(command);
		break;
	default:
		break;
	}
}

void lockTask(char command)
{
	if (command % 2 == 0)
	{
		if (tasks[command + 1] != NULL)
			tasks[command + 1]->lock();
	}
	else
	{
		if (tasks[command - 1] != NULL)
			tasks[command - 1]->lock();
	}
}

void unlockTask(char command)
{
	if (command % 2 == 0)
	{
		if (tasks[command + 1] != NULL)
			tasks[command + 1]->unlock();
	}
	else
	{
		if (tasks[command - 1] != NULL)
			tasks[command - 1]->unlock();
	}
}
