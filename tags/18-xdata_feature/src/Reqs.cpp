#include "Reqs.h"
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <Arduino.h>
#include <TimeLib.h>
#include "ConnWIFI.h" //getOnline() readConfig()devid owner pwd
#include "Sched.h"
#include "CONFIG.h"

Sched sched;
extern char itopic[40];
extern char ipayload[250];
extern char idev[9];
extern unsigned int ipaylen;

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
        case 2://in prg CYURD073/prg {"id":0,"pro":[[0,0,61,59],[9,0,62,60],[11,48,61,59],[15,5,62,60],[17,11,61,59]]}
          Serial.println(ipayload);
          sched.deseriProg(ipayload);
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
  getXdata(idev, ipayload);
}

void Reqs::getXdata(char* xdevtpc, char* xpayload){
  char xpayl[100];
	strcpy(xpayl,xpayload);
  for (int i=0; i<=xdata.numdevs;i++){
    if(strcmp(xdevtpc, xdata.xda[i].xdev)==0){
      DynamicJsonDocument rot(1000);
      deserializeJson(rot, xpayload);  
      int id = rot["id"];
      JsonArray darr = rot["darr"];
      srs_t xrs = xdata.xda[i].xrs;
      for (int i=0; i<xrs.numse;i++){
        if(xrs.se[i].sr==id){
          xrs.se[i].reading = darr[0];
          printf("xdevtpc:%s, xpayload:%s \n", xdevtpc, xpayl);
        }
      }
      for (int i=0; i<xrs.numcs;i++){
        if(xrs.cs[i].sr==id){
          xrs.cs[i].reading = darr[0];
          xrs.cs[i].onoff = darr[1];
          xrs.cs[i].hi = darr[2];
          xrs.cs[i].lo = darr[3];
          printf("xdevtpc:%s, xpayl:%s \n", xdevtpc, xpayl);
        }
      }
      for (int i=0; i<xrs.numrel;i++){
        if(xrs.rel[i].sr==id){
          xrs.rel[i].onoff = darr[0];
          printf("xdevtpc:%s, xpayl:%s \n", xdevtpc, xpayl);
        }
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
  int tsec = rot["tsec"];
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
  case 2://rel CYURD128/cmd {"id":3,"sra":[1],"tsec":23}
    printf("tsec:%d \n",tsec);
    if (tsec>0){
      int bit = pow(2,id);
      f.HAStIMR=f.HAStIMR | bit;
      srs.rel[ici.idx].onoff = 1;
      digitalWrite(ports.port[id].out, 1);
      f.tIMElEFT[id]=tsec;
    } 
    else if(sra[0] != digitalRead(ports.port[id].out)){
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
    f.HAYpROG = 1023;
    pubPrg(f.HAYpROG);
    break;
   case 2://\"id\":2, \"req\":"flags"}
    Serial.println("in desiriReq 2=flags");
    pubFlags();
    break;
   case 3://\"id\":2, \"req\":"timr"}
    Serial.println("in desiriReq 3=timr");
    pubTimr();
    break;
   default:
    Serial.println("in desiriReq default");
  }  
}

void Reqs::pubFlags(){
  char flags[20];
  strcpy(flags,cdevid);
  strcat(flags,"/flags"); 
  // StaticJsonBuffer<500> jsonBuffer;
  // JsonObject& root = jsonBuffer.createObject();
  StaticJsonDocument<500> root;
  root["aUTOMA"]=f.aUTOMA;
  root["fORCErESET"]=f.fORCErESET;  
  root["cREMENT"]=f.cREMENT;
  root["HAStIMR"]=f.HAStIMR; //11100(28) 4,8, and 16 have timers not temp
  root["IStIMERoN"]=f.IStIMERoN;//11100 assume some time left, timers with tleft>0 
  root["HAYpROG"]=f.HAYpROG;// = senrels with events>1
  // root["HAYpROGcNG"]=f.HAYpROGcNG;// 11111(31 force report) g change root["or ext
  root["HAYsTATEcNG"]=f.HAYsTATEcNG; //11111(31 force report)state ch root["or ext
  root["CKaLARM"]=f.CKaLARM; //11111 assume alarm is set at start
  root["ISrELAYoN"]=f.ISrELAYoN;// = summary of relay states  
  JsonArray tleft = root.createNestedArray("tIMElEFT");
  for(int i=0;i<5;i++){
    tleft.add(f.tIMElEFT[i]);
  }
  char ast[180];
  serializeJson(root, ast);
  printf("creaJSON313: %s, sizeof(%d) \n",ast, sizeof(ast));
  Serial.println(ast);

  clpub(flags,ast);  
}

void Reqs::pubTimr(){
  char timr[20];
  strcpy(timr,cdevid);
  strcat(timr,"/timr"); 
  // StaticJsonBuffer<500> jsonBuffer;
  // JsonObject& root = jsonBuffer.createObject();
  StaticJsonDocument<500> root;
  root["cREMENT"]=f.cREMENT;
  root["IStIMERoN"]=f.IStIMERoN;//11100 assume some time left, timers with tleft>0 
  root["ISrELAYoN"]=f.ISrELAYoN;// = summary of relay states  
  JsonArray tleft = root.createNestedArray("tIMElEFT");
  for(int i=0;i<5;i++){
    tleft.add(f.tIMElEFT[i]);
  }
  char ast[180];
  serializeJson(root, ast);
  printf("pubTimr: %s \n",ast);
  clpub(timr,ast);  
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
      printf("Req219:%s{\"id\":%d, \"darr\":[%d, %d, %d, %d], \"new\":%d}\n",cdevid,sr, srs.cs[i].reading, srs.cs[i].onoff, srs.cs[i].hi, srs.cs[i].lo, shouldrec);  
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
  printf("req241: %d \n", cclient.connected());
  if (cclient.connected()){
    cclient.publish(topic, payload, true);
  }   
  client.publish(topic, payload, true);
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

int Reqs::getPrgIdx(int srid){
  int idx = -1;
  for (int i=0;i<prgs.numprgs;i++){
    if(srid==prgs.prg[i].sr){
      idx=i;
    }
  }
  return idx;
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
  // const int capacity = JSON_OBJECT_SIZE(4);
  StaticJsonDocument<2000> root;
  root["id"]= p.sr;
  root["aid"] = p.aid;
  root["ev"] = p.ev;
  root["numdata"] = p.numdata;
  JsonArray pro = root.createNestedArray("pro");
  for(int i=0;i<p.ev;i++){
    JsonArray data = pro.createNestedArray();
    for (int j=0;j<p.numdata + 2;j++){
      data.add(p.prg[i][j]);
    }
  }
  char ast[200];
  serializeJson(root, ast);
  printf("creaJSON313: %s, sizeof(%d) \n",ast, sizeof(ast));
  Serial.println(ast);
  strcpy(astr,ast);
}

