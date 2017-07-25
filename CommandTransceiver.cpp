// 
// 
// 

#include "CommandTransceiver.h"

void CommandTransceiverClass::init()
{
	Serial.begin(9600);

	for (byte i = 0; i < MAX_COMMAND_ID; ++i)
	{
		Command& cmd = commandList[i];

		cmd.MotorID = 0;
		cmd.Direction = 'N';
		cmd.Delivered = false;
		cmd.NumSteps = 0;
		cmd.Speed_ms = 0;
	}
}

void CommandTransceiverClass::update()
{
	if (Serial.available()) 
	{
		reset();

		Serial.println(F("EMPFANGE:"));

		byte size = Serial.readBytes(input, INPUT_LENGTH);
		input[size] = 0;
		Serial.println(input);

		char* methodParameters;
		char* line = strtok_r(input, METHOD_LINE_DELIMITER, &methodParameters);

		// methode bearbeiten
		char* methodArgs;
		char* method = strtok_r(input, METHOD_ARGS_DELIMITER, &methodArgs);

		if (strcasecmp(method, "RUN") == 0)
		{
			Serial.println(F("101 PARSING"));
			interpretMethod_Run(methodArgs, methodParameters);

			for (byte i = 0; i < MAX_COMMAND_ID; i++)
			{
				Serial.println(commandList[i].MotorID);
				Serial.println(commandList[i].Speed_ms);
				Serial.println(commandList[i].NumSteps);
				Serial.println(commandList[i].Direction);

				Serial.println();
			}
		}
		else if (strcasecmp(method, "SHIT") == 0)
		{
			Serial.println(F("102 PARSING"));
		}
		else
		{
			Serial.println(method);
			Serial.println(F("400 SYNTAX"));
			return;
		}
	}
}

void CommandTransceiverClass::reset()
{
	for (byte i = 0; i < MAX_COMMAND_ID; ++i)
	{
		commandList[i].Delivered = false;
		commandList[i].Speed_ms = 0;
		commandList[i].Direction = 'N';
		commandList[i].NumSteps = 0;
	}
}

void CommandTransceiverClass::interpretMethod_Run(char * methodArgs, char * methodParameters)
{
	char* save;
	byte numberOfCommands = 0; // muss kleinergleich COMMAND_LENGTH sein

	char* args = strtok_r(methodArgs, METHOD_ARGS_DELIMITER, &save);
	while (methodArgs != 0)
	{
		int id = atoi(methodArgs); // TODO gegen strtol austauschen aufgrund fehleranfälligkeit
		if (id >= 0 && id < MAX_COMMAND_ID) 
		{
			commandList[numberOfCommands].MotorID = id;
			numberOfCommands++;

			for (byte i = 0; i < numberOfCommands - 1; i++)
			{
				if (commandList[i].MotorID == id)
				{
					Serial.println(F("500 SEMANTIC")); // ID doppelt in den Argumenten
					return;
				}
			}
		}
		else 
		{
			Serial.println(F("500 SEMANTIC")); // ID out of range
			return; // Auslesen abbrechen
		}

		methodArgs = strtok_r(NULL, METHOD_ARGS_DELIMITER, &save);
	}

	Serial.println("jetzt kommen die parameter!");
	save = NULL;
	char* keyValues = strtok_r(methodParameters, METHOD_LINE_DELIMITER, &save);

	// parameter bearbeiten
	char* keyValuesSave;
	char* key;
	while (keyValues != 0)
	{
		key = strtok_r(keyValues, PARAMETER_DELIMITER, &keyValuesSave);

		if (strcasecmp(key, "Speed") == 0)
		{
			if (!interpretParameter_Speed(keyValuesSave, numberOfCommands)) 
			{
				send(MessageResponse::Syntax, 0xFF);
				keyValues = 0;
			}
		}
		else if (strcasecmp(key, "NumSteps") == 0)
		{
			interpretParameter_NumberOfSteps(keyValuesSave, numberOfCommands);

		} 
		else if (strcasecmp(key, "Direction") == 0) //TODO Bug: Es wird nicht in diesen Fall gesprungen!
		{
			interpretParameter_Direction(keyValuesSave, numberOfCommands);
		}
		else 
		{
			Serial.print("Key=");
			Serial.println(key);
			send(MessageResponse::Syntax, 123);
		}

		keyValues = strtok_r(NULL, METHOD_LINE_DELIMITER, &save);
		key = NULL;
	}
}

