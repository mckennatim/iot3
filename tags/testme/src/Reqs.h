#ifndef Reqs_h
#define Reqs_h

#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "CONFIG.h"

// struct iscsidx_t {
// 	int srtype;
// 	int idx;
// };

class Reqs{
public:
  Reqs(char* devid, PubSubClient& client);
  PubSubClient cclient;
  char* cdevid;
  void stime();
	void pubFlags();
	void pubTimr();
	void pubPrg(int ck);
	void pubState(int hc);
	void processInc(); 
	void deseriReq(); 
	void deseriCmd(); 
	iscsidx_t getTypeIdx(int srid);
	int getStoredReading(int srid);
private:	
	bool dog;
	void creaJson(prg_t & p, char* astr);
	void clpub(char status[20], char astr[200]);
	//void copyHiLoState(int id, senso_t& t, JsonArray& ev, int port);
	//void copyTimrState(int id, timr_t& t, JsonArray& ev, int port);
};

#endif