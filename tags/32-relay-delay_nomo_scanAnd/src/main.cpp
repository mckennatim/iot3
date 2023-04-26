#include <Arduino.h>
#include <EEPROM.h>
#include <TimeLib.h>
#include <TimeAlarms.h>
// #include <ESP8266WiFi.h> //not needed as of tag 04
#include <PubSubClient.h>
#include "ConnWIFI.h" //getOnline()
#include "CONFIG.h"
#include "MQclient.h"//globals(extern) NEW_MAIL, itopic, ipayload + Console
#include "Reqs.h"
#include "Inp.h"
#include "Util.h"
#include "Sched.h"

const long every6hrs = 21600000;
const long every5sec = 5000;
const long every2sec = 2000;
const long everyHalfsec = 500;
const long every200msec = 200;

signed long lckconn = -every6hrs;
signed long lcksens = 0;

WiFiClient espClient;
PubSubClient client(espClient);
Console console(devid, client);
MQclient mq(devid, owner, pwd);



void initShit(){
  for (int i=0;i<NUMSR;i++){
    if (srs[i].port>=0) pinMode(srs[i].port, OUTPUT);
  }
  for (int i=0;i<NUMINP;i++){
    if (inp[i].port[0]>=0) pinMode(srs[i].port, INPUT);
  }
}

void setup() {
  Serial.begin(115200);
  EEPROM.begin(512);
  WiFi.mode(WIFI_STA); // Force to station mode because if device was switched off while in access point mode it will start up next time in access point mode.
  initShit();
  getOnline();
  client.setServer(mqtt_server, atoi(mqtt_port));
  client.setCallback(handleCallback); //in Req.cpp
  delay(2000);
  
}

void loop() {
  i_butnLoop();
  Alarm.delay(100); //needed so timealarms works
  unsigned long inow = millis();
  if(inow-lckconn >= every6hrs){
    lckconn = inow;
    if (!f.cONNectd){
        if (!f.hayWIFI) getOnline();
        mq.reconn(client);
        f.cONNectd = f.hayWIFI & f.hayMQTT;
        printf("f.hayWIFI = %d, f.hayMQTT = %d, f.cONNectd= %d\n",f.hayWIFI, f.hayMQTT, f.cONNectd);
    }
  }
  if (f.cONNectd){
    client.loop();
    if(NEW_MAIL){
        Serial.println("hay NEW_MAIL");
        q_processInc();
        NEW_MAIL=0;
    }
  }
  i_updInputs();
  if (inow - lcksens > every200msec) {
    lcksens = inow;
    i_updCtrl();
    u_scanFLAGand(f.CKaLARM, NUMSR, &s_ckAlarms);
    i_updRelays();
    if(f.cONNectd) {
      q_pubState(client);
      q_pubPrg(client);
    }
    f.CKaLARM=0;
  }    
} 
