#include "Reqs.h"
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <Arduino.h>
#include <TimeLib.h>
#include "ConnWIFI.h" //getOnline() readConfig()devid owner pwd
#include "Sched.h"

Sched sched;
extern char itopic[40];
extern char ipayload[250];


// int getStoredReading2(int srid){
//   for (int i=0; i<srs.numse;i++){
//     if(srs.se[i].sr==srid){
//       return srs.se[i].reading;
//     }
//   }
//   for (int j=0; j<srs.numcs;j++){
//     if(srs.cs[j].sr==srid){
//       return srs.cs[j].reading;
//     }
//   }
//   return -4;
// }

bool isNewRec (bool rec, bool isnew){
  return(rec && isnew) ? true : false;
}


Reqs::Reqs(char* devid){
  cdevid = devid;
}

void Reqs::processInc(){
  for (int i=0;i<tpcsSub.numtopics;i++){
    if(strcmp(tpcsSub.scribedTo[i], itopic)==0){
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

void Reqs::deseriCmd(){
  Serial.println(ipayload);
  // StaticJsonBuffer<1000> jsonBuffer;
  // JsonObject& rot = jsonBuffer.parseObject(ipayload);
  DynamicJsonDocument rot(1000);
  deserializeJson(rot, ipayload);
  serializeJsonPretty(rot, Serial);
  int id = rot["id"];
  Serial.print("id = ");
  Serial.println(id);
  JsonArray& sra = rot["sra"]; 
  iscsidx_t ici = getTypeIdx(id); 
  int bit =pow(2,id);
  switch(ici.srtype){
  case 0://se
    //cannot change a se reading from afar
    break;
  case 1://cs
    srs.cs[ici.idx].hilimit = sra[0];
    srs.cs[ici.idx].lolimit = sra[1];
    // sched.adjRelay(id, srs.cs[ici.idx]);
    srs.cs[ici.idx].isnew=1;
    f.HAYsTATEcNG=f.HAYsTATEcNG | bit; 
    break;
  case 2://ti
    srs.ti[ici.idx].onoff = sra[0];
    srs.ti[ici.idx].isnew=1;    
    f.HAYsTATEcNG=f.HAYsTATEcNG | bit;
    break;
  case 3://di
    srs.di[ici.idx].don = sra[0];
    srs.di[ici.idx].doff = sra[1];
    srs.ti[ici.idx].isnew=1;    
    f.HAYsTATEcNG=f.HAYsTATEcNG | bit;
    break;  
  default:
    Serial.println("in desirCmd default");
    break;
  }   
}

iscsidx_t Reqs::getTypeIdx(int srid){
  iscsidx_t ici;
  for (int i=0; i<srs.numse;i++){
    if(srs.se[i].sr==srid){
      ici.srtype=0;
      ici.idx=i;
    }
  }
  for (int j=0; j<srs.numcs;j++){
    if(srs.cs[j].sr==srid){
      ici.srtype=1;
      ici.idx=j;
    }
  }
  for (int k=0; k<srs.numti;k++){
    if(srs.ti[k].sr==srid){
      ici.srtype=2;
      ici.idx=k;
    }
  }
  //last one wins overwrites
  for (int k=0; k<srs.numdi;k++){
    if(srs.di[k].sa==srid){
      ici.srtype=3;
      ici.idx=k;
    }
  }
  return ici;
}

int Reqs::getStoredReading(int srid){
  for (int i=0; i<srs.numse;i++){
    if(srs.se[i].sr==srid){
      return srs.se[i].reading;
    }
  }
  for (int j=0; j<srs.numcs;j++){
    if(srs.cs[j].sr==srid){
      return srs.cs[j].reading;
    }
  }
  return -4;
}

void Reqs::pubState(int hc, PubSubClient& client){
  Serial.println(hc);
  char devtopic[20];
  strcpy(devtopic,cdevid);
  strcat(devtopic,"/srstate");  
  char payload[200];
  bool shouldrec = 0;
  for (int i=0; i<srs.numse; i++){
    shouldrec = isNewRec(srs.se[i].rec, srs.se[i].isnew);
    sprintf(payload, "{\"id\":%d, \"darr\":[%d], \"new\":%d}", srs.se[i].sr, srs.se[i].reading, shouldrec);
    srs.se[i].isnew=0;
    int bit =pow(2,srs.se[i].sr);
    if((hc&bit)==bit){
      client.publish(devtopic, payload, true);
    }
  }
  for (int i=0; i<srs.numcs; i++){
    shouldrec = isNewRec(srs.cs[i].rec, srs.cs[i].isnew);
    // printf("i = %d srs.cs[i].hilimit = %d \n",i,srs.cs[i].hilimit);
    sprintf(payload, "{\"id\":%d, \"darr\":[%d, %d, %d, %d], \"new\":%d}", srs.cs[i].sr, srs.cs[i].reading, srs.cs[i].onoff, srs.cs[i].hilimit, srs.cs[i].lolimit, shouldrec);  
    srs.cs[i].isnew=0; 
    // int bit =pow(2,i);
    // if((hc&bit)==bit){
      client.publish(devtopic, payload, true);
    // } 
  }
  for (int i=0; i<srs.numti; i++){
    shouldrec = isNewRec(srs.ti[i].rec, srs.ti[i].isnew);
    sprintf(payload, "{\"id\":%d, \"darr\":[%d], \"new\":%d}", srs.ti[i].sr, srs.ti[i].onoff, shouldrec);
    srs.ti[i].isnew=0;
    int bit =pow(2,i);
    if((hc&bit)==bit){
      client.publish(devtopic, payload, true);
    }
  }
  for (int i=0; i<srs.numdi; i++){
    shouldrec = isNewRec(srs.di[i].rec, srs.di[i].isnew);
    int saidx = srs.di[i].sa;
    int sbidx = srs.di[i].sb;
    int don = srs.di[i].don;
    int doff = srs.di[i].doff;
    int onoff = srs.di[i].onoff;
    int saread = getStoredReading(saidx);
    int sbread = getStoredReading(sbidx);
    sprintf(payload, "{\"sa\":%d, \"sb\":%d, \"darr\":[%d,%d,%d,%d,%d], \"new\":%d}", saidx, sbidx, saread, sbread, don, doff, onoff,shouldrec);  
    srs.di[i].isnew=0; 
    client.publish(devtopic, payload, true);
  }
}