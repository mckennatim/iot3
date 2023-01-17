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
	void pubFlags();
	void pubTimr();
	void pubPrg(int ck);
	void pubState(int hc, PubSubClient& client);
	void processInc(); 
	void deseriReq(); 
	void deseriCmd(); 
	iscsidx_t getTypeIdx(int srid);
	int getStoredReading(int srid);
private:	
	bool dog;
	void creaJson(prg_t & p, char* astr);
	void clpub(char topic[20], char payload[200]);
};


#endif