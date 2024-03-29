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

signed long lckconn = -every6hrs;
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
  getOnline();
  client.setServer(mqtt_server, atoi(mqtt_port));
  client.setCallback(handleCallback); //in Req.cpp
  delay(2000);
  
}

void loop() {
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
  if (inow - lcksens > every2sec) {
    lcksens = inow;
    i_updInputs();
    u_scanFLAGand(f.HAYsTATEcNG, NUMSR, &i_updCtrl);
    u_scanFLAGand(f.CKaLARM, NUMSR, &s_ckAlarms);
    if(f.cONNectd) {
      q_pubState(client);
      q_pubPrg(client);
    }
    f.HAYsTATEcNG=0;
    f.CKaLARM=0;
  }    
} 
