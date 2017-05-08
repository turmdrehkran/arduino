// 
// 
// 

#include "CommandTransceiver.h"

void CommandTransceiverClass::receive()
{
	Serial.println(F("receive..."));
	commands = Serial.readString();
}

void CommandTransceiverClass::init()
{
	messages = new Queue(sizeof(Command), 10, FIFO, false);
}

bool CommandTransceiverClass::isAvailable()
{
	return Serial.available();
}

bool CommandTransceiverClass::setTasks(Task** tasks, byte length)
{
	receive();

	char buf[commands.length()];
	commands.toCharArray(buf, sizeof(buf));
	char *p = buf;
	char *str;
	while ((str = strtok_r(p, ";", &p)) != NULL)
	{ 
		Serial.println(str);
		String command = String(str);
		// MOVE [MotorIndex] [NumberOfPerformings], z.B. MOVE 00 100, MOVE 01 2000, MOVE 03 2
		if (command.startsWith(F("MOVE")))
		{
			// index von 0 bis 9 möglich...
			int motorIndex = command.substring(5, 7).toInt();
			int numberOfPerformings = command.substring(8).toInt();
			Serial.println(motorIndex);
			Serial.println(numberOfPerformings);

			if (motorIndex < 3)
				tasks[motorIndex]->start(numberOfPerformings);
		}
		else if (command == F("STOP"))
		{
			for (int j = 0; j < length; ++j)
			{
				tasks[j]->stop();
			}
		}
	}

	

	return true;
}


CommandTransceiverClass CommandTransceiver;

