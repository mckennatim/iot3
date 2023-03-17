#include<stdio.h>
#include <string.h>

// #include "Util.h"

#define sizeOf(a) (sizeof(a) / sizeof(a[0]))

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

//char ca[4][10];

const char* ca[4] = {"reading", "onoff", "hi", "lo"} ;
// string cs[] = {"reading", "onoff", "hi", "lo"} ;
const char* cb[2][4]= {{"reading", "onoff", "hi", "lo"},{"tsec", "onoff"}};

// int sizeNotNull(char *cc[]){
//   int ret;
//   for (int i=0;i<6;i++){
//     if (cc[i]==NULL){
//       ret =i;
//     }
//   }
//   return ret;
// };

// int sizeNotNull2(string cc[]){
//   int ret;
//   for (int i=0;i<2;i++){
//     if (cc[i]==NULL){ 
//       ret =i;
//     }
//   }
//   return ret;
// };
void getCsIdx(const char* s){
  printf("this char* is %s \n",s);
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

int sztd = 6;

// struct td_t {
//   char* tzyp;
//   int numd;
//   const char* dname[7];
// };

// td_t td[6]; 

// td[0].tzyp = "se";
// td[0].numd = 1;

// td[1] ={"cs", 1, {"reading", "onoff", "hi", "lo"}};
//   {"cr", 1, {"onoff", "tsec"}},
//   {"rel", 1, {"onoff"}},
//   {"di", 1, {"onoff", "rdA", "rdB", "dif", "hyst", "gd"}},
// }

/*START prg data structure declarations*/  
struct prg_t{
  int sr;
  int aid;
  int ev;
  int numdata;
  int prg[11][4];//max 11 events [hr,min,max,min]  
  char ts[10];
  char tsa[6][6];
  int hms;
};
struct prgs_t{
  int numprgs;
  prg_t prg[2];/*MODIFY*/
};

prgs_t prgs{  
  1, // numprgs 
  { // prg:{sr,aid,ev,numdata,prg[[]],hms} 
    {3, 255, 1, 1, {{0,0,0,}}, "dog",  {"onoff", "tsec"}, 1500},   
    {4, 255, 1, 1, {{0,0,0,}}, "cat",  {"onoff", "tsec", "hi"}, 1500}   
  }
};

struct td_t {
  char type[8];
  int numd;
  char dats[6][10];
};

struct tds_t{
  int numtyp;
  td_t td[5];
};

const tds_t tds ={
  5,
  {
    {"se", 1, {"reading"}},
    {"cs", 4, {"reading", "onoff", "hi", "lo"}},
    {"cr", 2, {"onoff", "tsec"}},
    {"rel", 1, {"onoff"}},
    {"di", 6, {"onoff", "rdA", "rdB", "dif", "hyst", "gd"}},
  }
};

int getTypIdx(const char* type){
  int ret;
  for (int i =0; i<tds.numtyp;i++){
    if(strcmp(tds.td[i].type, type)==0){ret =i;}
  }
  return ret;
}
int getDnIdx(int typidx, const char* dname){
  int ret;
  for (int i =0; i<tds.td[typidx].numd;i++){
    if(strcmp(tds.td[typidx].dats[i], dname)==0){ret =i;}
  }
  return ret;
}

int getStoredValue(int sr, const char* type, const char* dname){
  int typidx = getTypIdx(type);
  int dnidx = getDnIdx(typidx, dname);
  return dnidx;
}

int main()   // define the main function  
{  
    printf("getStird %d \n", getStoredValue(0, "di", "hyst"));
    printf("rel typidx =%d \n",getTypIdx("rel"));
    printf("dog %s \n", prgs.prg[0].tsa[1]);
    printf("dog %s \n", prgs.prg[1].tsa[2]);
    printf("dog %s \n", tds.td[3].dats[0]);
    printf("dog %s \n", tds.td[4].dats[3]);
    printf("sizog, %d \n", (int)sizeof(tds)/(int)sizeof(tds.td[0]));
    printf("sizog, %d \n", tds.numtyp);
    // const char* td[0].dname = {"reading"};
    printf("Welcome to JavaTpoint \n"); 
    printf("srs.se[0].reading: %d \n", (int)srs.se[0].reading);
    printf("srs.se[0].reading: %d \n", (int)srs.cs[0].onoff);
    int sr = 0;
    int dog = getRelSt(sr);
    printf("the relay state = %d \n",dog);
    setRelSt(sr, 1);
    printf("the relay state = %d \n",getRelSt(sr));
    printf("ca[%d] is %s \n", 1, ca[1]);
    // printf("cs[%d] is %s \n", 2, cs[2]);
    if(strcmp(ca[1],"onoff")==0){
      printf("dey da same \n");
    }
    printf("sizeOf(ca)=%d \n",(int)sizeOf(ca));
    printf("sizeOf(cb[1])=%d \n",(int)sizeOf(cb[0]));
    printf("cb[1][0] %s \n",cb[1][0]);

    // printf("sizeNotNull(ca[0]) %d \n", sizeNotNull(cb[0]));
    getCsIdx(ca[0]);
    return 1; // print the statement.  
}  

