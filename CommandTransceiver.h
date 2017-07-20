// CommandTransceiver.h

#ifndef _COMMANDTRANSCEIVER_h
#define _COMMANDTRANSCEIVER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#define INPUT_LENGTH 256
#define COMMAND_LENGTH 3
#define INPUT_DELIMITER "\n"
#define METHOD_DELIMITER " "
#define PARAMETER_DELIMITER ":"
#define VALUE_DELIMITER " "

enum Direction : byte
{
	Undefined = 0xFF,
	Left = 0x00,
	Right = 0x01 // TODO entsprechend LOW/HIGH und der motorenrichtung
};

typedef struct Command {
	byte MotorID;
	unsigned int Speed;
	Direction Direction;
	unsigned NumberOfSteps;
	bool HoldingTorgue;
	bool Delivered;
} Command;

enum MessageResponse
{
	Operating = 100,
	Done = 200,
	Syntax = 400,
	Semantic = 410,
	OutOfBound = 550
	// and many more...
};

class CommandTransceiverClass
{
private:
	bool isActive;
	char input[INPUT_LENGTH + 1];
	Command commandList[COMMAND_LENGTH];

	void reset();
	void interpretMethod_Run(char* methodArgs, char* methodParameters);
	void interpretParameter_Speed(byte* IDs, char* values, byte numberOfCommands);
	void interpretParameter_Direction(byte* IDs, char* values, byte numberOfCommands);
	void interpretParameter_HoldingTorgue(byte* IDs, char* values, byte numberOfCommands);
	void interpretParameter_NumberOfSteps(byte* IDs, char* values, byte numberOfCommands);

 public:
	void init();
	void update();
	bool isAvailable();
	Command* getCommand(byte motorId);

	void send(MessageResponse type, byte motorId);
};

extern CommandTransceiverClass CommandTransceiver;

#endif

