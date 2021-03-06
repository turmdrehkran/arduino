// 
// 
// 

#include "CommandTransceiver.h"

void CommandTransceiverClass::init()
{
	Serial1.begin(9600);
	memset(input, 0, INPUT_LENGTH);
	finished = 0;
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
	// Ein neuer Befehl kann nur abgesetzt werden, wenn kein Motor mehr einen Befehl abarbeitet.
	// Dies ist notwendig, da das Auslesen der seriellen Schnittstelle dauert und die Motoren ansonsten nicht richtig drehen (Timing!)
	if (Serial1.available() && finished == 0) 
	{
		reset();

		String buffer;
		buffer.reserve(INPUT_LENGTH);
		
		do 
		{
			int chunk = Serial1.read();
			if (chunk >= 0) 
			{
				//Serial1.println(chunk);
				buffer += (char)chunk;
			}

		} while (buffer[buffer.length() - 2] != buffer[buffer.length() - 1] || buffer[buffer.length() - 1] != '\n');
		
		if (buffer.length() > INPUT_LENGTH) 
		{
			send(MessageResponse::Syntax, F("Received Message too long."));
			return;
		}

		memcpy(input, buffer.c_str(), buffer.length());

		char* methodParameters;
		char* line = strtok_r(input, METHOD_LINE_DELIMITER, &methodParameters);

		// methode bearbeiten
		char* methodArgs;
		char* method = strtok_r(input, METHOD_ARGS_DELIMITER, &methodArgs);

		if (strcasecmp(method, "RUN") == 0)
		{
			interpretMethod_Run(methodArgs, methodParameters);

			// Command erfolgreich erhalten und alles ok!
			hasData = true;
		}
		else if (strcasecmp(method, "CAL") == 0)
		{
			inCalibration = true;
			finished = MAX_COMMAND_ID;

			// TODO motoren anpassen an echten Kran: Direction und Speed
			for (byte i = 0; i < MAX_COMMAND_ID; ++i)
			{
				Command& cmd = commandList[i];

				cmd.MotorID = i;
				cmd.Direction = 'L';
				cmd.NumSteps = NUM_STEPS_MAX;
				cmd.Speed_ms = 5;
			}
			hasData = true;

			send(MessageResponse::CalOperating);
		}
		else
		{
			send(MessageResponse::Syntax, F("Unknown Method"));
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
	inCalibration = false;
	error = false;
	memset(input, 0, INPUT_LENGTH);
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
					send(MessageResponse::Semantic, F("Multiple settings of a single motor"));
					return;
				}
			}
			finished = numberOfCommands;
		}
		else 
		{
			send(MessageResponse::Semantic, F("Motor does not exist"));
			return; // Auslesen abbrechen
		}

		methodArgs = strtok_r(NULL, METHOD_ARGS_DELIMITER, &save);
	}

	save = NULL;
	char* keyValues = strtok_r(methodParameters, METHOD_LINE_DELIMITER, &save);

	char* keyValuesSave;
	char* key;
	while (keyValues != 0)
	{
		key = strtok_r(keyValues, PARAMETER_DELIMITER, &keyValuesSave);

		if (strcasecmp_PF(key, F("Speed")) == 0)
		{
			if (!interpretParameter_Speed(keyValuesSave, numberOfCommands)) 
			{
				send(MessageResponse::Syntax, 0xFF);
				keyValues = 0;
			}
		}
		else if (strcasecmp_PF(key, F("NumSteps")) == 0)
		{
			interpretParameter_NumberOfSteps(keyValuesSave, numberOfCommands);

		} 
		else if (strcasecmp_PF(key, F("Direction")) == 0) 
		{
			interpretParameter_Direction(keyValuesSave, numberOfCommands);
		}
		else 
		{
			send(MessageResponse::Syntax, F("Unknown parameter found"));
			keyValues = 0;
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
				send(MessageResponse::Syntax, F("Out of Range Speed"));
				value = 0;
				return false;
			}
		}
		else
		{
			send(MessageResponse::Semantic, F("Motor does not exist"));
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

			if (direction == DIRECTION_LEFT || direction == DIRECTION_RIGHT) 
			{
				commandList[i].Direction = direction;
				i++;
				value = strtok_r(NULL, VALUE_DELIMITER, &save);
			}
			else
			{
				send(MessageResponse::Syntax, F("Unknown Direction"));
				value = 0;
				return false;
			}
		}
		else
		{
			send(MessageResponse::Semantic, F("Motor does not exist"));
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
			if (nsteps > NUM_STEPS_MIN && nsteps <= NUM_STEPS_MAX)
			{
				commandList[i].NumSteps = nsteps;
				i++;
				value = strtok_r(NULL, VALUE_DELIMITER, &save);
			}
			else
			{
				send(MessageResponse::Syntax, F("Out of Range NumSteps"));
				value = 0;
				return false;
			}
		}
		else
		{
			send(MessageResponse::Semantic, F("Motor does not exist"));
			value = 0;
			return false;
		}
	}
	return true;
}

