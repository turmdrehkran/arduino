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
#define METHOD_LINE_DELIMITER "\n"
#define METHOD_ARGS_DELIMITER " "
#define PARAMETER_DELIMITER ":"
#define VALUE_DELIMITER " "

#define SPEED_MIN_HZ 0
#define SPEED_MAX_HZ 1000
#define NUM_STEPS_MIN 1
#define NUM_STEPS_MAX 10000
#define DIRECTION_LEFT 'L'
#define DIRECTION_RIGHT 'R'

typedef struct Command {
	byte MotorID;
	unsigned int Speed_ms;
	char Direction;
	unsigned int NumSteps;
	bool Delivered;
} Command;

enum MessageResponse
{
	Operating = 100,
	CalOperating = 110,
	OK = 200,
	CalOK = 210,
	Syntax = 400,
	Semantic = 410,
	OutOfBound = 550
	// and many more...
};

class CommandTransceiverClass
{
private:
	byte finished;
	bool error;
	bool inCalibration;
	bool hasData;
	char input[INPUT_LENGTH + 1];
	Command commandList[MAX_COMMAND_ID];

	void reset();
	void interpretMethod_Run(char* methodArgs, char* methodParameters);
	bool interpretParameter_Speed(char* values, byte numberOfCommands);
	bool interpretParameter_Direction(char* values, byte numberOfCommands);
	bool interpretParameter_HoldingTorgue(char* values, byte numberOfCommands);
	bool interpretParameter_NumberOfSteps(char* values, byte numberOfCommands);

 public:
	void init();
	void update();
	bool isAvailable();
	Command* getCommand(byte motorId);
	void setFinished(byte motorId);
	void setError(byte motorId);
	bool hasError();
	bool isCalibrating();

	void send(MessageResponse type, byte motorId);
};

extern CommandTransceiverClass CommandTransceiver;

#endif

