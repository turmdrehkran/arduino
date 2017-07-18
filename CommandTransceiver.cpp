// 
// 
// 

#include "CommandTransceiver.h"

void CommandTransceiverClass::init()
{
	Serial1.begin(9600);
	Serial.begin(9600);
}

void CommandTransceiverClass::update()
{
	Serial1.flush();
	byte size = Serial1.readBytes(input, RECEIVERBUFFER);

	if (size > 0) {
		Serial1.println("EMPFANGE:");
		reset();
		input[size] = 0;
		Serial.println(input);
		char* save1;
		char* line = strtok_r(input, INPUT_DELIMITER, &save1);

		// methode bearbeiten
		char* save2;
		char* method = strtok_r(line, METHOD_DELIMITER, &save2);

		if (strcmp(method, "RUN") == 0) {
			semanticError = false;
			syntaxError = false;
			commandOk = false;
			outOfBoundsError = false;
			done = false;
			interpretMethodRun(method, line, save1, save2);
		}
		else {
			Serial1.println("Fehler: Methode existiert nicht! => " + (String)method);
			Serial1.flush();
			return;
		}
	}
}

void CommandTransceiverClass::reset()
{
	for (byte i = 0; i < COMMAND_LENGTH; ++i) 
	{
		commandList[i].MotorID = i;
		commandList[i].Direction = 'N';
		commandList[i].Speed = 0;
		commandList[i].NumberOfSteps = 0;
		commandList[i].Delivered = false;
	}
}

