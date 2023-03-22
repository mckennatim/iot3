#ifndef Sched_h
#define Sched_h

#include <Arduino.h>
#include "CONFIG.h"
#include <ArduinoJson.h>
// void alarmRings();

class Sched{
public:
	void deseriTime(); 
	void actTime();
private:
  time_t unix;
  const char* LLLL;
  int zone;	
};

#endif