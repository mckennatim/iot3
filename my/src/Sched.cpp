#include "Sched.h"
#include <ArduinoJson.h>
#include <TimeLib.h>
#include "CONFIG.h"
#include "Reqs.h"

extern Reqs req;
extern char ipayload[250];

void Sched::deseriTime(){
  DynamicJsonDocument root(200);
  deserializeJson(root, ipayload);
  serializeJsonPretty(root, Serial);
  unix = root["unix"];
  LLLL = root["LLLL"];
  zone = root["zone"];
}

void Sched::actTime(){
  time_t datetime = unix + zone*60*60;
  setTime(datetime);
  setSyncInterval(4000000); 
  printf("%d:%d \n", hour(), minute() );
}

void Sched::adjRelay(int sr, cs_t& te){
  int bit =pow(2,sr);
  int mask = 1023-bit; 
  bool relayState = te.onoff;
  if (te.reading >= te.hi){
    relayState=0;
  } else if (te.reading <= te.lo){
    relayState=1;
  }
  if (relayState != te.onoff){
    te.onoff = relayState;
    ports.port[sr].isnew=1;  
    // te.isnew=1;
    int relayon = f.ISrELAYoN;
    if(te.onoff){
      relayon = relayon | bit;
    }else{
      relayon = relayon & mask;
    }
    if(relayon!=f.ISrELAYoN){
      f.ISrELAYoN = relayon;
      // req.pubTimr();
    }
    digitalWrite(ports.port[sr].out, relayState);
    // for (int i=0;i<prgs.numprgs;i++){
    //   if(prgs.prg[i].sr==sr){
    //     digitalWrite(prgs.prg[i].port, relayState); 
    //   }
    // }
    printf("sr: %d, port: %d, relayState: %d \n",sr, ports.port[sr].out, digitalRead(ports.port[sr].out));
  }    
  // printf("relayState = %d", relayState);
}

void Sched::ckRelays(){
  for (int i=0; i<ports.numports;i++){
    if (ports.port[i].out>=0){
      iscsidx_t ici = req.getTypeIdx(ports.port[i].sr);
      switch(ici.srtype){
        case 0:
          //cannot change a se reading from afar
          break;
        case 1:
          if(srs.cs[ici.idx].onoff != digitalRead(ports.port[i].sr)){
            digitalWrite(ports.port[i].out, srs.cs[ici.idx].onoff);
          }
          break;
        case 2:
          if(srs.rel[ici.idx].onoff != digitalRead(ports.port[i].out)){
            digitalWrite(ports.port[i].out, srs.rel[ici.idx].onoff);
          }
          break;
        default:
          Serial.println("in desirCmd default");
          break;
      }   
    }
  }
}

// void Sched::ckRelays2(){
//   for (int i=0;i<prgs.numprgs;i++){
//     iscsidx_t ici = req.getTypeIdx(prgs.prg[i].sr);
//     switch(ici.srtype){
//       case 0:
//         //cannot change a se reading from afar
//         break;
//       case 1:
//         if(srs.cs[i].onoff != digitalRead(prgs.prg[i].port)){
//           digitalWrite(prgs.prg[i].port, srs.cs[i].onoff);
//         }
//         break;
//       case 2:
//         if(srs.rel[ici.idx].onoff != digitalRead(prgs.prg[i].port)){
//           digitalWrite(prgs.prg[i].port, srs.rel[ici.idx].onoff);
//         }
//         break;
//       default:
//         Serial.println("in desirCmd default");
//         break;
//     } 
//   }  
// }



void Sched::copyProg(prg_t& t, JsonArray ev){
  t.ev=ev.size();
  for(size_t h=0;h<ev.size();h++){
    JsonArray aprg = ev[h];
    for(int j=0;j<t.numdata+2;j++){
      t.prg[h][j] = aprg[j];
    }
  }
}

void Sched::deseriProg(char* kstr){
  Serial.println(kstr);
  DynamicJsonDocument rot(1000);
  deserializeJson(rot, kstr);  
  int id = rot["id"];
  Serial.print("id = ");
  Serial.println(id);
  JsonArray events = rot["pro"];
  for(int i=0; i<prgs.numprgs;i++){ 
    if(id==prgs.prg[i].sr){
      int bit =pow(2,id);
      copyProg(prgs.prg[i], events);
      f.CKaLARM=f.CKaLARM | bit;      
    }
  }
}    

void Sched::setCur(prg_t& p, int &cur, int &nxt){
  Serial.print("ev(size)=");
  Serial.println(p.ev);
  Serial.print(hour());
  Serial.print(":");
  Serial.println(minute());
  for(int j=0; j<p.ev;j++){
    if (hour() == p.prg[j][0]){
      if (minute() < p.prg[j][1]){
        cur = j-1;
        break;
      }
    }
    if (hour() < p.prg[j][0]){
      cur= j-1;
      break;
    }
    cur =j;
  }
  nxt = cur+1;
  if (nxt>=p.ev){
    nxt=0;
  }        
}

