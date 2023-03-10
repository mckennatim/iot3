#ifndef aconf_h
#define aconf_h

#include<stdio.h>
#include <string.h>

#define sizeOf(a) (sizeof(a) / sizeof(a[0]))
/*project constants*/
#define NUMXD 2 //number of external data sources
#define NUMSR 8 //number of sr's (also port entries)
#define NUMTYP 5 //number of types
#define MAXD 6  //max number of data in an sr
/*END proj constants*/

extern char devid[9];
extern char owner[254];
extern char pwd[24];
extern char mqtt_server[60];
extern char mqtt_port[6];

/*ports*/
struct ports_t {
  int sr;
  int in;
  int out;
  char type[8];
  int xd;
  char label[12];
  int rec;
  int isnew;
};
extern ports_t ports[] ;
/*END ports*/

/*type and data labels*/
struct tds_t {
  char type[8];
  int numdl;
  char dats[MAXD][10];
};
extern const tds_t tds[];
/*END type and data labels*/

/*external device/topic*/
// struct xdevtpc_t {
extern char xdevtpc[][20];

/*external data*/
extern int xdata[][MAXD+3];
/*END external data*/


extern char itopic[40];
extern char ipayload[256];
extern char idev[17];
extern unsigned int ipaylen;
extern bool NEW_MAIL;

/*predefined */
#define D0 16
#define D1 5
#define D2 4
#define D3 0
#define D4 2
#define D5 14
#define D6 12
#define D7 13
#define D8 8
#define RX 3
#define TX 1
#define A0 0

#endif