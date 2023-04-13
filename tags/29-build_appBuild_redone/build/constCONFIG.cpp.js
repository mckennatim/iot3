const chead = 
`
#include<stdio.h>
#include <string.h>
#include "CONFIG.h"
`

const cvar = `
//{sr, port, typidx, data[]}
srs_t srs[NUMSR] ={ //pow(2,sr) = HASpROG=0b0001011, HAStIMR =0b0001100,
  {0, D8, 0, {0, 67, 45, 9, 1}},//difctrl "tank-panel" prg
  {1, D7, 1, {1, 67, 70, 1}},//tstat "lr" prg
  {2, D6, 1, {0, 35, 41, 1}},//tstat "dr"
  {3, D5, 2, {0, 120}},//relay "door_strike" prg timr 32768 sec or ~9hrs
  {4, D4, 3, {0, 74, 130}},//hilimit "bbq" timr
};

inp_t inp[NUMINP] = {
  {{D1}, "switch", 1,
    { {1, {{3, 0}}} }, 
    {200} },
  {{D3}, "1-wire", 3, 
    { {1, {{6, 6}}},//0,rdA 1,reading
      {1, {{5, 0}}}, 
      {4, {{5, 1}, {5, 2}, {6, 4}, {6, 5}}} }, 
    {1} },
  {{D2}, "1-wire", 3, 
    { {2, {{2, 1}, {6, 1}}}, //0,rdA, 
      {2, {{0, 1}, {1, 1}}}, 
      {4, {{5, 4}, {5, 5}, {6, 2}, {6, 3}}} }, 
    {0} },                                             
};

xdata_t xdata[NUMXD] = {
  {0, 0, "se", 
    { {1, {{0, 2}}} //rdB
    }}
};

/*prgs extern data structure initalization*/ 
prgs_t prgs[NUMPRGS]={  
  // prg:{sr, dax, aid, ev, prg[[]], hms} 
  {0, 3, 255, 3, {{0,0,9},{9,15,13},{18,34,21}}, 1500},  
  {1, 2, 255, 3, {{0,0,68},{10,20,69},{20,30,65}}, 1503},
}

/*flags extern data structure*/
flags_t f {
  0b0011010,//HAYtIMR 1100000 64+32=96
  0b0011011,//HAYpROG 11000010 =128+64+4=198
  0b00100010,//dOrEC
`


const cfoot =
`
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

`

export{ chead, cfoot}