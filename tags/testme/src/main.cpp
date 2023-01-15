#include <Arduino.h>
#include <EEPROM.h>
#include <TimeLib.h>
#include <TimeAlarms.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "ConnWIFI.h" //getOnline()
#include "CONFIG.h"
#include "MQclient.h"//globals(extern) NEW_MAIL, itopic, ipayload + Console

const long every6hrs = 21600000;
const long every5sec = 5000;
const long every2sec = 2000;

signed long lckconn = -every6hrs;
signed long lcktimr = 0;
signed long lcksens = 0;

WiFiClient espClient;
PubSubClient client(espClient);
Console console(devid, client);
MQclient mq(devid, owner, pwd);

void initShit(){
}

void setup() {
  Serial.begin(115200);
  EEPROM.begin(512);
  initShit();
  setTime(8,29,0,1,1,11);
  client.setServer(mqtt_server, atoi(mqtt_port));
  client.setCallback(handleCallback); //in Req.cpp
  Serial.println("leaving setup");

}

void loop() {
  Alarm.delay(100); //needed so timealarms works
  unsigned long inow = millis();
  
  if(inow-lckconn >= every6hrs){
    Serial.print("dif: ");
    Serial.println(inow-lckconn);
    Serial.print("every6hrs: ");
    Serial.println(every6hrs);
    lckconn = inow;
    if (!f.cONNectd){
        if (!f.hayWIFI) getOnline();
        mq.reconn(client);
        f.cONNectd = f.hayWIFI & f.hayMQTT;
    }
  }
}  
