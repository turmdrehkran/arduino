// CommandTransceiver.h

#ifndef _COMMANDTRANSCEIVER_h
#define _COMMANDTRANSCEIVER_h

#include "Task.h"
#include <Queue.h>

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

typedef struct Command 
{
	String desciption;
	uint16_t id;
	uint16_t steps;
};

class CommandTransceiverClass
{
private:
	void receive();

 protected:
	 const static byte MAX_COMMANDS = 10;
	 String commandList[MAX_COMMANDS];
	 String commands;
	 
	 Queue* messages;

 public:
	void init();
	bool isAvailable();

	bool setTasks(Task** tasks, byte length);
};

extern CommandTransceiverClass CommandTransceiver;

#endif

