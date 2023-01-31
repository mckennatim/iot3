#ifndef CONFIG_h
#define CONFIG_h

  #include <TimeLib.h>
  #include <TimeAlarms.h>
  #include <ESP8266WebServer.h>
  //CONFIG_CYURD127strike.h
  // #include <Arduino.h>//if CUSTOM CODE

  /*INSTRUCTIONS
  MODIFY this file where indicated for srs_t , prgs_t
  */
	
  #define sizeOf(a) (sizeof(a) / sizeof(a[0]))

  void customInit();
  void customLoop();

  /*dev */
  extern bool haywifi;
  extern char devid[9];
	extern char owner[254];
	extern char pwd[24];
	extern char mqtt_server[60];
	extern char mqtt_port[6];
  /*dev */

  /*PORT*/
  struct port_t{
    uint8_t pid;
    int io;
  }

  /*PORTS*/
  struct ports_t{
    port_t port[4]; 
  }
  extern ports_t ports;

  /*SE constant declarations*/  
  struct senso_t{
    int nums;
    int ids[8]; //srs/portin
    char type[12];
    char model[12];
  };
  struct sen_t {
    int numtypes;
    int numsens;
    senso_t stype[8];
  };
  extern const sen_t SE;
  /*SE constant declarations*/ 

  /*INCOMING topics*/
  struct topics_t {
    int numtopics;
    char scribedTo[6][8]; 
  };
  extern const topics_t TPC ;
  /*INCOMING topics*/

  /*PORTS for INPUT*/
  struct portsin_t{
    uint8_t DS18B20a;
    uint8_t DS18B20b;
    uint8_t Dht11;
    uint8_t ANNALOG;
    uint8_t SPIdo;
    uint8_t SPIcs;//d8
    uint8_t SPIcl;
    uint8_t inp1;
    uint8_t inp2;
  };
  extern const portsin_t inpo;
  /*PORTS for INPUT*/

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
    se_t se[1];/*MODIFY*/
    int numcs;
    cs_t cs[0];/*MODIFY*/
    int numti;
    ti_t ti[0];/*MODIFY*/
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
    prg_t prg[0];/*MODIFY*/
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
    int tIMElEFT[10];// =[0,0,56,0,0] timeleft in timrs
  };
  extern flags_t f;

struct iscsidx_t {
	int srtype;
	int idx;
};

// struct nxtal_t{
//   int sr;
//   AlarmID_t aid;
//   int hms;
// };
// struct nxtalarr_t{
//   int num;
//   nxtal_t arr[10];  
// };

// extern nxtalarr_t nxtal;

//ESP8266 builtin port constants
// static const uint8_t SDA = 4;
// static const uint8_t SCL = 5;

// static const uint8_t LED_BUILTIN = 2;
// static const uint8_t BUILTIN_LED = 2;

// static const uint8_t D0 = 16;
// static const uint8_t D1 = 5;
// static const uint8_t D2 = 4;
// static const uint8_t D3 = 0;
// static const uint8_t D4 = 2;
// static const uint8_t D5 = 14;
// static const uint8_t D6 = 12;
// static const uint8_t D7 = 13;
// static const uint8_t D8 = 15;
// static const uint8_t RX = 3;
// static const uint8_t TX = 1;


  // struct astate_t{
  //   int id;
  //   int stsz;
  //   int stdata[3];
  //   bool state; 
  //   bool rec;
  // };


  // /*OUTPUT ports*/
  // struct srpo_t{
  //   int sr;
  //   uint8_t portnum;
  // };

  // struct srpos_t {
  //   int numpos;
  //   srpo_t pod[3];
  // };

  // extern const srpos_t outpos;
  // /*OUTPUT ports*/

  // struct ctrld_sensor_t{
  //   int id;
  //   char type[12];
  //   char model[12];
  //   int inport;
  //   int outport;
  //   prg_t prgo;
  // };

  // struct timr_t{
  //   int id;
  //   char type[12];
  //   int outport;
  //   prg_t prgo;  
  // };
#endif

