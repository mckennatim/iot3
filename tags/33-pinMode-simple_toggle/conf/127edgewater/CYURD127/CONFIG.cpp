
#include<stdio.h>
#include <string.h>
#include "CONFIG.h"
 
/*dev device variables*/  
char devid[9]="CYURD127";
char owner[254]="tim@sitebuilt.net";
char pwd[24]="geniot";
char mqtt_server[60]="sitebuilt.net";
char mqtt_port[6]="1884";
  
srs_t srs[NUMSR] ={//{sr, port, typidx, data[]}   
  {0, D1, 5, {1,}}, //power power 
  {1, D6, 2, {0, 120, 2,}}, //relay zone1 
  {2, D7, 3, {0, 120, 1, 0,}}, //delay zone2 
};
 
const cmd_t cmds[NUMSR] ={//{sr,nda,data{})
  {0, 1, {0}}, //power power
  {1, 2, {0, 1}}, //relay zone1
  {2, 2, {0, 1}}, //delay zone2
};
  
prgs_t prgs[NUMPRGS]={//{sr, dax, aid, ev, prg[[h,m,set]], hms} 
 {1, 1, 255, 1, {{0,0,68}}},
 {2, 1, 255, 1, {{0,0,68}}},
};
 
inp_t inp[NUMINP] = {
  {{D5,}, "toggle", 1, 
    { {1, {{1, 0}, }}, },
    {200,}, },
};
 
const char xdevtpc[NUMXDEV][MAXDSTR] = 
{
};
 
const xdata_t xdata[NUMXD] = 
{
};
 
const char sensors[SENSTYPS][MAXSSTR] = 
{"butn", "switch", "1-wire", "dht", "i2c", "spi", "toggle", };
 
/*flags extern data structure*/
flags_t f {
  0b000,//HAYtIMR 1100000 64+32=96
  0b110,//HAYpROG 11000010 =128+64+4=198
  0b000,//dOrEC 
  0,//cONNectd
  0,//hayWIFI
  0,//hayMQTT
  0,//aUTOMA
  0,//fORCErESET
  5,//cREMENT
  0b0000000,//IStIMERoN
  0b0000000,//HAYsTATEcNG
  0b0011011,//CKaLARM
  0,//ISrELAYoN
  0b00000000,//iSnEW
  0b00000000,//dOpUBLISH
};

const tds_t tds[NUMTYP] ={
  {"difctrl", 5, {"onoff", "rdA", "rdB", "dif", "difgt0"}},
  {"tstat", 4, {"onoff", "reading", "setting", "dif"}},
  {"relay", 2, {"onoff", "tsec","sr"}},
  {"delay", 2, {"onoff", "tsec", "sr","prior"}},
  {"array", 8, {"a0", "a1", "a2", "a3", "a4", "a5", "a6", "a7"}},
  {"se", 1, {"reading"}},
  {"cs", 4, {"onoff", "reading", "hi", "lo"}},
  {"re", 1, {"onoff"}}
};

unsigned long sTRTsWtIMR[NUMSR] ;
unsigned long lONGpRESStIMR[NUMINP] ;
int bUTNpREV[NUMINP] ;
unsigned long tIMElEFT[NUMSR] ;

/*INCOMING const mqtt topics*/
const char subTopics[NUMTOPICS][MAXSSTR] =
  {"devtime", "cmd", "prg", "req", "set"};

extern char itopic[40];
extern char ipayload[256];
extern char idev[17];
extern unsigned int ipaylen;
extern bool NEW_MAIL;

