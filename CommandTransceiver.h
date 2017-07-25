// CommandTransceiver.h

#ifndef _COMMANDTRANSCEIVER_h
#define _COMMANDTRANSCEIVER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#define INPUT_LENGTH 256
#define MAX_COMMAND_ID 3
#define MEHTOD_LINE_DELIMITER "\n"
#define METHOD_ARGS_DELIMITER " "
#define PARAMETER_DELIMITER ":"
#define VALUE_DELIMITER " "

#define SPEED_MIN_HZ 0
#define SPEED_MAX_HZ 1000
#define NUM_STEPS_MIN 1
#define NUM_STEPS_MAX INT_MAX // TODO schauen, obs knallt
#define DIRECTION_LEFT 'L'
#define DIRECTION_RIGHT 'R'

typedef struct Command {
	byte MotorID;
	unsigned int Speed;
	char Direction;
	unsigned NumSteps;
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
	Command commandList[MAX_COMMAND_ID];

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

