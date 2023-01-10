#include <Arduino.h>
#include <EEPROM.h>
#include <TimeLib.h>
#include <TimeAlarms.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "ConnWIFI.h" //getOnline()
#include "CONFIG.h"
#include "MQclient.h"//globals(extern) NEW_MAIL, itopic, ipayload + Console


WiFiClient espClient;
PubSubClient client(espClient);
Console console(devid, client);
MQclient mq(devid, owner, pwd);

void initShit(){
 
}

void dailyAlarm(){
  Serial.print("in dailyAlarm() haywifi: ");
  Serial.println(haywifi);
  if(haywifi==0){
    haywifi=1;
    getOnline();
  }
  Serial.println("in daily alarm");
  Serial.print("assume haywifi: ");
  Serial.println(haywifi);
  Serial.println(devid);
  // int minu = (10*((int)devid[6]-'0')+(int)devid[7]-'0')%16;
  Serial.print(hour());
  Serial.print(':');
  Serial.println(minute());
  //Alarm.alarmRepeat(0,minu,0, getTime);
}

void setup() {
  Serial.begin(115200);
  EEPROM.begin(512);
  initShit();
  setTime(8,29,0,1,1,11); 
  getOnline();//ConnWIFI
  client.setServer(mqtt_server, atoi(mqtt_port));
  client.setCallback(handleCallback); //in Req.cpp
  Alarm.timerOnce(10, dailyAlarm);
}

void loop() {
  Alarm.delay(1000); //needed so timealarms works
  if(!client.connected() && haywifi){
    mq.reconn(client);
  }else{
    client.loop();
  }
}  
