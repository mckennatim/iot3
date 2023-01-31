#ifndef CONFIG_h
#define CONFIG_h

#include <TimeLib.h>
#include <TimeAlarms.h>
#include <ESP8266WebServer.h>

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
    int hilimit;
    int lolimit;
    bool rec;
    bool isnew;   
};
struct ti_t {//timers
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
    se_t se[2];/*MODIFY*/
    int numcs;
    cs_t cs[1];/*MODIFY*/
    int numti;
    ti_t ti[3];/*MODIFY*/
    int numdi;
    di_t di[0];/*MODIFY*/
};
extern srs_t srs;
/*srs data structure declarations*/  

/*prg data structure declarations*/  
  struct prg_t{
    int sr;
    AlarmID_t aid;
    int ev;
    int numdata;
    int prg[11][4];//max 11 events [hr,min,max,min]  
    int port;
    int hms;
  };
  struct prgs_t{
    int numprgs;
    prg_t prg[1];/*MODIFY*/
  };
  extern prgs_t prgs;
   /*prg data structure declarations*/  

/*flags*/
struct flags_t{
bool aUTOMA;
bool fORCErESET;  
int cREMENT;
int HAStIMR; //11100(28) 4,8, and 16 have timers not temp
int IStIMERoN;//11100 assume some time left, timers with tleft>0 
int HAYpROG;// = senrels with events>1
int HAYsTATEcNG; //11111(31 force report) some state change int or ext
int CKaLARM; //11111 assume alarm is set at start
int ISrELAYoN;// = summary of relay states  
bool hayWIFI;
bool hayMQTT;
bool cONNectd;
int tIMElEFT[10];// =[0,0,56,0,0] timeleft in timrs
};
extern flags_t f;

struct iscsidx_t {
	int srtype;
	int idx;
};

#endif  