bool CommandTransceiverClass::interpretParameter_Speed(char * values, byte numberOfCommands)
{
	char* save = NULL;
	char* value = strtok_r(values, VALUE_DELIMITER, &save);

	byte i = 0;

	while (value != 0) 
	{
		if (i < numberOfCommands) 
		{
			int speed_Hz = atoi(value); // TODO konvertierung testen

			if (speed_Hz > SPEED_MIN_HZ && speed_Hz <= SPEED_MAX_HZ)
			{
				commandList[i].Speed_ms = (unsigned int)1000 / speed_Hz;
				i++;
				value = strtok_r(NULL, VALUE_DELIMITER, &save);
			}
			else
			{
				send(MessageResponse::Syntax, commandList[i].MotorID);
				value = 0;
				return false;
			}
		}
		else
		{
			send(MessageResponse::Semantic, 0xFF);
			value = 0;
			return false;
		}
	}
	return true;
}

bool CommandTransceiverClass::interpretParameter_Direction(char * values, byte numberOfCommands)
{
	char* save = NULL;
	char* value = strtok_r(values, VALUE_DELIMITER, &save);

	byte i = 0;

	while (value != 0)
	{
		if (i < numberOfCommands)
		{
			int direction = value[0];
			Serial.print("dir=");
			Serial.println(direction);
			if (direction == DIRECTION_LEFT || direction == DIRECTION_RIGHT) 
			{
				commandList[i].Direction = direction;
				i++;
				value = strtok_r(NULL, VALUE_DELIMITER, &save);
			}
			else
			{
				send(MessageResponse::Syntax, commandList[i].MotorID);
				value = 0;
				return false;
			}
		}
		else
		{
			send(MessageResponse::Semantic, 0xFF);
			value = 0;
			return false;
		}
	}
	return true;
}

bool CommandTransceiverClass::interpretParameter_HoldingTorgue(char * values, byte numberOfCommands)
{
	return false;
}

bool CommandTransceiverClass::interpretParameter_NumberOfSteps(char * values, byte numberOfCommands)
{
	char* save = NULL;
	char* value = strtok_r(values, VALUE_DELIMITER, &save);

	byte i = 0;

	while (value != 0)
	{
		if (i < numberOfCommands)
		{
			int nsteps = atoi(value); // TODO konvertierung testen
			if (nsteps > NUM_STEPS_MIN && nsteps < NUM_STEPS_MAX)
			{
				commandList[i].NumSteps = nsteps;
				i++;
				value = strtok_r(NULL, VALUE_DELIMITER, &save);
			}
			else
			{
				send(MessageResponse::Syntax, commandList[i].MotorID);
				value = 0;
				return false;
			}
		}
		else
		{
			send(MessageResponse::Semantic, 0xFF);
			value = 0;
			return false;
		}
	}
	return true;
}

bool CommandTransceiverClass::isAvailable()
{
	return Serial.available();
}

void CommandTransceiverClass::send(MessageResponse type, byte motorId)
{
	String message;
	message.reserve(32);

	message += type;

	if (type == MessageResponse::Operating)
	{
		message += F(" Operating ");
	}
	else if (type == MessageResponse::Done)
	{
		message += F(" Done ");
	}
	else if (type == MessageResponse::Semantic)
	{
		message += F(" Semantic ");
	}
	else if (type == MessageResponse::Syntax)
	{
		message += F(" Syntax ");
	}

	if (message.length() > 0) 
	{
		message += motorId;
		Serial.println(message);
	}
}

CommandTransceiverClass CommandTransceiver;