void CommandTransceiverClass::send(String & message)
{
	message += F("\n\n");
	Serial1.print(message);
	Serial1.flush();
}

bool CommandTransceiverClass::isAvailable()
{
	return hasData;
}

Command * CommandTransceiverClass::getCommand(byte motorId)
{
	for (byte i = 0; i < MAX_COMMAND_ID; i++) 
	{
		if (commandList[i].MotorID == motorId && commandList[i].Direction != 'N' && !commandList[i].Delivered) 
		{
			commandList[i].Delivered = true;
			return &commandList[i];
		}
	}

	return nullptr;
}

void CommandTransceiverClass::setFinished(byte motorId)
{
	finished--;
	if (finished == 0) 
	{
		send(MessageResponse::OK);
		hasData = false;
	}
}

void CommandTransceiverClass::setError(byte motorId)
{
	if (inCalibration) // Kalibrierungsfahrt beendet
	{
		finished--;
		if (finished == 0) 
		{
			send(MessageResponse::CalOK);
			inCalibration = false;
		}
	}
	else // Echter Fehler
	{
		finished = 0;
		hasData = false;

		error = true;
		send(MessageResponse::OutOfBound, motorId);
	}
}

bool CommandTransceiverClass::hasError()
{
	return error;
}

bool CommandTransceiverClass::isCalibrating()
{
	return inCalibration;
}

void CommandTransceiverClass::send(MessageResponse type)
{
	String message;
	message.reserve(32);

	message += type;

	if (type == MessageResponse::OK)
	{
		message += F(" OK");
	}
	else if (type == MessageResponse::CalOperating)
	{
		message += F(" CALOPERATING");
	}
	else if (type == MessageResponse::CalOK)
	{
		message += F(" CALOK");
	}

	send(message);
}

void CommandTransceiverClass::send(MessageResponse type, byte motorId)
{
	String message;
	message.reserve(32);

	message += type;

	if (type == MessageResponse::Operating)
	{
		message += F(" OPERATING \nMotor: ");
		message += motorId;
	}
	else if (type == MessageResponse::OutOfBound)
	{
		message += F(" OUTOFBOUND ERROR\nMotor: ");
		message += motorId;
	}
	
	send(message);
}

void CommandTransceiverClass::send(MessageResponse type, const __FlashStringHelper *textLiteral)
{
	String message;
	message.reserve(32);

	message += type;

	if (type == MessageResponse::Semantic)
	{
		message += F(" SEMANTIC");
	}
	else if (type == MessageResponse::Syntax)
	{
		message += F(" SYNTAX");
	}

	String text(textLiteral);
	if (text.length() > 0)
	{
		message += F("\nMessage: ");
		message += text;
	}

	send(message);
}

CommandTransceiverClass CommandTransceiver;

