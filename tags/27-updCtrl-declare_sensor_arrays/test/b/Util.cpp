#include<stdio.h>
#include <string.h>
#include "CONF.h"
#include "Util.h"
#include <stdlib.h> 
#include <time.h>
#include <math.h>

int getTypIdx(const char* type){
  int ret = -1;
  for (int i =0; i<NUMTYP;i++){
    if(strcmp(tds[i].type, type)==0){ret =i;}
  }
  return ret;
}

int getXdataIdx(int dev, int sr){
  int xd=-1;
  for(int i=0;i<NUMXD;i++){
    if(xdata[i].dev == dev & xdata[i].sr == sr){xd= i;}
  }
  return xd;
}

int whichDev(char* idev){
  int xd=-1;
  for(int i=0;i<NUMXD;i++){
    if(strcmp(idev, xdevtpc[i])==0){xd= i;}
  }
  return xd;
}

void bitShift(int bnum, int LEN, void (*doStuff)(int, int)){
  int b = bnum;
  for(int i=0;i<LEN;i++){
    if(b & 1){
      doStuff(i,b);
    }
    b = b >> 1;
  }
}
void scanFLAGand(int FLAG, int LEN, void (*doStuff)(int, int)){
  int b = FLAG;
  for(int i=0;i<LEN;i++){
    if(b & 1){
      doStuff(i,b);
    }
    b = b >> 1;
  }
}

void doStuff(int i,int b){
  printf("the %d bit is %d \n", i, b & 1 );
}

int getOldReading(int sr, int da){
  return srs[sr].data[da];
}

int readSenseI2c(const int *port, int ix){
  return rand()%(60-5 + 1) + 5;
}

bool debounce(int port) {
  static int state = 0;
  state = (state<<1) | digitalRead(port) | 0xfe00;
  return (state == 0xff00);
}

int readSwitch(const int *port, int ix){
  if (debounce(port[0])) {
    digitalWrite(port[0], !digitalRead(port[0]));
  }
  return digitalRead(port[0]);
}

unsigned long int millis(int max, int min){
  srand(time(0));
  return rand()%(max-min + 1) + min;
}

int readButton(const int *port, int h){
  srand(time(0));
  int prev = inp[h].actions.prev;
  int lptime = inp[h].actions.lptime;
  // int shlo[] = [inp[h].actions.lval;
  int lval = inp[h].actions.lval;
  int sval = inp[h].actions.sval;
  int shlo[] = {0, sval, lval};
  int retval = 0;
  if (debounce(port[0])) {
    digitalWrite(port[0], !digitalRead(port[0]));
  }
  int down =  digitalRead(port[0]);
  if (down & prev==0){ //press button, start long press timer
    lONGpRESStIMR[h] = 100000; //CNG
  } 
  if (!down & prev ){ //release button, calc time pressed -> nval = lval|sval
    int now = 100000 + millis(1000,1);//CNG
    int dif = now - lONGpRESStIMR[h];
    printf("button dif = %d \n", dif);
    if(dif > lptime){
      retval = lval;
    }else retval = sval;
  }
  int wsl = rand()%(2-0 + 1) + 0;
  retval = shlo[wsl];
  printf("fakebutton retval %d, millis(2,0)=%d \n", retval, wsl);
  return retval;
}

int readSense(const int *port, int ix){
  return rand()%(60-5 + 1) + 5;
}

int pow2(int ex){
  int v =1;
  for(int i=0;i<ex;i++) v = v*2;
  return v;
}


void setFlag(int sr, int *FLAG){
  *FLAG = *FLAG | pow2(sr);
}
void unsetFlag(int sr, int *FLAG){
  int mask = 1023 - pow2(sr);
  *FLAG = *FLAG & mask;
}

bool getFlag(int sr, int FLAG){
  return (FLAG >> sr) & 0x1;
}

void printFlag(int FLAG){
  char str[] = "0b00000000";//len10
  for(int i =0;i<8;i++){
    int bi= (int)((FLAG >> i) & 1);
    str[8-i+2] = '0'+bi;
  }
  printf("%d: %s \n",FLAG, str);
}