void Sched::ckAlarms(){
  for (int i=0;i<prgs.numprgs;i++){
    int sr =prgs.prg[i].sr;
    int bit = pow(2,sr);
    int mask = 1023-bit;
    int cur, nxt;
    //int asec;
    prg_t *p = &prgs.prg[i];
    setCur(*p, cur, nxt);
    Alarm.free(p->aid);
    iscsidx_t ici = req.getTypeIdx(sr);
    switch(ici.srtype){
      case 0:{
        //no timers on uncontrolled sensors
        Serial.println("cat");
      }
        break;
      case 1:{
        cs_t *cs = &srs.cs[i];
        cs->hi = p->prg[cur][2];
        cs->lo = p->prg[cur][3];
        adjRelay(sr,*cs);
        int hr = p->prg[nxt][0];
        int min = p->prg[nxt][1];
        // int asec = second()+sr;        
        p->aid = Alarm.alarmOnce(hr,min,sr,alarmRings);
        p->hms=hr*60+min;
        Serial.print("sr: ");
        Serial.print(p->sr);
        Serial.print(" aid: ");
        Serial.print(p->aid);
        Serial.print(" hr: ");
        Serial.print(hr);
        Serial.print(" min: ");
        Serial.println(min);
        sortPrgsHms(prgs.prg, prgs.numprgs);
        showArray(prgs.prg, prgs.numprgs);
        f.HAYsTATEcNG=f.HAYsTATEcNG | bit;
      }
        break;
      case 2:{
        port_t *prt = &ports.port[sr];
        rel_t *rel = &srs.rel[ici.idx];
        rel->onoff = p->prg[cur][2];
        int tleft=0;//initialize
        if (rel->onoff){ //if relay is on
          prt->isnew=1;
          f.ISrELAYoN = f.ISrELAYoN | bit;
          if (nxt != cur){ //don't countdown allday[[0,0,1]]
            getTleft(*p, cur, nxt, tleft);
            f.IStIMERoN = f.IStIMERoN | bit; //on here, shut off in deductCrement
          }else{ //shutoff timer if program only is a single ev, hold[[0,0,1]]
            f.IStIMERoN = f.IStIMERoN & mask;
          }
        }else {
          prt->isnew=0;
          f.ISrELAYoN = f.ISrELAYoN & mask;
          f.IStIMERoN = f.IStIMERoN & mask;
        }    
        f.tIMElEFT[sr]=tleft*60;
        f.HAYsTATEcNG=f.HAYsTATEcNG | bit;
        int hr = p->prg[nxt][0];
        int min = p->prg[nxt][1];
        p->aid = Alarm.alarmOnce(hr,min,sr, alarmRings);
        p->hms=hr*60+min;
        Serial.print("sr: ");
        Serial.print(p->sr);
        Serial.print(" aid: ");
        Serial.print(p->aid);
        Serial.print(" hr: ");
        Serial.print(hr);
        Serial.print(" min: ");
        Serial.println(min);
        sortPrgsHms(prgs.prg, prgs.numprgs);
        showArray(prgs.prg, prgs.numprgs);
      }
        break;
      default:
        Serial.println("in ckAlarmdefault");
        break;
    } 
  }  
}

void Sched::getTleft(prg_t p, int cur, int nxt, int &tleft){
  int hr = hour();
  int min = minute(); 
  if(nxt==0){
    tleft = (23-hr)*60+(59-min) +1;
  }else{
    int nxthr = p.prg[nxt][0];
    int nxtmin = p.prg[nxt][1];
    if(nxtmin < min){//12:25 -> 14:05
      nxtmin=nxtmin+60;
      nxthr--;
    }
    tleft= (nxthr-hr)*60 + (nxtmin - min);
  }
}

void Sched::sortPrgsHms(prg_t prg[], int size){
  bool swapped;
  do{
    swapped=false;
    for (int i=0;i<size -1 ;i++){
      if(prg[i].hms > prg[i+1].hms ){
        prg_t temp = prg[i];
        prg[i] = prg[i+1];
        prg[i+1] = temp;
        swapped = true;
      }
    }    
  }while(swapped);
}

void alarmRings(){//alarm went off
  Serial.println("RING RING RING");
  int sr = prgs.prg[0].sr; //for this sr
  int bit = pow(2,sr);
  f.CKaLARM=f.CKaLARM | bit; //so ck and reset next sched section
}

void Sched::showArray(const prg_t prg[], int size){
  for (int i=0;i<size;i++){
    Serial.print(prg[i].sr);
    Serial.print(" ");
    Serial.println(prg[i].hms);
  }
}

void Sched::deductCrement(int id){
  int mask = 1023 - pow(2,id);
  int t = f.tIMElEFT[id];
  t = t - f.cREMENT;
  if(t<=0){
    t=0;
    f.IStIMERoN = f.IStIMERoN & mask; //11011
    /*tsec code for turning off srs.rel[x].onoff when times up*/
    iscsidx_t ici = req.getTypeIdx(id);
    int bit = pow(2,id);
    if((bit & f.HAStIMR)==bit){
      srs.rel[ici.idx].onoff = 0;
      int mask = 1023-bit;
      f.HAStIMR = f.HAStIMR & mask;
    }
    /*tsec code end */
  }
  f.tIMElEFT[id] = t;
  // printf("deduct: id:%d, timeleft:%d \n",id,t);
}

void Sched::updTimers(){
  bool pub = false;
  for(unsigned int i=0;i<sizeOf(f.tIMElEFT);i++){
    if(f.tIMElEFT[i]>0){
      deductCrement(i);
      pub=true;
    }
  }
  if(pub && f.cONNectd){
    req.pubTimr();
  }
}