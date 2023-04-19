#include "CONFIG.h"
#include "Util.h"
#include <Arduino.h>

void u_scanFLAGand(int FLAG, int LEN, void (*doStuff)(int, int)){
  int b = FLAG;
  for(int i=0;i<LEN;i++){
    if(b & 1){
      doStuff(i,b);
    }
    b = b >> 1;
  }
}

void u_doStuff(int i,int b){
  printf("the %d bit is %d \n", i, b & 1 );
}

int pow2(int ex){
  return pow(2,ex);
}

void u_bitShift(int bnum, int LEN, void (*doStuff)(int, int)){
  int b = bnum;
  for(int i=0;i<LEN;i++){
    if(b & 1){
      doStuff(i,b);
    }
    b = b >> 1;
  }
}

void u_setFlag(int sr, int *FLAG){
  *FLAG = *FLAG | pow2(sr);
}
void u_unsetFlag(int sr, int *FLAG){
  int mask = 1023 - pow2(sr);
  *FLAG = *FLAG & mask;
}

int u_getFlag(int sr, int FLAG){
  return (FLAG >> sr) & 0x1;
}

void u_printFlag(int FLAG){
  char str[] = "0b00000000";//len10
  for(int i =0;i<8;i++){
    int bi= (int)((FLAG >> i) & 1);
    str[8-i+2] = '0'+bi;
  }
  printf("%d: %s \n",FLAG, str);
}

void u_printSrs(){
  for(int i=0;i<NUMSR;i++){
    printf("{%d, %d, {", srs[i].relay, srs[i].port);
    for(int j=0;j<MAXD; j++){
      printf("%d, ", srs[i].data[j]);
    }
    printf("}}\n");
  }
}

void u_printPrgs(){
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

int u_getNumdl(int sr){
  int typidx =srs[sr].typidx;
  return tds[typidx].numdl;
}

int u_getPrgIdx(int sr){
  int idx = -1;
  for (size_t i = 0; i < NUMPRGS; i++) if(prgs[i].sr==sr) idx = i;
  return idx;
}