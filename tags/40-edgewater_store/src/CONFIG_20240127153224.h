
#ifndef conf_h
#define conf_h

#include<stdio.h>
#include <string.h>
#include <TimeLib.h>
#include <TimeAlarms.h>
#include <ESP8266WebServer.h>
#define sizeOf(a) (sizeof(a) / sizeof(a[0]))


void c_initShit();

 
/*project constants for CYURD127edgewater*/
#define NUMXDEV 0 //# of x devices
#define NUMXD 0 //# of xdata records
#define NUMSR 5 //number of sr's 
#define NUMINP 1
#define NUMPRGS 1
#define MAXDT 0
#define SENSTYPS 7
#define NUMTYP 8

 
/*hacks for now*/
#define BTNPRT D3
#define BTNPRT2 D4
#define WIR0PRT D2 

/*constants that proabaly wont change*/
#define MAXD 8 //max number of data in an sr
#define MAXTARGETS 4  //max number of data in an sr
#define MAXSENSED 8 //max number onf(1-wire) sensors
#define MAXSSTR 8 //str len like '1-wire' 
#define MAXDSTR 20 //str len CYURD118/srtstae
#define MAXINPORTS 5 //maxnum ports req for an input SPI,SCL etc
#define NPRGDA 3 //like {hr,min,setting}
#define NUMTOPICS 5 //like 'cmd' etc
#define MAXEV 10 //max prg events

/*END proj constants*/


extern char devid[9];
extern char owner[254];
extern char pwd[24];
extern char mqtt_server[60];
extern char mqtt_port[6];

/*external device/topic*/
// struct xdevtpc_t {
extern const char xdevtpc[][MAXDSTR];

/*type and data labels*/
struct tds_t {
  char type[9];
  int numdl;
  char dats[MAXD][10];
};
extern const tds_t tds[];
/*END type and data labels*/

/*srs*/
struct srs_t {
  int relay;
  int port;
  int typidx;
  int data[MAXD];
};
extern srs_t srs[];

/*cmds*/
struct cmd_t {
  int srs;
  int nda;
  int data[MAXD];
};
extern const cmd_t cmds[];

/*START prg data structure declarations*/  
struct prgs_t{
  int sr;
  int dax;
  AlarmID_t aid;
  int ev;
  int prg[MAXEV][NPRGDA];//max 11 events [hr,min,max,min]  
  int hms;
};
extern prgs_t prgs[];
/*END prg data structure declarations*/  

extern unsigned long sTRTsWtIMR[];
extern unsigned long tIMElEFT[];
extern unsigned long lONGpRESStIMR[] ;
extern int bUTNpREV[] ;


extern const char sensors[][MAXSSTR];

struct targs_t{
  int numtargs;
  int gets[MAXTARGETS][2];
}; 

struct action_t {
  int sval;
  int lval;
  int prev;
  int lptime;
};

struct inp_t {
  int port[MAXINPORTS];
  char type[MAXSSTR];
  int numreadings;
  targs_t tar[MAXSENSED];
  action_t actions; //+1,+10,prev,sTRTsWtIMR,lp
};
extern inp_t inp[];

struct dt_t {
  int xdidx;
  targs_t targs;
};

struct xdata_t {
  int dev;
  int sr;
  int nd;
  dt_t dt[MAXDT];
};
extern const xdata_t xdata[];

/*INCOMING topics*/
extern const char subTopics[][MAXSSTR];

/*flags*/
struct flags_t{
  int HAYtIMR; //11100(28) 4,8, and 16 have timers not temp
  int HAYpROG;// = senrels with events>1
  int dOrEC;
  int cONNectd;
  int hayWIFI;
  int hayMQTT;
  bool aUTOMA;
  bool fORCErESET;  
  int cREMENT;
  int IStIMERoN;//11100 assume some time left, timers with tleft>0 
  int HAYsTATEcNG; //11111(31 force report) some state change int or ext
  int CKaLARM; //11111 assume alarm is set at start
  int ISrELAYoN;// = summary of relay states  
  int iSnEW;
  int dOpUBLISH;
};
extern flags_t f;

/*D1_mini typical port names*/
#define RX 3
#define TX 1
#define A0 0
#define SCL D1
#define SDA D2
#define SCK D5
#define SDO D6
#define SDI D7
#define CS D8

#endif
