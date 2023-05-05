
#ifndef CONFIG_h
#define CONFIG_h

#include <TimeLib.h>
#include <TimeAlarms.h>
#include <ESP8266WebServer.h>
#include <PubSubClient.h>

extern PubSubClient client;

void customInit();
void customLoop();

/*dev */
extern bool haywifi;
extern char devid[9];
extern char owner[254];
extern char pwd[24];
extern char mqtt_server[60];
extern char mqtt_port[6];


/*INCOMING topics*/
struct topics_t {
    int numtopics;
    char scribedTo[6][8]; 
};
extern const topics_t TPC ;

/*PORTS*/
struct port_t {
  int sr;
  int in;
  int out;
  int rec;
  int isnew;
};
struct ports_t {
  int numports;
  port_t port[5]; /*MODIFY*/
};
extern ports_t ports ;
/*PORT*/

/*START SE constant declarations*/  
struct senso_t{
  int nums;
  int ids[6]; //srs/portin
  char type[12];
  char model[12];
};
struct sen_t {
  int numtypes;
  int numsens;
  senso_t stype[3];
};
extern const sen_t SE;
/*END SE constant declarations*/ 

/*START srs data structure declarations*/ 
struct se_t {//sensors
    int sr;
    int reading;
};
struct cs_t {//controlled sensors
  int sr;
  int reading;
  bool onoff;
  int hi;
  int lo;
};
struct cr_t {//controlled relay
  int sr;
  int tsec;
  bool onoff;
};
struct rel_t {//timers
    int sr;
    bool onoff;
};
struct di_t {//diff control
    int sa;
    int sb; 
    int don;
    int doff;
    int maxa;
    int maxb;
    bool onoff;
};
struct srs_t {
  int numsr;
  int numse;
  se_t se[1];/*MODIFY*/
  int numcs;
  cs_t cs[2];/*MODIFY*/
  int numcr;
  cr_t cr[1];/*MODIFY*/
  int numrel;
  rel_t rel[1];/*MODIFY*/
  int numdi;
  di_t di[0];/*MODIFY*/
};
extern srs_t srs;
/*END srs data structure declarations*/  

/*START xdata structure declarations*/
struct xda_t {
  char xdev[17];
  srs_t xrs;
};
struct xdata_t {
  int numdevs;
  xda_t xda[1];
};
extern xdata_t xdata;
/*END xdata structure declarations*/


/*START prg data structure declarations*/  
struct prg_t{
  int sr;
  AlarmID_t aid;
  int ev;
  int numdata;
  int prg[11][4];//max 11 events [hr,min,max,min]  
  int hms;
};
struct prgs_t{
  int numprgs;
  prg_t prg[2];/*MODIFY*/
};
extern prgs_t prgs;
/*END prg data structure declarations*/  

/*flags*/
struct flags_t{
  int cONNectd;
  int hayWIFI;
  int hayMQTT;
  bool aUTOMA;
  bool fORCErESET;  
  int cREMENT;
  int HAStIMR; //11100(28) 4,8, and 16 have timers not temp
  int IStIMERoN;//11100 assume some time left, timers with tleft>0 
  int HAYpROG;// = senrels with events>1
  int HAYsTATEcNG; //11111(31 force report) some state change int or ext
  int CKaLARM; //11111 assume alarm is set at start
  int ISrELAYoN;// = summary of relay states  
  int tIMElEFT[10];// =[0,0,56,0,0] timeleft in timrs
};
extern flags_t f;

struct iscsidx_t {
  int srtype;
  int idx;
};

extern char itopic[40];
extern char ipayload[256];
extern char idev[17];
extern unsigned int ipaylen;
extern bool NEW_MAIL;

#endif
