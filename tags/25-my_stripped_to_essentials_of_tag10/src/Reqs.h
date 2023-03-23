#ifndef Reqs_h
#define Reqs_h

#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "CONFIG.h"
 
void q_processInc(); 
void q_pubState(PubSubClient& client);


#endif