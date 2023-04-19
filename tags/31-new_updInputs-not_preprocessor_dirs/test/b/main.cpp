#include<stdio.h>
#include <string.h>
#include "CONF.h"
#include "Util.h"
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
//{"id":3,"sra":{1},"tsec":23}
//{"id":3,"sra":{70,68}}


void ckRelays(){

}

int main()   // define the main functio
{  
  printf("Welcome to JavaTpoint \n"); 
  bitShift(f.HASpROG, NUMSR, &doStuff);
  printFlag(f.HAYsTATEcNG);
  scanFLAGand(f.HAYsTATEcNG, NUMSR, &updCtrl);
  printFlag(f.HAYsTATEcNG);
  printSrs();
  setXdata(d, 0, darr7);
  printSrs();
  printf("prgs %d \n",(int)prgs[3].aid);
  scanFLAGand(f.CKaLARM, NUMSR, &doStuff);
  printf("sr=%d, i=%d \n",3, getPrgIdx(3) );
  scanFLAGand(f.CKaLARM, NUMSR, &ckAlarms);
  printSrs();
  printFlag(f.HAStIMR);
  printf("bit %d is %d \n", 3, getFlag(3, f.HAStIMR));
  scanFLAGand(f.IStIMERoN, NUMSR, &updTimers);
  printPrgs();
  int pro[2][3] = {{0,0,11}, {11,33,14}};
  prgMsg(0, 2, pro);
  printPrgs();
  printf("%d\n",u_getNda(1));
  printSrs();
  i_updInputs();
  printSrs();
}