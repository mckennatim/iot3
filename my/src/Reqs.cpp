#include "Reqs.h"
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <Arduino.h>
#include <TimeLib.h>
#include "ConnWIFI.h" //getOnline() readConfig()devid owner pwd
#include "Sched.h"
#include "Util.h"
#include "storage.h"

// Sched sched;
extern char itopic[40];
extern char ipayload[250];


void q_processInc(){
  for (int i=0;i<NUMTOPICS;i++){
    if(strcmp(subTopics[i], itopic)==0){
    	switch (i){
        case 0:
          Serial.println("case 0 is what devtime?");
          printf("ipayload %s\n ", ipayload);
          deseriTime();
          f.aUTOMA = 1;  
				  f.CKaLARM = 1023; //1111111111
				  // f.HAYsTATEcNG =1023
          ; //1111111111         
          break;
        case 1://in cmd
          deseriCmd();
          Serial.println(ipayload);
          break;   
        case 2://in prg
          Serial.println(ipayload);
          deseriProg();
          break;         
        case 3://in req
          Serial.println("DOGFOOD");
          Serial.println(ipayload);
          deseriReq();
          break;          
        default: 
        	Serial.print(i);         
          Serial.println(": in processInc default");
          break;           
      }
    }
  }
}

void deseriTime(){
  DynamicJsonDocument root(200);
  deserializeJson(root, ipayload);
  serializeJsonPretty(root, Serial);
  time_t unix = root["unix"];
  const char* LLLL = root["LLLL"];
  int zone = root["zone"];
  time_t datetime = unix + zone*60*60;
  setTime(datetime);
  setSyncInterval(4000000); 
  printf("%d:%d %s\n", hour(), minute(), LLLL );  
}

void deseriCmd(){
  Serial.println(ipayload);
  DynamicJsonDocument rot(1000);
  deserializeJson(rot, ipayload);  
  int sr = rot["id"];
  JsonArray sra = rot["sra"]; 
  int dax = cmds[sr].nda; 
  for(int i=0;i<dax;i++){ 
    int idx = cmds[sr].data[i]; //srs data index for the cmd data
    srs[sr].data[idx]= sra[i];
  }
  char nstr[10];
  snprintf(nstr, sizeof(nstr), "re%d", sr); // "re23"
  Serial.print("nstr = ");
  Serial.println(nstr);  
  setStored(nstr, srs[sr].data[1]);
  Serial.print("nstr = ");
  Serial.println(nstr); 

  u_setFlag(sr, &f.HAYsTATEcNG);
}

void deseriReq(){
  Serial.println(ipayload);
  DynamicJsonDocument rot(1000);
  deserializeJson(rot, ipayload);  
  int sr = rot["id"];
  Serial.print("id = ");
  Serial.println(sr);
  //q_pubState(client);
}


void deseriProg(){
  DynamicJsonDocument rot(1000);
  deserializeJson(rot, ipayload);  
  int id = rot["id"];
  Serial.print("id = ");
  Serial.println(id);
  JsonArray events = rot["pro"];
  int prgx = u_getPrgIdx(id);
  int ev = events.size();
  prgs[prgx].ev = ev;
  for(int i=0;i<ev;i++){
    JsonArray aprg = events[i];
    for(int j=0;j<NPRGDA;j++){
      prgs[prgx].prg[i][j]= aprg[j];
    }
  }
  u_setFlag(id, &f.CKaLARM);
}    

void q_pubState(PubSubClient& client){
  char topic[20];
  strcpy(topic,devid);
  strcat(topic,"/srstate"); 
  for(int sr=0;sr<NUMSR;sr++){
    int sr2 = pow2(sr);
    u_setFlag(sr, &f.dOpUBLISH);
    if((f.dOpUBLISH & sr2) ==sr2 ){
      printf("sr2=%d \n", sr2);
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
      u_unsetFlag(sr, &f.dOpUBLISH);
    }
  }
}

void q_pubPrg(PubSubClient& client){
  int fl = f.CKaLARM;
  char topic[20];
  strcpy(topic,devid);
  strcat(topic,"/sched"); 
  for(int i=0;i<NUMPRGS;i++){
    if(fl & 1){//is LSB set
      int ev = prgs[i].ev;
      StaticJsonDocument<2000> root;
      root["id"]= prgs[i].sr;
      root["aid"] = prgs[i].aid;
      root["ev"] = ev;
      root["numdata"] = 1;
      JsonArray pro = root.createNestedArray("pro");
      for(int k=0;k<ev;k++){
        JsonArray data = pro.createNestedArray();
        for (int j=0;j<3;j++){
          data.add(prgs[i].prg[k][j]);
        }
      }
      char payload[200];
      serializeJson(root, payload);
      if (client.connected()) client.publish(topic, payload, true);
    }
    fl= fl >> 1; //move LSB to left
  }
}