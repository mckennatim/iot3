#include<stdio.h>
#include <string.h>
#include "aconf.h"
#include "autil.h"

int getTypIdx(const char* type){
  int ret;
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
    printf("%s, %s, %d \n", idev, xdevtpc[i],strcmp(idev, xdevtpc[i])==0);
    if(strcmp(idev, xdevtpc[i])==0){xd= i;}
  }
  return xd;
}

int getXsr(int osr, int xd){
  int xsr = -17;
  for(int i=0;i<NUMSR;i++){
    // printf("%d == %d is %d && %d = %d  is %d\n", 10+ports[i].in, osr, 10+ports[i].in == osr , ports[i].xd, xd, ports[i].xd==xd);
    if(10+ports[i].in == osr && ports[i].xd==xd){xsr=i;}
  }
  return xsr;
}

int getTdsIdx(int xrs){
  int tdidx;
  for(int i=0;i<NUMTYP;i++){
    if(strcmp(tds[i].type, ports[xrs].type)==0){tdidx= i;}
  }
  return tdidx;
}

int getXdataIdx(int xsr){
  int xd=-1;
  for(int i=0;i<NUMXD;i++){
    if(xdata[i][0]==xsr){xd= i;}
  }
  return xd;
}

/*get sr and darr from ArduinoJson on ipayload*/
void setXdata(char* idev, int sr, int darr[]){
  printf("%s siz of idev %d \n",idev, (int)strlen(idev));
  int xsr;
  if(strlen(idev)>2){
    xsr =getXsr(sr, whichDev(idev));
  }else {xsr=sr;}
  int tdidx = getTdsIdx(xsr);
  int xdidx = getXdataIdx(xsr);
  xdata[xdidx][2] = sr; 
  for (int i=0;i<tds[tdidx].numdl;i++){
    xdata[xdidx][i+3] = darr[i];
  }
}

void printXdata(){
  for (int i=0;i<NUMXD;i++){
    printf("{");
    for (int j=0;j<MAXD+3;j++){
      printf("%d, ",xdata[i][j]);
    }
    printf("}, \n");
  }
}