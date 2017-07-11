// CommandTransceiver.h

#ifndef _COMMANDTRANSCEIVER_h
#define _COMMANDTRANSCEIVER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <SoftwareSerial.h>


#define B(x) ( \
  0##x >>  0 & 0001 | \
  0##x >>  2 & 0002 | \
  0##x >>  4 & 0004 | \
  0##x >>  6 & 0010 | \
  0##x >>  8 & 0020 | \
  0##x >> 10 & 0040 | \
  0##x >> 12 & 0100 | \
  0##x >> 14 & 0200 )

#define COMMAND_LENGTH 3
#define INPUT_DELIMITER "\n"
#define METHOD_DELIMITER " "
#define PARAMETER_DELIMITER ":"
#define MAX_SPEED 9999
#define MIN_SPEED 1
#define MAX_STEP 100000
#define MIN_STEP 1
#define BAUT 9600
#define TX_PIN 18
#define RX_PIN 19

enum Direction : byte
{
	Undefined = 0xFF,
	Left = 0x00,
	Right = 0x01 // TODO entsprechend LOW/HIGH und der motorenrichtung
};

typedef struct Command {
	byte MotorID;
	unsigned int Speed;
	char Direction;
	unsigned int NumberOfSteps;
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
	SoftwareSerial* BlueSerial; // RX, TX
	char input[256 + 1];
	unsigned char runDevice;
	// Booleanwerte fuer Rueckmeldung
	boolean commandOk;
	boolean syntaxError;
	boolean semanticError;
	boolean outOfBoundsError;
	boolean done;
	// Error Nachricht
	String errorMSG;
	byte checkAllDevice;

	Command commandList[COMMAND_LENGTH];

	void reset();
	void methodRUN(char* method, char* line, char* save1, char* save2);
	void sendBack();
	void setCode(unsigned int code, String msg);

	
 public:
	void init();
	void update();
	Command* getCommand(byte motorId);
	bool isAvailable();
	void send(MessageResponse type, byte motorId);
};

extern CommandTransceiverClass CommandTransceiver;

#endif

