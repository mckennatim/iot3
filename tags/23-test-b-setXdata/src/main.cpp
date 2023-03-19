#include <Arduino.h>
#include <EEPROM.h>
#include <TimeLib.h>
#include <TimeAlarms.h>
// #include <ESP8266WiFi.h> //not needed as of tag 04
#include <Wire.h>
#include <PubSubClient.h>
#include "ConnWIFI.h" //getOnline()
#include "CONFIG.h"
#include "MQclient.h"//globals(extern) NEW_MAIL, itopic, ipayload + Console
#include "Reqs.h"
#include "Sched.h"
/* sensor libraries */
#include <DallasTemperature.h>
#include <BH1750.h>
#include <DHT.h>


const long every6hrs = 21600000;
const long every5sec = 5000;
const long every2sec = 2000;

signed long lckconn = -every6hrs;
signed long lcksens = 0;

WiFiClient espClient;
PubSubClient client(espClient);
Console console(devid, client);
Reqs req(devid, client);
MQclient mq(devid, owner, pwd);
Sched sched2;

OneWire oneWire(D2);
DallasTemperature DS18B20(&oneWire);

// OneWire oneWireb(D3);
// DallasTemperature DS18B20(&oneWireb);
DHT dht(D5, DHT11);
// BH1750 lightMeter;
// Adafruit_HTU21DF htu = Adafruit_HTU21DF();

void initShit(){
  /*set pinmode for all ports and init vals for relays*/
  for(int j=0;j<ports.numports;j++){
    if(ports.port[j].in>=0){pinMode(ports.port[j].in,INPUT);}
    if(ports.port[j].out>=0){
      int srid = ports.port[j].sr;
      pinMode(ports.port[j].out,OUTPUT);
      for (int i=0; i<srs.numcs;i++){
        if(srs.cs[i].sr==srid){
          digitalWrite(ports.port[j].out, srs.cs[i].onoff);
        }
      }
      for (int i=0; i<srs.numrel;i++){
        if(srs.rel[i].sr==srid){
          digitalWrite(ports.port[j].out, srs.rel[i].onoff);
        }
      }
    }
  }
  for (int i=0;i<SE.numtypes;i++){
    Serial.println(SE.stype[i].model);
    // if(strcmp(SE.stype[i].model, "BH1750")==0){
    //   Wire.begin();
    //   lightMeter.begin();
    //   Serial.println(F("BH1750 light begin"));
    // }
    if(strcmp(SE.stype[i].model, "DHT11")==0){
      dht.begin();
      Serial.println(F("DHT11 temp/hum begin"));
    }    
    // if(strcmp(SE.stype[i].model, "MAX31855")==0){
    //   tc.begin();
    //   Serial.println(F("MAX31855 thermoco begin"));
    // }    
    // if(strcmp(SE.stype[i].model, "HTU21D")==0){
    //   htu.begin();
    //   Serial.println(F("HTU21D temp-hum begin"));
    // }    
  } 
  customInit();
  // initDiff();
}

/*select between se and cs, used by setIfDif*/
void setSrs(int srid, int reading){
  for (int i=0; i<srs.numse;i++){
    if(srs.se[i].sr==srid){
      srs.se[i].reading=reading;
      ports.port[srid].isnew=1;
    }
  }
  for (int j=0; j<srs.numcs;j++){
    if(srs.cs[j].sr==srid){
      srs.cs[j].reading=reading;
      ports.port[srid].isnew=1;
      sched2.adjRelay(srid, srs.cs[j]);
    }
  } 
}

/*sets srs based upon input port, called at each sensor reading*/
void setIfDif (int srid, int reading, int old, int dif, int hi, int lo)
{
  if(abs(reading-old)>=dif && reading<hi && reading>lo){
    setSrs(srid, reading);
    int bit =pow(2,srid);
    f.HAYsTATEcNG=f.HAYsTATEcNG | bit; 
    printf("main setIfDif hay %d: Sensor %d: reading: %d  old: %d \n",f.HAYsTATEcNG, srid, reading, old);
  }
}


