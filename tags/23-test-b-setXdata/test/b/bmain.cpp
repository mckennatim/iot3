#include<stdio.h>
#include <string.h>
#include "bconf.h"
#include "butil.h"
#include <time.h>
// #include <ArduinoJson.h>

char sepayload[60] = "{\"id\":0, \"darr\":[54]}";
char dipayload[100] = "{\"id\":0, \"darr\":[0, 54,67, 10, 1, 0]}";

int id7 = 0;
int id5 = 0;
int darr5[1]={54};
int darr7[6]={1, 68, 46, 10, 1, 1};
char c[20] = "CYURD006/srstate";//idev 0
char d[20] = "CYURD018/srstate";//idev 1

void ckRelays(){

}

int main()   // define the main functio
{  
  printf("Welcome to JavaTpoint \n"); 

  // printf("DOREC %d \n", f.HAYsTATEcNG);
  // printf("DOREC %d \n", f.HAYsTATEcNG);
  bitShift(f.HASpROG, NUMSR, &doStuff);
  printSrs();
  updInputs();
  printSrs();
  // printFlag(f.HAStIMR);
  // printFlag(f.HASpROG);
  // printFlag(f.dOrEC);
  // printf("digread %d \n",digitalRead(D8));
  printFlag(f.HAYsTATEcNG);
  scanFLAGand(f.HAYsTATEcNG, NUMSR, &updCtrl);
  printFlag(f.HAYsTATEcNG);
  printSrs();


  // printf("getStird %d \n", getStoredValue(0, "difctrl", "rdB"));
  // printf("%s \n",xdevtpc[0]);
  // printf("%d \n", xdata[1][8]);
  // printf("%s \n", tds[4].dats[4]);

  // char e[9];
  // size_t l = strlen(d);
  // char f[20];
  // devOfDevTpc(xdevtpc[0],e);
  // printf("%d, %c, %d, %s\n",(int)l, d[1], d[1]=='Y', e);
  // printf("%s \n",devid);
  // printf("%d \n", isThisXdata(d));
  // printf("whiichDev %d \n", whichDev(d));
  // int xsr = getXsr(1, whichDev(d));
  // printf("xsr = %d \n",xsr );
  // int numdl = tds[getTdsIdx(xsr)].numdl;
  // printf("numdl = %d \n",numdl );
  setXdata(d, 0, darr7);
  // printf("%d, %s\n",getXdataIdx(1,0),xdata[getXdataIdx(1,0)].type);
  // printf("%d, %s\n",getTypIdx("se"),xdata[getXdataIdx(1,0)].type);
  // printXdata();
  // printf("%s \n", tds[0].type);
  printf("%d xd",xdata[0].tar[1].numtargs);
  printSrs();
}