#include<stdio.h>
#include <string.h>
#include "bconf.h"
#include "butil.h"
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
int getDnIdx(int typidx, const char* dname){
  int ret;
  for (int i =0; i<MAXD;i++){
    if(strcmp(tds[typidx].dats[i], dname)==0){ret =i;}
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


//getStoredValue(0, "se", "reading")
//getStoredValue(1, "di", "rdB")
int getStoredValue(int sr, const char* type, const char* dname){
  int typidx = getTypIdx(type);
  int dnidx = getDnIdx(typidx, dname);

  return dnidx;
}

bool isThisXdata(char* idev){
  char dev[9];
  devOfDevTpc(idev,dev);
  bool ret = false;
  if (strcmp(devid, dev)==1){ret = true;}
  return ret; 
}

void devOfDevTpc(char* dt, char* d){
  size_t i;
  for (i=0;i<strlen(dt);i++){
    d[i]=dt[i];
    if(dt[i]=='/'){break;}
  }
  d[i]='\0';  
}

int whichDev(char* idev){
  int xd=-1;
  for(int i=0;i<NUMXD;i++){
    // printf("%s, %s, %d \n", idev, xdevtpc[i],strcmp(idev, xdevtpc[i])==0);
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



int readSenseI2c(int port, int inpidx){
  return rand()%(60-5 + 1) + 5;
}

bool debounce(int port) {
  static int state = 0;
  state = (state<<1) | digitalRead(port) | 0xfe00;
  return (state == 0xff00);
}

int readSwitch(int port, int inpidx){
  if (debounce(port)) {
    digitalWrite(port, !digitalRead(port));
  }
  return digitalRead(port);
}

unsigned long int millis(int max, int min){
  srand(time(0));
  return rand()%(max-min + 1) + min;
}

int readButton(int port, int h){
  srand(time(0));
  int prev = inp[h].actions.prev;
  int lptime = inp[h].actions.lptime;
  // int shlo[] = [inp[h].actions.lval;
  int lval = inp[h].actions.lval;
  int sval = inp[h].actions.sval;
  int shlo[] = {0, sval, lval};
  int retval = 0;
  if (debounce(port)) {
    digitalWrite(port, !digitalRead(port));
  }
  int down =  digitalRead(port);
  if (down & prev==0){
    inp[h].actions.sttime = 100000;
  } 
  if (!down & prev ){
    int now = 100000 + millis(1000,1);
    int dif = now - inp[h].actions.sttime;
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

int readSense(int port, int inpidx){
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
  // printf("sr=%d, pow2(sr)=%d FLAG=", sr, pow2(sr)); 
  // printFlag(*FLAG);
  int mask = 1023 - pow2(sr);
  *FLAG = *FLAG & mask;
  // printf("sr=%d, FLAG=", sr); 
  // printFlag(*FLAG);
}

void printFlag(int FLAG){
  char str[] = "0b00000000";//len10
  for(int i =0;i<8;i++){
    int bi= (int)((FLAG >> i) & 1);
    str[8-i+2] = '0'+bi;
  }
  printf("%d: %s \n",FLAG, str);
}


void invokeInp(int h, int i, int (*readSense)(int port, int i)){
  int reads = inp[h].numreadings;
  srand(time(0));
  for (int k=0;k<reads;k++){
    int nval = readSense(inp[h].port[0], h);//(max-min +1) + min
    int oval = getOldReading(inp[h].tar[k].gets[0][0], inp[h].tar[k].gets[0][1]);
    // printf("h=%d, i=%d, oval is %d \n",h, i, oval);
    if(nval!=oval){
      for(int m=0;m<inp[h].tar[k].numtargs;m++){
        int sr = inp[h].tar[k].gets[m][0];
        /* for switch with tsec>0*/

        int tsec;
        if(i == 1 & oval == 0 & nval ==1 & srs[sr].data[1]>0){//i=1=switch
          tsec =srs[sr].data[1];
          stime[sr] = 4445567; //millis() //CNG//
        }
        if(i == 1 & oval == 1 & nval == 1 & srs[sr].data[1]>0){
          unsigned long starttime = stime[sr];
          unsigned long endtime = stime[sr]-rand()%((120*1000 - 0) + 1); //CNG// max tsec =32767
          if(starttime-endtime>tsec*1000) nval=0;
          stime[sr] = 0;
        }
        /* end of switch with tsec>0*/
        int da = inp[h].tar[k].gets[m][1];
        /* for button short/long press*/
        if(nval == inp[h].actions.lval | nval == inp[h].actions.sval){
          // printf("invoke %d \n", srs[sr].data[da]);
          nval = oval + nval;
        }
        /* end of button short/long press*/
        srs[sr].data[da]=nval;
        // printf("h=%d, i=%d, s=%d, nval is %d \n",h, i, sr, nval);
        setFlag(sr, &f.HAYsTATEcNG); 
      }
      
    }
    // printf("nval leaving =%d \n",nval);
  }  
}

void updInputs(){
  for(int h=0;h<NUMINP;h++){
    for(int i=0;i<SENSTYPS;i++){
      if(strcmp(sensors[i],inp[h].type)==0){
        switch (i){
          case 0: //butn
            invokeInp(h, i, &readButton);
            break;
          case 1: //switch
            invokeInp(h, i, &readSwitch);
            break;
          case 2: //1-wire
            invokeInp(h, i, &readSenseI2c);
            break;
          case 3: //dht
            invokeInp(h, i, &readSenseI2c);
            break;
          case 4: //i2c
            invokeInp(h, i, &readSenseI2c);
            break;
          case 5: //spi
            invokeInp(h, i, &readSenseI2c);
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

int digitalRead(int port){
  return rand() & 1;
}

void digitalWrite(int port, int onoff){
  //
}

void pubState(int sr){
  printf("published State of sr=%d \n", sr);
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
      unsetFlag(sr, &f.HAYsTATEcNG); 
      break; 
    }
    case 2: //switch
    {
      int conoff =  srs[sr].data[0];
      int tsec =  srs[sr].data[1];
      if(digitalRead(srs[sr].port)!=conoff) digitalWrite(srs[sr].port, conoff);
      pubState(sr); 
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
  printf("%s size of idev %d \n",idev, (int)strlen(idev));
  int didx = whichDev(idev);
  int xidx = getXdataIdx(didx ,sr);
  if(xidx>-1){
    printf("xidx=%d, %s\n",xidx,xdata[0].type);
    int numd = tds[getTypIdx(xdata[xidx].type)].numdl;
    printf("numd%d %d\n",numd, tds[4].numdl);
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
  
  

  // int xsr;
  // if(strlen(idev)>2){
  //   xsr =getXsr(sr, whichDev(idev));
  // }else {xsr=sr;}
  // int tdidx = getTdsIdx(xsr);
  // int xdidx = getXdataIdx(xsr);
  // xdata[xdidx][2] = sr; 
  // for (int i=0;i<tds[tdidx].numdl;i++){
  //   xdata[xdidx][i+3] = darr[i];
  // }
}

// int getXsr(int osr, int xd){
//   int xsr = -17;
//   for(int i=0;i<NUMSR;i++){
//     // printf("%d == %d is %d && %d = %d  is %d\n", 10+ports[i].in, osr, 10+ports[i].in == osr , ports[i].xd, xd, ports[i].xd==xd);
//     if(10+ports[i].in == osr && ports[i].xd==xd){xsr=i;}
//   }
//   return xsr;
// }






// void printXdata(){
//   for (int i=0;i<NUMXD;i++){
//     printf("{");
//     for (int j=0;j<MAXD+3;j++){
//       printf("%d, ",xdata[i][j]);
//     }
//     printf("}, \n");
//   }
// }

