#include<stdio.h>
#include <string.h>
#include "CONFIG.h"

/*dev extern device variables*/  
char devid[9]="CYURD128";
char owner[254]="tim@sitebuilt.net";
char pwd[24]="geniot";
char mqtt_server[60]="sitebuilt.net";
char mqtt_port[6]="1884";

const char xdevtpc[NUMXD][MAXDSTR] = 
{"CYURD006/srstate", "CYURD018/srstate"};

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

srs_t srs[NUMSR] ={ //pow(2,sr) = HASpROG=0b0001011, HAStIMR =0b0001100,
  {0, D8, 0, {0, 67, 45, 9, 1}},//difctrl "tank-panel" prg
  {1, D7, 1, {1, 67, 70, 1}},//tstat "lr" prg
  {2, D6, 1, {0, 35, 41, 1}},//tstat "dr"
  {3, D5, 2, {0, 120}},//relay "door_strike" prg timr 32768 sec or ~9hrs
  {4, D4, 3, {0, 74, 130}},//hilimit "bbq" timr
  {5, -9, 4, {67, 65, 66, 68, 35}},//array "readings" (of readings)
  {6, -9, 4, {60, 15, 12, 8, 11, 2, 5, 12}}//array "currents" {of currents}
};

const cmd_t cmds[NUMSR] ={ /// {sr, dax, data{})
  {0, 1, {3}},//difctrl "tank-panel" prg
  {1, 3, {0, 2, 3}},//tstat "lr" prg
  {2, 3, {2, 3}},//tstat "dr"
  {3, 2, {0, 1}},//relay "door_strike" prg timr 32768 sec or ~9hrs
  {4, 1, {2}},//hilimit "bbq" timr
  {5, 0, {}},//array "readings" (of readings)
  {6, 0, {}}//array "currents" {of currents}
};

/*prgs extern data structure initalization*/ 
prgs_t prgs[NUMPRGS]={  
  // prg:{sr,dax,aid,ev,prg[[]],hms} 
  {0, 3, 255, 3, {{0,0,9},{9,15,13},{18,34,21}}, 1500},  
  {1, 2, 255, 3, {{0,0,68},{10,20,69},{20,30,65}}, 1503},  
  {3, 0, 255, 3, {{0,0,0},{7,12,1},{13,14,0}}, 1504}, 
  {4, 2, 255, 3, {{0,0,130},{5,10,140},{20,0,150}}, 1505},   
};

unsigned long sTRTsWtIMR[NUMSR] ;
unsigned long lONGpRESStIMR[NUMINP] ;
unsigned long tIMElEFT[NUMSR] ;

const char sensors[SENSTYPS][MAXSSTR] =
{"butn", "switch", "1-wire", "dht", "i2c", "spi","ext"};


const inp_t inp[NUMINP] = {
  {{D0}, "butn", 1, 
    { {1, {{2, 2}}} }, 
    {1, 10, 0, 500, 0} },
  {{D1}, "butn", 1, 
    { {1, {{2, 2}}} }, 
    {-1, -10, 0, 500, 0} },
  {{D1}, "switch", 1,
    { {1, {{3, 0}}} }, 
    {200} },
  {{D2}, "1-wire", 3, 
    { {2, {{0, 1}, {1, 1}}},  
      {1, {{5, 0}}}, 
      {2, {{5, 1}, {5, 2}}} }, 
    {} },
  {{D3}, "dht", 2, 
    { {1, {{5, 3}}}, 
      {1, {{5, 4}}} }, 
    {}  },                                               //
  {{SCL, SDA}, "i2c", 8, 
    { {1, {{6, 0}}}, 
      {1, {{6, 1}}}, 
      {1, {{6, 2}}}, 
      {1, {{6, 3}}}, 
      {1, {{6, 4}}}, 
      {1, {{6, 5}}}, 
      {1, {{6, 6}}}  }, 
    {}  }, // currents
  {{SCK, SDO, SDI, CS}, "spi", 1, 
    { {1, {{4, 1}}}  }, 
    {}  }                                                     // thermo
};

xdata_t xdata[NUMXD] = {
  {1, 0, "cs", 
    { {0, {{}}},
      {1, {{6, 7}}},//reading
      {0, {{}}},
      {0, {{}}}
    }},
  {0, 0, "se", 
    { {1, {{0, 2}}} //reading
    }}
};

/*INCOMING const mqtt topics*/
const char subTopics[NUMTOPICS][MAXSSTR] =
  {"devtime", "cmd", "prg", "req", "set"};

/*flags extern data structure*/
flags_t f {
  0,//cONNectd
  0,//hayWIFI
  0,//hayMQTT
  0,//aUTOMA
  0,//fORCErESET
  5,//cREMENT
  0b0011010,//HAStIMR 1100000 64+32=96
  0b0000000,//IStIMERoN
  0b0011011,//HAYpROG 11000010 =128+64+4=198
  0b0000000,//HAYsTATEcNG
  0b0011011,//CKaLARM
  0,//ISrELAYoN
  0b00100010,//dOrEC
  0b0001011//HASpROG
};

extern char itopic[40];
extern char ipayload[256];
extern char idev[17];
extern unsigned int ipaylen;
extern bool NEW_MAIL;

