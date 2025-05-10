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
#include "storage.h"


Preferences prefs;


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

void setup() {
  Serial.begin(115200);
  
  prefs.begin("edgedata");
  prefs.putInt("re0", 2600);
  prefs.putInt("re1", 2600);
  prefs.putInt("re2", 2600);
  prefs.putInt("re3", 2600);

  


  // EEPROM.begin(512);
  WiFi.mode(WIFI_STA); // Force to station mode because if device was switched off while in access point mode it will start up next time in access point mode.
  c_initShit();
  getOnline();
  client.setServer(mqtt_server, atoi(mqtt_port));
  client.setCallback(handleCallback); //in Req.cpp
  delay(2000);
  Serial.println("Starting up...");
  Serial.println(srs[0].data[1]);
  srs[0].data[1] = prefs.getInt("re0", 3400);
  srs[1].data[1] = prefs.getInt("re1", 3401);
  srs[2].data[1] = prefs.getInt("re2", 3402);
  srs[3].data[1] = prefs.getInt("re3", 3403);
  Serial.println(srs[0].data[1]);
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
        q_pubState(client);
        q_pubPrg(client);
        NEW_MAIL=0;
    }
  }
  i_updInputs();
  if (inow - lcksens > every200msec) {
    lcksens = inow;
    i_updCtrl();
    u_scanFLAGand(f.CKaLARM, NUMSR, &s_ckAlarms);
    i_updRelays();
    // if(f.cONNectd) {
    //   q_pubState(client);
    //   q_pubPrg(client);
    // }
    f.CKaLARM=0;
  }    
} 
