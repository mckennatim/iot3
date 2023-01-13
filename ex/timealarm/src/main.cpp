#include <Arduino.h>
#include <TimeLib.h>
#include <TimeAlarms.h>
// AlarmId id;

// functions to be called when an alarm triggers:
void MorningAlarm() {
  Serial.println("Alarm: - turn lights off");
}

void EveningAlarm() {
  Serial.println("Alarm: - turn lights on");
}

void WeeklyAlarm() {
  Serial.println("Alarm: - its Monday Morning");
}

void ExplicitAlarm() {
  Serial.println("Alarm: - this triggers only at the given date and time");
}

void Repeats() {
  Serial.println("15 second timer");
}

void Repeats2() {
  Serial.println("2 second timer");
}

void OnceOnly() {
  Serial.println("This timer only triggers once, stop the 2 second timer");
  // use Alarm.free() to disable a timer and recycle its memory.
  // Alarm.free(id);
  // optional, but safest to "forget" the ID after memory recycled
  // id = dtINVALID_ALARM_ID;
  // you can also use Alarm.disable() to turn the timer off, but keep
  // it in memory, to turn back on later with Alarm.enable().
}

void printDigits(int digits) {
  Serial.print(":");
  if (digits < 10)
    Serial.print('0');
  Serial.print(digits);
}


void digitalClockDisplay() {
  // digital clock display of the time
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.println();
}


void setup() {
  Serial.begin(115200);
  while (!Serial) ; // wait for Arduino Serial Monitor

  // setTime(8,29,0,1,1,11); // set time to Saturday 8:29:00am Jan 1 2011

  // // create the alarms, to trigger at specific times
  // Alarm.alarmRepeat(8,30,0, MorningAlarm);  // 8:30am every day
  // Alarm.alarmRepeat(17,45,0,EveningAlarm);  // 5:45pm every day
  // Alarm.alarmRepeat(dowSaturday,8,30,30,WeeklyAlarm);  // 8:30:30 every Saturday

  // // create timers, to trigger relative to when they're created
  // Alarm.timerRepeat(15, Repeats);           // timer for every 15 seconds
  // id = Alarm.timerRepeat(2, Repeats2);      // timer for every 2 seconds
  Alarm.timerOnce(10, OnceOnly);            // called once after 10 seconds
}

void loop() {
  // digitalClockDisplay();
  Alarm.delay(1000); // wait one second between clock display
}

