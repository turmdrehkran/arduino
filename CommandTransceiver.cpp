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

		cmd.MotorID = i;
		cmd.Direction = '\0';
		cmd.Delivered = false;
		cmd.NumSteps = 0;
		cmd.Speed = 0;
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
		char* line = strtok_r(input, MEHTOD_LINE_DELIMITER, &methodParameters);

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
				Serial.println(commandList[i].Speed);
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
		commandList[i].Speed = 0;
		commandList[i].Direction = '\0';
		commandList[i].NumSteps = 0;
	}
}

void CommandTransceiverClass::interpretMethod_Run(char * methodArgs, char * methodParameters)
{
	char* save;
	byte numberOfCommands = 0; // muss kleinergleich COMMAND_LENGTH sein
	byte IDs[MAX_COMMAND_ID];

	char* args = strtok_r(methodArgs, METHOD_ARGS_DELIMITER, &save);
	while (methodArgs != 0)
	{
		int id = atoi(methodArgs); // TODO gegen strtol austauschen aufgrund fehleranfälligkeit
		if (id >= 0 && id < MAX_COMMAND_ID) 
		{
			IDs[numberOfCommands] = id;
			Serial.println(id);
			numberOfCommands++;

			for (byte i = 0; i < numberOfCommands - 1; i++)
			{
				if (IDs[i] == id)
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
	char* keyValues = strtok_r(methodParameters, MEHTOD_LINE_DELIMITER, &save);

	// parameter bearbeiten
	char* keyValuesSave;
	while (keyValues != 0)
	{
		char* key = strtok_r(keyValues, PARAMETER_DELIMITER, &keyValuesSave);

		if (strcasecmp(key, "Speed") == 0)
		{
			interpretParameter_Speed(IDs, keyValuesSave, numberOfCommands);
		}
		else if (strcasecmp(key, "Direction") == 0)
		{

		}

		keyValues = strtok_r(NULL, MEHTOD_LINE_DELIMITER, &save);
	}
}

void CommandTransceiverClass::interpretParameter_Speed(byte * IDs, char * values, byte numberOfCommands)
{

}

void CommandTransceiverClass::interpretParameter_Direction(byte * IDs, char * values, byte numberOfCommands)
{
}

void CommandTransceiverClass::interpretParameter_HoldingTorgue(byte * IDs, char * values, byte numberOfCommands)
{
}

void CommandTransceiverClass::interpretParameter_NumberOfSteps(byte * IDs, char * values, byte numberOfCommands)
{
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
		message += F(" DONE ");
	}

	if (message.length() > 0) 
	{
		message += motorId;
		Serial.println(message);
	}
}

CommandTransceiverClass CommandTransceiver;