void CommandTransceiverClass::interpretMethodRun(char* methodArgs, char* methodBody, char* bodySave, char* argsSave)
{
	runDevice = B(0000000);
	checkAllDevice = B(00000000);
	byte allDevice = B(00000000);
	while (methodArgs != 0)
	{
		methodArgs = strtok_r(NULL, METHOD_DELIMITER, &argsSave);
		if (strcmp(methodArgs, "0") == 0) {
			allDevice = (allDevice << 1) | 1;
			if ((runDevice | B(1111)) == B(1111)) {
				if (runDevice == 0) {
					runDevice = B(100000); // Erster Motor
					commandList[0].MotorID = 0;
				}
				else if (runDevice == B(1000) || runDevice == B(0010)) {
					runDevice = runDevice | B(010000);
					commandList[1].MotorID = 0;
				}
				else if (runDevice == B(1001) || runDevice == B(0110)) {
					runDevice = runDevice | B(110000);
					commandList[2].MotorID = 0;
				}
			}
			else {
				Serial1.println("Fail: doppelte Besetzung der Motoren");
				errorMSG = "Fail: doppelte Besetzung der Motoren";
				syntaxError = true;
				argsSave = 0;
				methodArgs = 0;
			}
		}
		else if (strcmp(methodArgs, "1") == 0) {
			allDevice = (allDevice << 1) | 1;
			if ((runDevice | B(110011)) == B(110011)) {
				if (runDevice == 0) {
					runDevice = B(001000); // Erster Motor
					commandList[0].MotorID = 1;
				}
				else if (runDevice == B(100000) || runDevice == B(000010)) {
					runDevice = runDevice | B(000100);
					commandList[1].MotorID = 1;
				}
				else if (runDevice == B(100001) || runDevice == B(010010)) {
					runDevice = runDevice | B(001100);
					commandList[2].MotorID = 1;
				}
			}
			else {
				Serial1.println("Fail: doppelte Besetzung der Motoren");
				errorMSG = "Fail: doppelte Besetzung der Motoren";
				syntaxError = true;
				argsSave = 0;
				methodArgs = 0;
			}

		}
		else if (strcmp(methodArgs, "2") == 0) {
			allDevice = (allDevice << 1) | 1;
			if ((runDevice | B(111100)) == B(111100)) {
				if (runDevice == 0) {
					runDevice = B(000010); // Erster Motor
					commandList[0].MotorID = 2;
				}
				else if (runDevice == B(100000) || runDevice == B(001000)) {
					runDevice = runDevice | B(000001);
					commandList[1].MotorID = 2;
				}
				else if (runDevice == B(100100) || runDevice == B(011000)) {
					runDevice = runDevice | B(000011);
					commandList[2].MotorID = 2;
				}
			}
			else {
				Serial1.println("Fail: doppelte Besetzung der Motoren");
				errorMSG = "Fail: doppelte Besetzung der Motoren";
				syntaxError = true;
				argsSave = 0;
				methodArgs = 0;
			}

		}
		else {
			Serial1.println("Fail: ID existiet nicht => " + (String)methodArgs);
			errorMSG = "Fail: ID existiet nicht => " + (String)methodArgs;
			semanticError = true;
		}
	}


	methodBody = strtok_r(NULL, INPUT_DELIMITER, &bodySave);
	// parameter bearbeiten
	char* paramterSave;
	char* value;
	while (methodBody != 0)
	{
		strtok_r(methodBody, PARAMETER_DELIMITER, &paramterSave);
		checkAllDevice = B(00000000);
		value = '0';
		if (strcmp(methodBody, "Direction") == 0) {
			while (value != 0) {
				value = strtok_r(NULL, METHOD_DELIMITER, &paramterSave);
				if (strcmp(value, "L") == 0 || strcmp(value, "R") == 0 || value == 0) {
					if (value != 0) {
						checkAllDevice = (checkAllDevice << 1) | 1;
					}
					if (commandList[0].Direction == 'N') {
						commandList[0].Direction = *value;
					}
					else if (commandList[0].Direction != 'N' && commandList[1].Direction == 'N') {
						commandList[1].Direction = *value;
					}
					else if (commandList[0].Direction != 'N' && commandList[1].Direction != 'N' && commandList[2].Direction == 'N') {
						commandList[2].Direction = *value;
					}
					else if (value == 0) {
						Serial1.println("Direction");
					}
					else {
						Serial1.println("Fehler: zu viele Werte");
						errorMSG = "Fail: zu viele Werte";
						syntaxError = true;
						value = 0;
						methodBody = 0;
					}
				}
				else {
					Serial1.println("Fehler: falsche Werte in Direction");
					errorMSG = "Fail: falsche Werte in Direction";
					syntaxError = true;
					value = 0;
					methodBody = 0;
				}
			}
		}
		else if (strcmp(methodBody, "Speed") == 0) {
			while (value != 0) {
				value = strtok_r(NULL, METHOD_DELIMITER, &paramterSave);
				if (atoi(value) >= MIN_SPEED && atoi(value) <= MAX_SPEED || value == 0) { // atoi ist teuer, nur einmal verwenden. Was ist, wenn value == 0 ist?
					if (value != 0) {
						checkAllDevice = (checkAllDevice << 1) | 1; 
					}
					// Wäre an dieser Stelle nicht ein Zähler besser gewesen? 
					if (commandList[0].Speed == 0) {
						commandList[0].Speed = atoi(value);
					}
					else if (commandList[0].Speed != 0 && commandList[1].Speed == 0) {
						commandList[1].Speed = atoi(value);
					}
					else if (commandList[0].Speed != 0 && commandList[1].Speed != 0 && commandList[2].Speed == 0) {
						commandList[2].Speed = atoi(value);
					}
					else if (value == 0) {
						Serial1.println("Speed");
					}
					else {
						Serial1.println("Fehler: zu viele Werte");
						errorMSG = "Fail: zu viele Werte";
						syntaxError = true;
						value = 0;
						methodBody = 0;
					}
				}
				else {
					Serial1.println("Fehler: falsche Werte in Speed");
					errorMSG = "Fail: falsche Werte in Speed";
					syntaxError = true;
					value = 0;
					methodBody = 0;
				}
			}
		}
		else if (strcmp(methodBody, "NumSteps") == 0) {
			while (value != 0) {
				value = strtok_r(NULL, METHOD_DELIMITER, &paramterSave);
				if (atoi(value) >= MIN_STEP  && atoi(value) < MAX_STEP || value == 0) {
					if (value != 0) {
						checkAllDevice = (checkAllDevice << 1) | 1;
					}
					if (commandList[0].NumberOfSteps == 0) {
						commandList[0].NumberOfSteps = atoi(value);
					}
					else if (commandList[0].NumberOfSteps != 0 && commandList[1].NumberOfSteps == 0) {
						commandList[1].NumberOfSteps = atoi(value);
					}
					else if (commandList[0].NumberOfSteps != 0 && commandList[1].NumberOfSteps != 0 && commandList[2].NumberOfSteps == 0) {
						commandList[2].NumberOfSteps = atoi(value);
					}
					else if (value == 0) {
						Serial1.println("NumSteps");
					}
					else {
						Serial1.println("Fehler: zu viele Werte");
						errorMSG = "Fail: zu viele Werte";
						syntaxError = true;
						value = 0;
						methodBody = 0;
					}
				}
				else {
					Serial1.println("Fehler: falsche Werte in NumSteps");
					errorMSG = "Fail: falsche Werte in NumSteps";
					syntaxError = true;
					value = 0;
					methodBody = 0;
				}
			}
		}
		else {
			Serial1.println("Fehler: Key existiert nicht!");
			errorMSG = "Fail: Key existiert nicht!";
			syntaxError = true;
		}
		if (checkAllDevice != allDevice) {
			Serial1.println("Fehler: Falsche Angaben");
			commandOk = false;
			methodBody = 0;
		}
		else {
			Serial1.println("Variablen okay");
			Serial1.println("");
			methodBody = strtok_r(0, INPUT_DELIMITER, &bodySave);
			commandOk = true;
			syntaxError = false;
			semanticError = false;
		}
	}
}


