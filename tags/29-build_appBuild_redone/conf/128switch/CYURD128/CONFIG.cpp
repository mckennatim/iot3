
#include<stdio.h>
#include <string.h>
#include "CONFIG.h"
 
/*dev device variables*/  
char devid[9]="CYURD128";
char owner[254]="tim@sitebuilt.net";
char pwd[24]="geniot";
char mqtt_server[60]="sitebuilt.net";
char mqtt_port[6]="1884";
  
srs_t srs[NUMSR] ={//{sr, port, typidx, data[]}   
  {0, D8, 0, {0, 67, 45, 9, 1,}}, //difctrl solar 
  {1, D6, 1, {0, 35, 42, 1,}}, //tstat dr 
  {2, D7, 2, {0, 120,}}, //relay sw1 
};
 
const cmd_t cmds[NUMSR] ={//{sr,nda,data{})
  {0, 1, {3}}, //difctrl solar
  {1, 3, {0, 2, 3}}, //tstat dr
  {2, 2, {0, 1}}, //relay sw1
};
  
prgs_t prgs[NUMPRGS]={//{sr, dax, aid, ev, prg[[h,m,set]], hms} 
 {0, 3, 255, 2, {{0,0,11},{9,15,10}}},
 {1, 3, 255, 1, {{0,0,68}}},
};
 
inp_t inp[NUMINP] = {
  {{D2,}, "1-wire", 3, 
    { {1, {{0, 2}, }},
      {2, {{1, 1}, {3, 1}, }},
      {1, {{2, 1}, }}, },
    {}, },
  {{D1,}, "switch", 1, 
    { {1, {{2, 0}, }}, },
    {200,}, },
};
 
const char xdevtpc[NUMXDEV][MAXDSTR] = 
{"CYURD006/srstate", "CYURD118/srstate"};
 
const xdata_t xdata[NUMXD] = 
{
  {0, 2, 2,//dev,sr,nd 
    { //xdidx, {numtargs {{sr, tdidx}}}
      {2, {1, {{0, 1}}}}, 
      {1, {1, {{1, 1}}}}, 
    },
  },
  {0, 0, 1,//dev,sr,nd 
    { //xdidx, {numtargs {{sr, tdidx}}}
      {0, {1, {{2, 1}}}}, 
    },
  },
  {1, 3, 1,//dev,sr,nd 
    { //xdidx, {numtargs {{sr, tdidx}}}
      {3, {1, {{3, 2}}}}, 
    },
  },
};
 
/*flags extern data structure*/
flags_t f {
  0b100,//HAYtIMR 1100000 64+32=96
  0b011,//HAYpROG 11000010 =128+64+4=198
  0b001,//dOrEC 
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
};

const tds_t tds[NUMTYP] ={
  {"difctrl", 5, {"onoff", "rdA", "rdB", "dif", "difgt0"}},
  {"tstat", 4, {"onoff", "reading", "setting", "dif"}},
  {"relay", 2, {"onoff", "tsec"}},
  {"ctrl", 3, {"bell", "reading", "setting"}},
  {"array", 8, {"a0", "a1", "a2", "a3", "a4", "a5", "a6", "a7"}},
  {"se", 1, {"reading"}},
  {"cs", 4, {"onoff", "reading", "hi", "lo"}},
  {"re", 1, {"onoff"}}
};

unsigned long sTRTsWtIMR[NUMSR] ;
unsigned long lONGpRESStIMR[NUMINP] ;
unsigned long tIMElEFT[NUMSR] ;

const char sensors[SENSTYPS][MAXSSTR] =
{"butn", "switch", "1-wire", "dht", "i2c", "spi","ext"};

/*INCOMING const mqtt topics*/
const char subTopics[NUMTOPICS][MAXSSTR] =
  {"devtime", "cmd", "prg", "req", "set"};

extern char itopic[40];
extern char ipayload[256];
extern char idev[17];
extern unsigned int ipaylen;
extern bool NEW_MAIL;

