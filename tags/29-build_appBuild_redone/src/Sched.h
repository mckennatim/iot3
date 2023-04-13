#ifndef Sched_h
#define Sched_h

#include <Arduino.h>
#include "CONFIG.h"
#include <ArduinoJson.h>
// void alarmRings();

void s_ckAlarms(int sr, int b);

void findCurNxt(int idx, int &cur, int &nxt, int &tleft);

#endif