void readSensors(){
  int senvals[SE.numsens]; //store readings here
  int sr;
  int old;
  for(int i=0;i<SE.numtypes;i++){//check all the types incl.
    if(strcmp(SE.stype[i].model, "DS18B20")==0){
      for(int j=0;j<SE.stype[i].nums;j++) {
        DS18B20.requestTemperatures(); 
        sr = SE.stype[i].ids[j];
        old = req.getStoredReading(sr);
        senvals[sr] = (int)DS18B20.getTempFByIndex(j);
        // printf("old = %d, new = %d \n", old, senvals[sr]);
        setIfDif (sr, senvals[sr] , old, 1, 220, -20);
      }
    // }else if(strcmp(SE.stype[i].model, "DS18B20b")==0){
    //   for(int j=0;j<SE.stype[i].nums;j++) {
    //     sr = SE.stype[i].ids[j];
    //     DS18B20b.requestTemperatures(); 
    //     sr = SE.stype[i].ids[j];
    //     old = req.getStoredReading(sr);
    //     senvals[sr] = (int)DS18B20b.getTempFByIndex(j);
    //     setIfDif (sr, senvals[sr] , old, 1, 220, -20);
    //   }
    // }else if(strcmp(SE.stype[i].model, "BH1750")==0){
    //   int lux = (int)lightMeter.readLightLevel();
    //   sr = SE.stype[i].ids[0];
    //   senvals[sr] = lux;
    //   sr = SE.stype[i].ids[0];
    //   old = req.getStoredReading(sr);
    //   setIfDif (sr, senvals[sr] , old, 5, 1500, 0);
    // }else if(strcmp(SE.stype[i].model, "HTU21D")==0){
    //   int srt = SE.stype[i].ids[0];
    //   int srh = SE.stype[i].ids[1];   
    //   senvals[srt] = (int)(htu.readTemperature()*9/5 +32);
    //   senvals[srh] = (int)htu.readHumidity();
    //   old = req.getStoredReading(srt);
    //   setIfDif (srt, senvals[srt] , old, 1, 120, -20);
    //   old = req.getStoredReading(srh);
    //   setIfDif (srh, senvals[srh] , old, 1, 100, 0);
    // }else if(strcmp(SE.stype[i].model, "DHT22")==0){
    //   int srt = SE.stype[i].ids[0];
    //   int srh = SE.stype[i].ids[1];      
    //   senvals[srt] = (int)(dht.readTemperature(true));
    //   senvals[srh] = (int)dht.readHumidity();
    //   old = req.getStoredReading(srt);
    //   setIfDif (srt, senvals[srt] , old, 1, 120, -20);
    //   old = req.getStoredReading(srh);
    //   setIfDif (srh, senvals[srh] , old, 1, 100, 0);
    }else if(strcmp(SE.stype[i].model, "DHT11")==0){
      int srt = SE.stype[i].ids[0];
      int srh = SE.stype[i].ids[1];      
      senvals[srt] = (int)dht.readTemperature(true);
      senvals[srh] = (int)dht.readHumidity();
      old = req.getStoredReading(srt);
      setIfDif (srt, senvals[srt] , old, 1, 120, -20);
      // printf("DHTll temp sensro:%d, reading:%d \n", srt, senvals[srt]);
      old = req.getStoredReading(srh);
      setIfDif (srh, senvals[srh] , old, 3, 100, 0);
      // printf("DHTll humidity sensro:%d, reading:%d \n", srh, senvals[srh]);
    // }else if(strcmp(SE.stype[i].model, "ANALOG")==0){
    //   sr = SE.stype[i].ids[0];
    //   senvals[sr] = map(constrain(analogRead(inpo.ANNALOG),460,1023),463,1023,100,0);
    //   old = req.getStoredReading(sr);
    //   setIfDif (sr, senvals[sr] , old, 2, 101, -1);
    //   // printf("Soil sensro:%d, reading:%d %\n", sr, senvals[sr]);
    // }else if(strcmp(SE.stype[i].model, "MAX31855")==0){
    //     sr = SE.stype[i].ids[0];
    //     double ftemp =tc.readInternal();
    //     Serial.println(ftemp);
    //     senvals[sr] = (int)ftemp;
    //     old = req.getStoredReading(sr);
    //     setIfDif (sr, senvals[sr] , old, 1, 100, 0);
    // }else if(strcmp(SE.stype[i].model, "inp1")==0){
    //   sr = SE.stype[i].ids[0];
    //   old = req.getStoredReading(sr);
    //   int digi = digitalRead(D1);
    //   setIfDif (sr, digi , old, 1, 100, -100);
    // }else if(strcmp(SE.stype[i].model, "inp2")==0){
    //   sr = SE.stype[i].ids[0];
    //   old = req.getStoredReading(sr);
    //   int digi = digitalRead(D2);
    //   setIfDif (sr, digi , old, 1, 100, -100);
    }
  }
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
        // Serial.println("hay NEW_MAIL");
        req.processInc();
        NEW_MAIL=0;
    }
  }
  
  if (inow - lcksens > every2sec) {
    lcksens = inow;
    readSensors();
    if(f.HAYsTATEcNG>0){
        if(f.cONNectd) req.pubState(f.HAYsTATEcNG, client);
        f.HAYsTATEcNG=0;
    }
    sched2.ckRelays();
    sched2.updTimers();
    // if (f.cONNectd){
    //   req.pubTimr();
    // }
    customLoop();
    // diffCtrl();  
    if (f.CKaLARM>0){
      sched2.ckAlarms(); //whatever gets scheduled should publish its update
      if (f.cONNectd){
        req.pubPrg(f.CKaLARM);
      }
      f.CKaLARM=f.CKaLARM & 0; //11110 turnoff CKaLARM for 1
    }
  }    
} 
