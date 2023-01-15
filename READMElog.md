# READMElog

## log

### 04-WIFI_MQTTconn-v2
Part of the process of keeping a default loop running even without connection. No more alarm every day. Every 6 hours, if !f.cONNectd, getOnline and mq.reconn run, setting hayWIFI and hayMQTT. By setting the lckconn= -every6hrs the timer gets set off to run at startup. If both hayWIFI and hayMQTT are true then f.cONNectd get set true. This flag is used to define default and connected operation.

Leaves off with handleCallback working and setting NEW_MAIL=1. Go from there.

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
    Reqs req(devid, client);

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
            req.processInc();
            NEW_MAIL=0;
        }
      }  
    } 

    *wm:STA IP Address: 192.168.1.99
    connected...yeey :)
    out of getOnline()
    Attempting remo MQTT connection...connected
    in mq.reconn->devid/time, <-/prg&/devtime
    f.hayWIFI = 1, f.hayMQTT = 1, f.cONNectd= 1
    Payload size is 107
    hay NEW_MAIL

### 03-getOnline_mq.reconn
WifiManager is working. mq.reconn is erroring out since the server doesn't know CYURD127

This is the core connection part of the old version. The problem with it is that there is no need to have getonline() in setup and there is nothing telling you both hayWIFI and hayMQTT are true. 

Also, the old style just doesn't work at keeping you connected. It will infinite loop if connection problems and there is no core set of functions that will work without a connection.


### 02-AlarmTimerOnce
needs Alarm.delay(1000); in loop to work

### 01-blink
for esp8266 and esp32

### 00 start iot3
The latest working is getting put into iot3/my slowly (since it is not compiling iin espBoth)
