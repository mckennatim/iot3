
#include<stdio.h>
#include <string.h>
#include "CONFIG.h"
 
/*dev device variables*/  
char devid[9]="CYURD127";
char owner[254]="tim@sitebuilt.net";
char pwd[24]="geniot";
char mqtt_server[60]="sitebuilt.net";
char mqtt_port[6]="1884";
 
void initShit(){
  pinMode(srs[0].port, OUTPUT);
  pinMode(srs[1].port, OUTPUT);
  pinMode(srs[2].port, OUTPUT);
  pinMode(srs[3].port, OUTPUT);
  pinMode(srs[4].port, OUTPUT);
  pinMode(inp[0].port[0], INPUT);
}
 
srs_t srs[NUMSR] ={//{sr, port, typidx, data[]}   
  {0, D0, 2, {0, 15, 1, -1, -1,}}, //relay zone1 
  {1, D6, 2, {0, 15, 2, 0, 0,}}, //relay zone2 
  {2, D7, 2, {0, 15, 3, 1, 0,}}, //relay zone3 
  {3, D8, 2, {0, 15, -1, 2, 0,}}, //relay zone4 
  {4, D1, 2, {0, 15, -1, -1, -1,}}, //relay zone5 
};
 
const cmd_t cmds[NUMSR] ={//{sr,nda,data{})
  {0, 2, {0, 1}}, //relay zone1
  {1, 2, {0, 1}}, //relay zone2
  {2, 2, {0, 1}}, //relay zone3
  {3, 2, {0, 1}}, //relay zone4
  {4, 2, {0, 1}}, //relay zone5
};
  
prgs_t prgs[NUMPRGS]={//{sr, dax, aid, ev, prg[[h,m,set]], hms} 
 {4, 1, 255, 1, {{0,0,0}}},
};
 
inp_t inp[NUMINP] = {
  {{D3,}, "toggle", 1, 
    { {1, {{0, 0}, }}, },
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
 
const tds_t tds[NUMTYP] ={
  {"difctrl", 5, {"onoff", "rdA", "rdB", "dif", "difgt0", }},
  {"tstat", 4, {"onoff", "reading", "setting", "dif", }},
  {"relay", 5, {"onoff", "tsec", "nxtsr", "priosr", "prionoff", }},
  {"ctrl", 3, {"bell", "reading", "setting", }},
  {"array", 8, {"a0", "a1", "a2", "a3", "a4", "a5", "a6", "a7", }},
  {"se", 1, {"reading", }},
  {"cs", 4, {"onoff", "reading", "hi", "lo", }},
  {"re", 1, {"onoff", }},
};
 
/*flags extern data structure*/
flags_t f {
  0b00000,//HAYtIMR 1100000 64+32=96
  0b11111,//HAYpROG 11000010 =128+64+4=198
  0b00000,//dOrEC 
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