void invokeInp(int ix, int sen, int (*readSense)(const int *port, int sen)){
  int reads = inp[ix].numreadings;
  srand(time(0));
  for (int k=0;k<reads;k++){
    int nval = readSense(inp[ix].port , sen);//(max-min +1) + min
    int oval = getOldReading(inp[ix].tar[k].gets[0][0], inp[ix].tar[k].gets[0][1]);
    if(nval!=oval){
      for(int m=0;m<inp[ix].tar[k].numtargs;m++){//for all targets
        int sr = inp[ix].tar[k].gets[m][0];
        /* for switch with tsec>0*/
        int tsec;
        if(sen == 1 & oval == 0 & nval ==1 & srs[sr].data[1]>0){//i=1=switch is on 
          tsec =srs[sr].data[1]; 
          sTRTsWtIMR[sr] = 4445567; //millis() //CNG//
        }
        if(sen == 1 & oval == 1 & nval == 1 & srs[sr].data[1]>0){        
          unsigned long starttime = sTRTsWtIMR[sr];
          unsigned long endtime = sTRTsWtIMR[sr]-rand()%((120*1000 - 0) + 1); //CNG// max tsec =32767
          if(starttime-endtime>tsec*1000) {
            nval=0;
            sTRTsWtIMR[sr] = 0;
          }
        }
        /* end of switch with tsec>0*/
        int da = inp[ix].tar[k].gets[m][1]; //target data
        /* for button short/long press*/
        if(nval == inp[ix].actions.lval | nval == inp[ix].actions.sval){
          // printf("invoke %d \n", srs[sr].data[da]);
          nval = oval + nval;
        }
        /* end of button short/long press*/
        srs[sr].data[da]=nval;
        setFlag(sr, &f.HAYsTATEcNG); 
      }
    }
  }  
}

void i_updInputs(){
  for(int ix=0;ix<NUMINP;ix++){
    for(int sen=0;sen<SENSTYPS;sen++){
      if(strcmp(sensors[sen],inp[ix].type)==0){
        switch (sen){
          case 0: //butn
            invokeInp(ix, sen, &readButton);
            break;
          case 1: //switcinp
            invokeInp(ix, sen, &readSwitch);
            break;
          case 2: //1-wire
            invokeInp(ix, sen, &readSenseI2c);
            break;
          case 3: //xt
            invokeInp(ix, sen, &readSenseI2c);
            break;
          case 4: //i2c
            invokeInp(ix, sen, &readSenseI2c);
            break;
          case 5: //spi
            invokeInp(ix, sen, &readSenseI2c);
            break;
          default:
            printf("never heard of it! \n");
            break;  
        }
      }
    }
  }
}

void printSrs(){
  for(int i=0;i<NUMSR;i++){
    printf("{%d, %d, {", srs[i].relay, srs[i].port);
    for(int j=0;j<MAXD; j++){
      printf("%d, ", srs[i].data[j]);
    }
    printf("}}\n");
  }
}

void printPrgs(){
  for(int i=0;i<NUMPRGS;i++){
    printf("{%d, %d, %d, %d{", prgs[i].sr, prgs[i].dax, prgs[i].aid, prgs[i].ev); 
    for(int j=0;j<prgs[i].ev; j++){
      printf("{");
      for(int k=0;k<NPRGDA; k++){
        printf("%d, ", prgs[i].prg[j][k]);
      }
      printf("}, ");
    }
    printf("}}\n");
  }
}


int digitalRead(int port){
  return rand() & 1;
}

void digitalWrite(int port, int onoff){
  //
}

void pubState(int sr){
  printf("published State of sr=%d \n", sr);
}

void pubTimr(int sr){
  printf("published Timr of sr=%d \n", sr);
}

void updCtrl(int sr, int x){
  int typidx = srs[sr].typidx;
  switch (typidx){
    case 0: //diffctrl;
    {
      printf("in updCtrl case 0 \n");
      int conoff =  srs[sr].data[0];
      int onoff=0; 
      int cdif = abs(srs[sr].data[1]-srs[sr].data[2]);
      int dif = srs[sr].data[3];
      int difgt0 = srs[sr].data[4];
      if(cdif > dif){
        onoff = difgt0 ? 1 : 0;
      }else {
        onoff = difgt0 ? 0 : 1;
      }
      if(onoff!=conoff) srs[sr].data[0] = onoff;
      if(digitalRead(srs[sr].port)!=onoff) digitalWrite(srs[sr].port, onoff);
      pubState(sr); 
      if (onoff==1) setFlag(sr, &f.ISrELAYoN);
      unsetFlag(sr, &f.HAYsTATEcNG); 
      break;
    }   
    case 1: //tstat
    {
      int conoff =  srs[sr].data[0];
      int onoff=0; 
      int reading = srs[sr].data[1];
      int setting = srs[sr].data[2];
      int dif = srs[sr].data[3];
      if(reading<(setting-dif)) onoff = 1;
      if(reading>(setting+dif)) onoff = 0;
      if(conoff != onoff) srs[sr].data[0]=onoff;
      if(digitalRead(srs[sr].port)!=onoff) digitalWrite(srs[sr].port, onoff);
      pubState(sr); 
      if (onoff==1) setFlag(sr, &f.ISrELAYoN);
      unsetFlag(sr, &f.HAYsTATEcNG); 
      break; 
    }
    case 2: //switch
    {
      int conoff =  srs[sr].data[0];
      int tsec =  srs[sr].data[1];
      if(digitalRead(srs[sr].port)!=conoff) digitalWrite(srs[sr].port, conoff);
      pubState(sr); 
      if (conoff==1) setFlag(sr, &f.ISrELAYoN);
      unsetFlag(sr, &f.HAYsTATEcNG); 
    } 
      break; 
    case 3: //ctrl
    {
      int conoff =  srs[sr].data[0];
      int onoff;
      int reading =  srs[sr].data[1];
      int setting =  srs[sr].data[2];
      onoff = setting-reading > 0 ? 1: 0;
      if(conoff != onoff){
        if(digitalRead(srs[sr].port)!=onoff) digitalWrite(srs[sr].port, onoff);
      }
      pubState(sr); 
      if (onoff==1) setFlag(sr, &f.ISrELAYoN);
      unsetFlag(sr, &f.HAYsTATEcNG); 
    }
      break; 
    case 4: //array
    {
      pubState(sr); 
      unsetFlag(sr, &f.HAYsTATEcNG); 
    }
      break; 
    default:
      printf("never heard of it! \n");
      break;  

  } 
}

