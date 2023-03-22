#ifndef Reqs_h
#define Reqs_h

#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "CONFIG.h"
 
// void pubState2(int hc, PubSubClient& client);

class Reqs{
public:
  Reqs(char* devid);
  char* cdevid;
	void processInc(); 
private:	
	bool dog;
};

#endif