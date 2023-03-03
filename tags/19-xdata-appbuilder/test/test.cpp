#include<stdio.h>

// #include "Util.h"

/*srs data structure declarations*/ 
struct se_t {//sensors
    int sr;
    int reading;
    bool rec;
    bool isnew;
};
struct cs_t {//controlled sensors
    int sr;
    int reading;
    bool onoff;
    int hi;
    int lo;
    bool rec;
    bool isnew;   
};
struct rel_t {//timers
    int sr;
    bool onoff;
    bool rec; 
    bool isnew;  
};
struct di_t {//diff control
    int sa;
    int sb; 
    int don;
    int doff;
    int maxa;
    int maxb;
    int port;
    bool onoff;
    bool rec;
    bool isnew;
};
struct srs_t {
  int numsr;
  int numse;
  se_t se[1];/*MODIFY*/
  int numcs;
  cs_t cs[2];/*MODIFY*/
  int numrel;
  rel_t rel[1];/*MODIFY*/
  int numdi;
  di_t di[0];/*MODIFY*/
};
extern srs_t srs;
/*srs data structure declarations*/  

srs_t srs {
  4, // numsr 
  1, // numse 
  { // se:{sr, reading} 
    {0, 15}  // temp2
  },
  2, // numcs 
  { // cs:{sr, reading, onoff, hi, lo} 
    {1, 44, 0, 69, 67}, // temp
    {2, 24, 0, 60, 90}  // hum
  },
  1, // numrel 
  { // rel:{sr, onoff} 
    {3, 0}  // timr1
  },
  0, // numdi 
  {} // dif:{sr, sra, srb, diffon, diffoff, maxa, maxb, onoff} 
};

int getRelSt(int sr){
  int rval = -99;
  for (int j=0; j<srs.numcs;j++){
    if(srs.se[j].sr == sr){
      rval = srs.se[j].reading;
    }
  }
  for (int j=0; j<srs.numcs;j++){
    if(srs.cs[j].sr == sr){
      rval = srs.cs[j].onoff;
    }
  }
  for (int j=0; j<srs.numrel;j++){
    if(srs.rel[j].sr == sr){
      rval = srs.rel[j].onoff;
    }
  }
  return rval;
} 

void setRelSt(int sr, int onoff){
  for (int j=0; j<srs.numcs;j++){
    if(srs.cs[j].sr == sr){
      srs.cs[j].onoff = onoff;
    }
  }
  for (int j=0; j<srs.numrel;j++){
    if(srs.rel[j].sr == sr){
      srs.rel[j].onoff = onoff;
    }
  }  
}


int main()   // define the main function  
{  
    printf("Welcome to JavaTpoint \n"); 
    printf("srs.se[0].reading: %d \n", (int)srs.se[0].reading);
    printf("srs.se[0].reading: %d \n", (int)srs.cs[0].onoff);
    int sr = 0;
    int dog = getRelSt(sr);
    printf("the relay state = %d \n",dog);
    setRelSt(sr, 1);
    printf("the relay state = %d \n",getRelSt(sr));
    return 1; // print the statement.  
}  