#ifndef Reqs_h
#define Reqs_h

#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "CONFIG.h"
 
void q_processInc(); 
void q_pubState(PubSubClient& client);
void q_pubPrg(PubSubClient& client);

void deseriTime();
void deseriCmd();
void deseriProg();
void deseriReq();
#endif