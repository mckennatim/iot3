
#ifndef CONFIG_h
#define CONFIG_h

#include <TimeLib.h>
#include <TimeAlarms.h>
#include <ESP8266WebServer.h>

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
extern const topics_t tpcsSub ;

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
