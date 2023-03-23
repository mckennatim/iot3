#include "Reqs.h"
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <Arduino.h>
#include <TimeLib.h>
#include "ConnWIFI.h" //getOnline() readConfig()devid owner pwd
#include "Sched.h"
#include "Util.h"

Sched sched;
extern char itopic[40];
extern char ipayload[250];





void q_processInc(){
  for (int i=0;i<NUMTOPICS;i++){
    if(strcmp(subTopics[i], itopic)==0){
    	switch (i){
        case 0:
          Serial.println("case 0 is what devtime?");
          printf("ipayload %s\n ", ipayload);
          sched.deseriTime();
          sched.actTime();
          f.aUTOMA = 1;  
				  f.CKaLARM = 1023; //1111111111
				  f.HAYsTATEcNG =1023; //1111111111         
          break;
        case 1://in cmd
          // deseriCmd();
          Serial.println(ipayload);
          break;   
        case 2://in prg
          Serial.println(ipayload);
          // sched.deseriProg(ipayload);
          break;         
        case 3://in req
          Serial.println(ipayload);
          // deseriReq();
          break;          
        default: 
        	Serial.print(i);         
          Serial.println(": in processInc default");
          break;           
      }
    }
  }
}

void q_pubState(PubSubClient& client){
  int fl = f.HAYsTATEcNG;
  char topic[20];
  strcpy(topic,devid);
  strcat(topic,"/srstate"); 
  for(int sr=0;sr<NUMSR;sr++){
    if(fl & 1){
      StaticJsonDocument<500> root;
      root["id"]=sr;
      JsonArray darr = root.createNestedArray("darr"); 
      for(int i=0;i<u_getNumdl(sr);i++) {
        darr.add(srs[sr].data[i]);
      } 
      char payload[180];
      serializeJson(root, payload);
      printf("%s\n", payload);
      if (client.connected()) client.publish(topic, payload, true);
    }
    fl= fl >> 1;
  }
}

