// 
// 
// 

#include "CommandTransceiver.h"

String CommandTransceiverClass::receive()
{
	Serial.println(F("receive..."));
	return Serial.readString();
}

void CommandTransceiverClass::init()
{
}

bool CommandTransceiverClass::isAvailable()
{
	return Serial.available();
}

bool CommandTransceiverClass::setTasks(Task** tasks, byte length)
{
	String commands = receive();

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

void CommandTransceiverClass::send(MessageType type, byte motorId)
{
	String message;
	if (type == MessageType::Response) 
	{
		message = F("FINISHED ");
	}
	else if (type == MessageType::Cancelation) 
	{
		message = F("CANCELED ");
	}

	if (message.length() > 0) 
	{
		message += motorId;
		Serial.println(message);
	}
}

CommandTransceiverClass CommandTransceiver;

