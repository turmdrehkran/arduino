// CommandTransceiver.h

#ifndef _COMMANDTRANSCEIVER_h
#define _COMMANDTRANSCEIVER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

enum MessageResponse
{
	Trying = 100,
	OK = 200,
	LeftLightbarrierReached = 510,
	RightLightbarrierReached = 520,
	RequestParsingFailed = 600
	// and many more...
};

class CommandTransceiverClass
{
private:
	String receive();

 public:
	void init();
	void update();
	bool isAvailable();

	void send(MessageResponse type, byte motorId);
};

extern CommandTransceiverClass CommandTransceiver;

#endif

