#include "Sched.h"
#include <ArduinoJson.h>
#include <TimeLib.h>
#include "CONFIG.h"
#include "Reqs.h"

extern Reqs req;
extern char ipayload[250];

void Sched::deseriTime(){
  DynamicJsonDocument root(200);
  deserializeJson(root, ipayload);
  serializeJsonPretty(root, Serial);
  unix = root["unix"];
  LLLL = root["LLLL"];
  zone = root["zone"];
}

void Sched::actTime(){
  time_t datetime = unix + zone*60*60;
  setTime(datetime);
  setSyncInterval(4000000); 
  printf("%d:%d \n", hour(), minute() );
}

