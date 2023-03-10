#include<stdio.h>
#include <string.h>
#include "aconf.h"

/*dev extern device variables*/  
char devid[9]="CYURD128";
char owner[254]="tim@sitebuilt.net";
char pwd[24]="geniot";
char mqtt_server[60]="sitebuilt.net";
char mqtt_port[6]="1884";

char xdevtpc[NUMXD][20] = 
{"CYURD006/srstate", "CYURD018/srstate"};

ports_t ports[NUMSR]={
  //port:{sr, in, out, type, xd, label rec, isnew}
  {0, D2, -9, "se", -1, "temp2", 0, 0},// temp2 DS18B20
  {1, D5, -1, "cs", -1, "temp", 0, 0},// temp DHT11
  {2, D5, D7, "cs", -1, "hum", 0, 0},// hum DHT11
  {3, -9, D6, "rel", -1, "timr", 0, 0},// hum DHT11
  {4, D3, D1, "cr", -1, "btn1", 0, 0}, // btn1 NO-btn
  {5, -10, -1, "se", 0, "temp_out", 0, 0},// {xsr, in-10=sr, grp-1, type, datxidx, label, rec, isnew}
  {6, -1, D8, "di", -1, "out_reset", 0, 0},
  {7, -9, -99, "di", 1, "solar", 0, 0 }
};

int xdata[NUMXD][MAXD+3]={//{xsr, devtpc, osr, data...}
  {7, 1, 1, 0, 55, 44, 10, 1, 1},//solar di read only
  {5, 0, 0, 56, -9, -9, -9, -9, -9}//temp_out se
};

const tds_t tds[NUMTYP] ={
    {"se", 1, {"reading"}},
    {"cs", 4, {"reading", "onoff", "hi", "lo"}},
    {"cr", 2, {"onoff", "tsec"}},
    {"rel", 1, {"onoff"}},
    {"di", 6, {"onoff", "rdA", "rdB", "dif", "hyst", "gd"}},
};