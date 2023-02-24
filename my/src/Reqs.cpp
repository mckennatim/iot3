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

Reqs::Reqs(char* devid, PubSubClient& client ){
  cdevid = devid;
  cclient = client;
}

bool isNewRec (bool rec, bool isnew){
  return(rec && isnew) ? true : false;
}

void Reqs::processInc(){
  for (int i=0;i<TPC.numtopics;i++){
    if(strcmp(TPC.scribedTo[i], itopic)==0){
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
          deseriCmd();
          Serial.println(ipayload);
          break;   
        case 2://in prg
          Serial.println(ipayload);
          // sched.deseriProg(ipayload);
          break;         
        case 3://in req
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

void Reqs::deseriCmd(){
  Serial.println(ipayload);
  DynamicJsonDocument rot(1000);
  deserializeJson(rot, ipayload);  
  int id = rot["id"];
  Serial.print("id = ");
  Serial.println(id);
  JsonArray sra = rot["sra"]; 
  iscsidx_t ici = getTypeIdx(id); 
  int bit =pow(2,id);
  switch(ici.srtype){
  case 0://se
    //cannot change a se reading from afar
    break;
  case 1://csCYURD128/cmd {"id":3,"sra":[70,68]}
    srs.cs[ici.idx].hi = sra[0];
    srs.cs[ici.idx].lo = sra[1];
    sched.adjRelay(id, srs.cs[ici.idx]);
    ports.port[ici.idx].isnew=1;
    // srs.cs[ici.idx].isnew=1;
    f.HAYsTATEcNG=f.HAYsTATEcNG | bit; 
    break;
  case 2://rel CYURD128/cmd {"id":3,"sra":[1]}
    if(sra[0] != digitalRead(ports.port[id].out)){
      srs.rel[ici.idx].onoff = sra[0];
      digitalWrite(ports.port[id].out, sra[0]);
      ports.port[id].isnew=1;
      f.HAYsTATEcNG=f.HAYsTATEcNG | bit;
      printf("srid: %d, port %d is now %d \n",id, ports.port[id].out, digitalRead(ports.port[id].out));
    } 
    break;
  // case 3://di
  //   srs.di[ici.idx].don = sra[0];
  //   srs.di[ici.idx].doff = sra[1];
  //   srs.rel[ici.idx].isnew=1;    
  //   f.HAYsTATEcNG=f.HAYsTATEcNG | bit;
  //   break;  
  default:
    Serial.println("in desirCmd default");
    break;
  }  
}


// void Reqs::deseriCmdOrig(){
//   Serial.println(ipayload);
//   StaticJsonBuffer<1000> jsonBuffer;
//   JsonObject& rot = jsonBuffer.parseObject(ipayload);
//   int id = rot["id"];
//   Serial.print("id = ");
//   Serial.println(id);
//   JsonArray& sra = rot["sra"]; 
//   iscsidx_t ici = getTypeIdx(id); 
//   int bit =pow(2,id);
//   switch(ici.srtype){
//   case 0://se
//     //cannot change a se reading from afar
//     break;
//   case 1://cs
//     srs.cs[ici.idx].hilimit = sra[0];
//     srs.cs[ici.idx].lolimit = sra[1];
//     sched.adjRelay(id, srs.cs[ici.idx]);
//     srs.cs[ici.idx].isnew=1;
//     f.HAYsTATEcNG=f.HAYsTATEcNG | bit; 
//     break;
//   case 2://ti
//     srs.ti[ici.idx].onoff = sra[0];
//     srs.ti[ici.idx].isnew=1;    
//     f.HAYsTATEcNG=f.HAYsTATEcNG | bit;
//     break;
//   case 3://di
//     srs.di[ici.idx].don = sra[0];
//     srs.di[ici.idx].doff = sra[1];
//     srs.ti[ici.idx].isnew=1;    
//     f.HAYsTATEcNG=f.HAYsTATEcNG | bit;
//     break;  
//   default:
//     Serial.println("in desirCmd default");
//     break;
//   }  
// }


void Reqs::deseriReq(){
  DynamicJsonDocument rot(300);
  deserializeJson(rot, ipayload);
  int id = rot["id"];  
  switch(id){
   case 0://`{\"id\":0, \"req\":"srstates"}`
    f.HAYsTATEcNG = 1023; //1111111111 (10,1's)
    break;
   case 1://\"id\":1, \"req\":"sched"}
    Serial.println("in desiriReq 1=sched");
    pubPrg(f.HAYpROG);
    break;
  //  case 2://\"id\":2, \"req\":"flags"}
  //   Serial.println("in desiriReq 2=flags");
  //   pubFlags();
  //   break;
  //  case 3://\"id\":2, \"req\":"timr"}
  //   Serial.println("in desiriReq 3=timr");
  //   pubTimr();
  //   break;
   default:
    Serial.println("in desiriReq default");
  }  
}
void Reqs::pubPrg(int hayprg){
  char sched[20];
  strcpy(sched,cdevid);
  strcat(sched,"/sched"); 
  for( int i=0;i<prgs.numprgs;i++){
    int bit =pow(2,prgs.prg[i].sr);
    Serial.print("sr: ");
    Serial.print(prgs.prg[i].sr);
    Serial.print(" is ");
    Serial.println(((bit & hayprg)==bit));
    if((bit & hayprg)==bit){
      prg_t p = prgs.prg[i];
      char astr[200];
      creaJson(p, astr);
      clpub(sched,astr);
    }
  }
}

void Reqs::pubState(int hc, PubSubClient& client){
  Serial.println(hc);
  int sr;
  int bit;
  bool shouldrec = 0;
  char devtopic[20];
  strcpy(devtopic,cdevid);
  strcat(devtopic,"/srstate");  
  char payload[200];
  for (int i=0; i<srs.numse; i++){
    sr = srs.se[i].sr;
    bit =pow(2,sr);
    if((hc & bit)==bit){
      shouldrec = isNewRec(ports.port[sr].rec, ports.port[sr].isnew);
      sprintf(payload, "{\"id\":%d, \"darr\":[%d], \"new\":%d}", sr, srs.se[i].reading, shouldrec);
      printf("Req128{\"id\":%d, \"darr\":[%d], \"new\":%d }\n", sr, srs.se[i].reading, shouldrec);
      ports.port[sr].isnew=0;
      client.publish(devtopic, payload, true);
    }
  }
  for (int i=0; i<srs.numcs; i++){
    sr = srs.cs[i].sr;
    bit =pow(2, sr);
    if((hc & bit)==bit){
      shouldrec = isNewRec(ports.port[sr].rec, ports.port[sr].isnew);
      sprintf(payload, "{\"id\":%d, \"darr\":[%d, %d, %d, %d], \"new\":%d}",sr, srs.cs[i].reading, srs.cs[i].onoff, srs.cs[i].hi, srs.cs[i].lo, shouldrec);  
      printf("Req219:{\"id\":%d, \"darr\":[%d, %d, %d, %d], \"new\":%d}\n",sr, srs.cs[i].reading, srs.cs[i].onoff, srs.cs[i].hi, srs.cs[i].lo, shouldrec);  
      ports.port[sr].isnew=0;
      client.publish(devtopic, payload, true);
    }
  }
    for (int i=0; i<srs.numrel; i++){
    sr = srs.rel[i].sr;
    bit =pow(2,sr);
    if((hc & bit)==bit){
      shouldrec = isNewRec(ports.port[sr].rec, ports.port[sr].isnew);
      sprintf(payload, "{\"id\":%d, \"darr\":[%d], \"new\":%d}", sr, srs.rel[i].onoff, shouldrec);
      printf("Req230{\"id\":%d, \"darr\":[%d], \"new\":%d }\n", sr, srs.rel[i].onoff, shouldrec);
      ports.port[sr].isnew=0;
      client.publish(devtopic, payload, true);
    }
  }
}

void Reqs::clpub(char topic[20], char payload[200]){
  if (cclient.connected()){
    cclient.publish(topic, payload, true);
  }   
  Serial.print(topic);
  Serial.println(payload);
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
  for (int k=0; k<srs.numrel;k++){
    if(srs.rel[k].sr==srid){
      ici.srtype=2;
      ici.idx=k;
    }
  }
  //last one wins overwrites
  // for (int k=0; k<srs.numdi;k++){
  //   if(srs.di[k].sa==srid){
  //     ici.srtype=3;
  //     ici.idx=k;
  //   }
  // }
  return ici;
}

// int Reqs::getStoredReading(int srid){
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

// bool isNewRec (bool rec, bool isnew){
//   return(rec && isnew) ? true : false;
// }

void Reqs::creaJson(prg_t& p, char* astr){
  const int capacity = JSON_OBJECT_SIZE(4);
  StaticJsonDocument<capacity> root;
  root["id"]= p.sr;
  root["aid"] = p.aid;
  root["ev"] = p.ev;
  root["numdata"] = p.numdata;
  serializeJsonPretty(root, Serial);
  JsonArray pro = root.createNestedArray("pro");
  for(int i=0;i<p.ev;i++){
    JsonArray data = pro.createNestedArray();
    for (int j=0;j<p.numdata + 2;j++){
      data.add(p.prg[i][j]);
    }
  }
  char ast[200];
  serializeJson(root, ast);
  Serial.println(ast);
  strcpy(astr,ast);
}

