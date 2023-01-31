#include "Reqs.h"
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <Arduino.h>
#include <TimeLib.h>
#include "ConnWIFI.h" //getOnline() readConfig()devid owner pwd
#include "Sched.h"

// extern Sched sched;
extern char itopic[40];
extern char ipayload[250];

Reqs::Reqs(char* devid, PubSubClient& client ){
  cdevid = devid;
  cclient = client;
}

//{"devtime", "cmd", "prg", "req", "set"}
void Reqs::processInc(){
  for (int i=0;i<TPC.numtopics;i++){
    if(strcmp(TPC.scribedTo[i], itopic)==0){
    	switch (i){
        case 0:
          Serial.println("case 0 is what devtime?");
          // sched.deseriTime();
          // sched.actTime();
          f.aUTOMA = 1;  
				  f.CKaLARM = 1023; 
				  f.HAYsTATEcNG =1023;          
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

void Reqs::deseriCmd(){
  Serial.println(ipayload);
  StaticJsonDocument<1000> jsonBuffer;
  // JsonDocument rot = deserializeJson(jsonBuffer,ipayload);
  // int id = rot["id"];
  // Serial.print("id = ");
  // Serial.println(id);
  // JsonArray& sra = rot["sra"]; 
  // iscsidx_t ici = getTypeIdx(id); 
  // int bit =pow(2,id);
  // switch(ici.srtype){
  // case 0://se
  //   //cannot change a se reading from afar
  //   break;
  // case 1://cs
  //   srs.cs[ici.idx].hilimit = sra[0];
  //   srs.cs[ici.idx].lolimit = sra[1];
  //   sched.adjRelay(id, srs.cs[ici.idx]);
  //   srs.cs[ici.idx].isnew=1;
  //   f.HAYsTATEcNG=f.HAYsTATEcNG | bit; 
  //   break;
  // case 2://ti
  //   srs.ti[ici.idx].onoff = sra[0];
  //   srs.ti[ici.idx].isnew=1;    
  //   f.HAYsTATEcNG=f.HAYsTATEcNG | bit;
  //   break;
  // case 3://di
  //   srs.di[ici.idx].don = sra[0];
  //   srs.di[ici.idx].doff = sra[1];
  //   srs.ti[ici.idx].isnew=1;    
  //   f.HAYsTATEcNG=f.HAYsTATEcNG | bit;
  //   break;  
  // default:
  //   Serial.println("in desirCmd default");
  //   break;
  // }  
  
}

void Reqs::deseriReq(){
  StaticJsonDocument<300> jsonBuffer;
  // JsonObject& rot = jsonBuffer.parseObject(ipayload);
  // int id = rot["id"];  
  // switch(id){
  //  case 0://`{\"id\":0, \"req\":"srstates"}`
  //   f.HAYsTATEcNG = 1023; //1111111111 (10,1's)
  //   break;
  //  case 1://\"id\":1, \"req\":"sched"}
  //   Serial.println("in desiriReq 1=sched");
  //   pubPrg(f.HAYpROG);
  //   break;
  //  case 2://\"id\":2, \"req\":"flags"}
  //   Serial.println("in desiriReq 2=flags");
  //   pubFlags();
  //   break;
  //  case 3://\"id\":2, \"req\":"timr"}
  //   Serial.println("in desiriReq 3=timr");
  //   pubTimr();
  //   break;
  //  default:
  //   Serial.println("in desiriReq default");
  // }  
}

// void Reqs::clpub(char topic[20], char payload[200]){
//   if (cclient.connected()){
//     cclient.publish(topic, payload, true);
//   }   
//   Serial.print(topic);
//   Serial.println(payload);
// }

// iscsidx_t Reqs::getTypeIdx(int srid){
//   iscsidx_t ici;
//   for (int i=0; i<srs.numse;i++){
//     if(srs.se[i].sr==srid){
//       ici.srtype=0;
//       ici.idx=i;
//     }
//   }
//   for (int j=0; j<srs.numcs;j++){
//     if(srs.cs[j].sr==srid){
//       ici.srtype=1;
//       ici.idx=j;
//     }
//   }
//   for (int k=0; k<srs.numti;k++){
//     if(srs.ti[k].sr==srid){
//       ici.srtype=2;
//       ici.idx=k;
//     }
//   }
//   //last one wins overwrites
//   for (int k=0; k<srs.numdi;k++){
//     if(srs.di[k].sa==srid){
//       ici.srtype=3;
//       ici.idx=k;
//     }
//   }
//   return ici;
// }

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

// void Reqs::creaJson(prg_t& p, char* astr){
//   StaticJsonDocument<2000> jsonBuffer;
//   JsonObject& root = jsonBuffer.createObject();
//   root["id"]= p.sr;
//   root["aid"] = p.aid;
//   root["ev"] = p.ev;
//   root["numdata"] = p.numdata;
//   JsonArray& pro = root.createNestedArray("pro");
//   for(int i=0;i<p.ev;i++){
//     JsonArray& data = pro.createNestedArray();
//     for (int j=0;j<p.numdata + 2;j++){
//       data.add(p.prg[i][j]);
//     }
//   }
//   char ast[200];
//   root.printTo(ast, sizeof(ast));
//   Serial.println(ast);
//   strcpy(astr,ast);
// }

// void Reqs::pubPrg(int hayprg){
//   char sched[20];
//   strcpy(sched,cdevid);
//   strcat(sched,"/sched"); 
//   for( int i=0;i<prgs.numprgs;i++){
//     int bit =pow(2,prgs.prg[i].sr);
//     Serial.print("sr: ");
//     Serial.print(prgs.prg[i].sr);
//     Serial.print(" is ");
//     Serial.println(((bit & hayprg)==bit));
//     if((bit & hayprg)==bit){
//       prg_t p = prgs.prg[i];
//       char astr[200];
//       creaJson(p, astr);
//       clpub(sched,astr);
//     }
//   }
// }

// void Reqs::pubState(int hc){
//   Serial.println(hc);
//   char devtopic[20];
//   strcpy(devtopic,cdevid);
//   strcat(devtopic,"/srstate");  
//   char payload[200];
//   bool shouldrec = 0;
//   for (int i=0; i<srs.numse; i++){
//     shouldrec = isNewRec(srs.se[i].rec, srs.se[i].isnew);
//     sprintf(payload, "{\"id\":%d, \"darr\":[%d], \"new\":%d}", srs.se[i].sr, srs.se[i].reading, shouldrec);
//     srs.se[i].isnew=0;
//     int bit =pow(2,srs.se[i].sr);
//     if((hc&bit)==bit){
//       clpub(devtopic, payload);
//     }
//   }
//   for (int i=0; i<srs.numcs; i++){
//     shouldrec = isNewRec(srs.cs[i].rec, srs.cs[i].isnew);
//     // printf("i = %d srs.cs[i].hilimit = %d \n",i,srs.cs[i].hilimit);
//     sprintf(payload, "{\"id\":%d, \"darr\":[%d, %d, %d, %d], \"new\":%d}", srs.cs[i].sr, srs.cs[i].reading, srs.cs[i].onoff, srs.cs[i].hilimit, srs.cs[i].lolimit, shouldrec);  
//     srs.cs[i].isnew=0; 
//     // int bit =pow(2,i);
//     // if((hc&bit)==bit){
//       clpub(devtopic, payload);
//     // } 
//   }
//   for (int i=0; i<srs.numti; i++){
//     shouldrec = isNewRec(srs.ti[i].rec, srs.ti[i].isnew);
//     sprintf(payload, "{\"id\":%d, \"darr\":[%d], \"new\":%d}", srs.ti[i].sr, srs.ti[i].onoff, shouldrec);
//     srs.ti[i].isnew=0;
//     int bit =pow(2,i);
//     if((hc&bit)==bit){
//       clpub(devtopic, payload);
//     }
//   }
//   for (int i=0; i<srs.numdi; i++){
//     shouldrec = isNewRec(srs.di[i].rec, srs.di[i].isnew);
//     int saidx = srs.di[i].sa;
//     int sbidx = srs.di[i].sb;
//     int don = srs.di[i].don;
//     int doff = srs.di[i].doff;
//     int onoff = srs.di[i].onoff;
//     int saread = getStoredReading(saidx);
//     int sbread = getStoredReading(sbidx);
//     sprintf(payload, "{\"sa\":%d, \"sb\":%d, \"darr\":[%d,%d,%d,%d,%d], \"new\":%d}", saidx, sbidx, saread, sbread, don, doff, onoff,shouldrec);  
//     srs.di[i].isnew=0; 
//     // int bit =pow(2,i);
//     // if((hc&bit)==bit){
//       clpub(devtopic, payload);
//     // } 
//   }
// }

// void Reqs::pubStateOld(int hc){
//   Serial.println(hc);
//   char devtopic[20];
//   strcpy(devtopic,cdevid);
//   strcat(devtopic,"/srstate");  
//   char payload[200];
//   bool shouldrec = 0;
//   for( int i=0; i<srs.numsr; i++){
//     int bit =pow(2,i);
//     if((hc&bit)==bit){
//       iscsidx_t ici = getTypeIdx(i);
//       if (ici.srtype==0){//se
//         shouldrec = isNewRec(srs.se[ici.idx].rec, srs.se[ici.idx].isnew);
//         sprintf(payload, "{\"id\":%d, \"darr\":[%d], \"new\":%d}", srs.se[ici.idx].sr, srs.se[ici.idx].reading, shouldrec);
//         srs.se[ici.idx].isnew=0;
//       }else if (ici.srtype==1){//cs
//         shouldrec = isNewRec(srs.cs[ici.idx].rec, srs.cs[ici.idx].isnew);
//         sprintf(payload, "{\"id\":%d, \"darr\":[%d, %d, %d, %d], \"new\":%d}", srs.cs[ici.idx].sr, srs.cs[ici.idx].reading, srs.cs[ici.idx].onoff, srs.cs[ici.idx].hilimit, srs.cs[ici.idx].lolimit, shouldrec);  
//         srs.cs[ici.idx].isnew=0;    
//       }else{//ti
//         shouldrec = isNewRec(srs.ti[ici.idx].rec, srs.ti[ici.idx].isnew);
//         sprintf(payload, "{\"id\":%d, \"darr\":[%d], \"new\":%d}", srs.ti[ici.idx].sr, srs.ti[ici.idx].onoff, shouldrec);
//         srs.ti[ici.idx].isnew=0;
//       }
//       clpub(devtopic, payload);
//     }
//   }
// }

// void Reqs::pubTimr(){
//   char devtopic[20];
//   strcpy(devtopic,cdevid);
//   strcat(devtopic,"/timr"); 
//   StaticJsonDocument<500> jsonBuffer;
//   JsonObject& root = jsonBuffer.createObject();
//   root["cREMENT"]=f.cREMENT;
//   root["IStIMERoN"]=f.IStIMERoN;//11100 assume some time left, timers with tleft>0 
//   root["ISrELAYoN"]=f.ISrELAYoN;// = summary of relay states  
//   JsonArray& tleft = root.createNestedArray("tIMElEFT");
//   for(int i=0;i<sizeOf(f.tIMElEFT);i++){
//     tleft.add(f.tIMElEFT[i]);
//   }
//   char payload[180];
//   root.printTo(payload, sizeof(payload));
//   clpub(devtopic,payload);  
// }

// void Reqs::pubFlags(){
//   char flags[20];
//   strcpy(flags,cdevid);
//   strcat(flags,"/flags"); 
//   StaticJsonDocument<500> jsonBuffer;
//   JsonObject& root = jsonBuffer.createObject();
//   root["aUTOMA"]=f.aUTOMA;
//   root["fORCErESET"]=f.fORCErESET;  
//   root["cREMENT"]=f.cREMENT;
//   root["HAStIMR"]=f.HAStIMR; //11100(28) 4,8, and 16 have timers not temp
//   root["IStIMERoN"]=f.IStIMERoN;//11100 assume some time left, timers with tleft>0 
//   root["HAYpROG"]=f.HAYpROG;// = senrels with events>1
//   root["HAYsTATEcNG"]=f.HAYsTATEcNG; //11111(31 force report)state ch root["or ext
//   root["CKaLARM"]=f.CKaLARM; //11111 assume alarm is set at start
//   root["ISrELAYoN"]=f.ISrELAYoN;// = summary of relay states  
//   JsonArray& tleft = root.createNestedArray("tIMElEFT");
//   for(int i=0;i<10;i++){
//     tleft.add(f.tIMElEFT[i]);
//   }
//   char ast[180];
//   root.printTo(ast, sizeof(ast));
//   clpub(flags,ast);  
// }