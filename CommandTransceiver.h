// CommandTransceiver.h

#ifndef _COMMANDTRANSCEIVER_h
#define _COMMANDTRANSCEIVER_h

#include "Task.h"

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

enum MessageType
{
	Response,
	Request,
	Cancelation
};

class CommandTransceiverClass
{
private:
	String receive();

 public:
	void init();
	bool isAvailable();

	bool setTasks(Task** tasks, byte length);
	void send(MessageType type, byte motorId);
};

extern CommandTransceiverClass CommandTransceiver;

#endif

