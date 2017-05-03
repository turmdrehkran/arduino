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

Task* tasks[6];

void setup()
{
	tasks[0] = new LedBlinkTask();
	tasks[0]->init(50, 12);
	tasks[1] = new LedBlinkTask();
	tasks[1]->init(100, 13);
	tasks[2] = new LedBlinkTask();
	tasks[2]->init(250, 10);

	tasks[3] = NULL;
	tasks[4] = NULL;
	tasks[5] = NULL;

	Serial.begin(9600);
}

void loop()
{
	updateTasks();
	updateKeys();

	if (keypad.getState() == IDLE && Serial.available()) 
	{
		// readStringUntil('\n');
		String command = Serial.readStringUntil('\n');
		Serial.println(command);

		// MOVE [MotorIndex] [NumberOfPerformings], z.B. MOVE 00 100, MOVE 01 2000, MOVE 03 2
		if (command.startsWith("MOVE")) 
		{
			// index von 0 bis 9 möglich...
			int motorIndex = command.substring(5, 7).toInt();
			int numberOfPerformings = command.substring(8).toInt();
			Serial.println(motorIndex);
			Serial.println(numberOfPerformings);

			if (motorIndex < 3)
				tasks[motorIndex]->start(numberOfPerformings);
		}
		else if (command == "STOP")
		{
			tasks[0]->stop();
		}
	}
}

void updateTasks()
{
	for (byte i = 0; i < 6; ++i)
	{
		if (tasks[i] != NULL)
			tasks[i]->update();
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