void CommandTransceiverClass::sendBack()
{
	if (commandOk) {
		int i = 0;
		Serial.println("Send ok:");
		while (checkAllDevice > B(00000000)) {
			checkAllDevice = (checkAllDevice >> 1) | 0;
			Serial1.println("100 OPERATING Motor: " + (String)commandList[i].MotorID + "\n");
			i++;
		}
	}
	else if (done) {
		Serial1.println("200 DONE \n");
	}
	else if (syntaxError && !semanticError && !outOfBoundsError) {
		Serial1.println("400 SYNTAX " + errorMSG + "\n");
	}
	else if (!syntaxError && semanticError && !outOfBoundsError) {
		Serial1.println("410 SEMNATIC " + errorMSG + "\n");
	}
	else if (!syntaxError && !semanticError && outOfBoundsError) {
		Serial1.println("550 OUTOFBOUNDS " + errorMSG + "\n");
	}
	else {
		Serial1.println("unbekannt " + errorMSG + "\n");
	}
}


void CommandTransceiverClass::setCode(unsigned int code, String msg)
{
	errorMSG = msg;
	// Zuruecksetzen aller Errors
	commandOk = false;
	semanticError = false;
	syntaxError = false;
	done = false;
	outOfBoundsError = false;
	switch (code) {
	case 200: done = true; break;
	case 400: syntaxError = true; break;
	case 410: semanticError = true; break;
	case 550: outOfBoundsError = true; break;
	default:break;
	}
	sendBack();
}

void CommandTransceiverClass::send(MessageResponse type, byte motorId)
{
	String message;
	message.reserve(32);

	message += type;

	if (type == MessageResponse::Operating)
	{
		message += F(" OPERATING ");
	}
	else if (type == MessageResponse::Done)
	{
		message += F(" DONE ");
	}
	else if (type == MessageResponse::Syntax)
	{
		message += F(" SYNTAX ");
	}
	else if (type == MessageResponse::Semantic)
	{
		message += F(" SEMANTIC ");
	}
	else if (type == MessageResponse::OutOfBound)
	{
		message += F(" OUTOFBOUND ");
	}

	if (message.length() > 0)
	{
		message += motorId;
		Serial1.println(message);
	}
}

Command * CommandTransceiverClass::getCommand(byte motorId)
{
	Command* command = nullptr;
	for (byte i = 0; i < COMMAND_LENGTH; i++) 
	{

		if (commandList[i].MotorID == motorId && commandList[i].Speed > 0) 
		{
			command = &commandList[i];
		}
	}

	return command;
}

bool CommandTransceiverClass::isAvailable()
{
	return true; // BlueSerial.available();
}

CommandTransceiverClass CommandTransceiver;