/*get sr and darr from ArduinoJson on ipayload*/
void setXdata(char* idev, int sr, int darr[]){
  int didx = whichDev(idev);
  int xidx = getXdataIdx(didx ,sr);
  if(xidx>-1){
    int numd = tds[getTypIdx(xdata[xidx].type)].numdl;
    for (size_t i = 0; i < numd; i++)
    {
      int ntargs = xdata[xidx].tar[i].numtargs;
      if(ntargs>0){
        for (size_t j = 0; j < ntargs; j++)
        {
          int sr = xdata[xidx].tar[i].gets[j][0];
          int da = xdata[xidx].tar[i].gets[j][1];
          srs[sr].data[da]=darr[i];
        }
      }
    }
  }
}

void findCurNxt(int idx, int &cur, int &nxt, int &tleft){
  int ev = prgs[idx].ev;
  int hr =hour();
  int min = minute();
  tleft = 0;
  printf("time= %d:%d \n", hr, min);
  for(int j=0; j<ev;j++){
    if (hr == prgs[idx].prg[j][0]){
      if (min < prgs[idx].prg[j][1]){
        cur = j-1;
        break;
      }
    }
    if (hr < prgs[idx].prg[j][0]){
      cur= j-1;
      break;
    }
    cur =j;
  }
  nxt = cur+1;
  if (nxt>=ev) nxt=0;  
  if (nxt==0) tleft = (23-hr)*60+(59-min) +1;
  else {
    int nxthr = prgs[idx].prg[nxt][0];
    int nxtmin = prgs[idx].prg[nxt][1];
    if(nxtmin < min){//12:25 -> 14:05
      nxtmin=nxtmin+60;
      nxthr--;
    }
    tleft= (nxthr-hr)*60 + (nxtmin - min);
  }
}

void ckAlarms(int sr, int b){
  int idx = getPrgIdx(sr);
  int dax = prgs[idx].dax;
  /*set current program setting in srs*/
  int cur, nxt, tleft; 
  findCurNxt(idx, cur, nxt, tleft);
  printf("prgidx=%d,cur:%d setting:%d, nxt:%d tleft=%d\n",idx, cur, prgs[idx].prg[cur][2],nxt,tleft);
  srs[sr].data[dax] = prgs[idx].prg[cur][2];
  updCtrl(sr, 0);  
  int onoff= digitalRead(srs[sr].port);
  if(onoff==1 & getFlag(sr, f.HAStIMR)) {
    tIMElEFT[sr] = tleft*60;
    setFlag(sr, &f.IStIMERoN);
  }
}

int getPrgIdx(int sr){
  int idx = -1;
  for (size_t i = 0; i < NUMPRGS; i++)
  {
    if(prgs[i].sr==sr) idx = i;
  }
  return idx;
}

void updTimers(int sr, int b){
  bool pub = false;
  if(tIMElEFT[sr]>0){
    tIMElEFT[sr] -= f.cREMENT;
    if(tIMElEFT[sr]<0){
      tIMElEFT[sr] = 0;
      unsetFlag(sr, &f.IStIMERoN);
    }
    pub=true;
  }
  if(pub && f.cONNectd){
    pubTimr(sr);
  }
}

int hour(){
  return rand()%(23);
}

int minute(){
  return rand()%(59);
}

void cmdMsg(int sr, int sra[]){
  int dax = cmds[sr].nda; 
  for(int i=0;i<dax;i++){ 
    int idx = cmds[sr].data[i]; //srs data index for the cmd data
    srs[sr].data[idx]= sra[i];
  }
  setFlag(sr, &f.HAYsTATEcNG);
}

void prgMsg(int sr, int ev, int pro[][3]){
  int prgx = getPrgIdx(sr);
  prgs[prgx].ev = ev;
  for(int i=0;i<ev;i++){
    for(int j=0;j<NPRGDA;j++){
      prgs[prgx].prg[i][j]= pro[i][j];
    }
  }
  setFlag(sr, &f.CKaLARM);
}
int u_getNda(int sr){
  int typidx =srs[sr].typidx;
  return tds[typidx].numdl;
}