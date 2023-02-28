#ifndef Sched_h
#define Sched_h

#include <Arduino.h>
#include "CONFIG.h"
#include <ArduinoJson.h>
void alarmRings();
#define sizeOf(a) (sizeof(a) / sizeof(a[0]))

class Sched{
public:
	void deseriTime(); 
	void actTime();
	void updTimers();
	void deseriProg(char* kstr);
	void ckAlarms();
	void ckRelays();
	void adjRelay(int id, cs_t& te);
private:
	void deductCrement(int id);
	void copyProg(prg_t& t, JsonArray ev);
	void setCur(prg_t& p, int &cur, int &nxt);
	void getTleft(prg_t p, int cur, int nxt, int &tleft);	
	void sortPrgsHms(prg_t prg[], int size);
	void showArray(const prg_t prg[], int size);
  time_t unix;
  const char* LLLL;
  int zone;	
};

#endif