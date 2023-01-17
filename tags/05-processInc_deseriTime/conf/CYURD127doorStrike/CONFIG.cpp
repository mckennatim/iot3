#include "CONFIG.h"
//CONFIG_CYURD127strike.cpp
/*INSTRUCTIONS
* be sure to modify FLAGS for HAYpROGS and HAYtIMRS
*/

/*CUSTOM CODE*/ 
void customInit(){
  pinMode(D8, OUTPUT);
  digitalWrite(D8, HIGH);
}
void customLoop(){
  int bef = digitalRead(D8);
  if(srs.ti[0].onoff==1 || srs.ti[1].onoff==1 || srs.ti[2].onoff==1){
    digitalWrite(D8, HIGH);
  }else{
    digitalWrite(D8, LOW);
  }
  int aft = digitalRead(D8);
  if(aft != bef){
    Serial.print("D8 is ");
    Serial.println(aft);
  }
}


/*DESCRIPTION
Cascada 
Pond D5 OUT when on acivates lohi water circuit+relay
  and pond pumps relay (120V)
Garden D6 & D7 OUT irrigation activate (24V)
Transformer D8 activated when either D5,D6 or D7 hi
hay NO sensors connected to esp8266
CONFIG_CYURD116cascada running on espboth/secsti
has CUSTOM CODE
*/

/*LOCID 10-12ParleyVale
*/


/*SERVER
dev extern device variables*/
bool haywifi=true;
char devid[9]="CYURD126";
char owner[254]="tim@sitebuilt.net";
char pwd[24]="geniot";
char mqtt_server[60]="sitebuilt.net";
char mqtt_port[6]="1884";
/*dev*/

/*SPECS
CONFIG extern const device variables
INCOMING const mqtt topics*/
const topics_t TPC {
  5,
  {"devtime", "cmd", "prg", "req", "set"}
};
/*PORTS for INPUT*/
const portsin_t inpo {
  0, //DS18b20a
  0, //DS18b20b
  0,//dht11
  0, //ANALOG
  0, //SPIdo
  0, //SPIcs
  D2, //inp1
  0};//inp2
/*SE constant declarations*/  
const sen_t SE {
  1,//number of different sensor types
  1,//number of sensors(numsens)
  {
    {1,{3},"digin", "inp1"}
  }
};
/*------------------------------------------------------
CONFIG extern structures (initial values, changeable)*/
/*srs extern data structure initalization
state of relays and sensors */ 
srs_t srs {
  5,//numsr
  2,//sumse
  {{3,0,1,0},{4,1,1,0}},
  0,//numcs
  {},
  3,//numti
  { //{sr,onoff,rec,isnew}
    {0,0,1,0},{1,0,1,0},{2,0,1,0}
  },
  0,//numdi difference control
  {}
};
/*prgs extern data structure initalization*/ 
prgs_t prgs{
  3,//numprgs
  { //sr,aid,ev,numdata,prg[[]],port,hms
    {0,255,1,1,{{0,0,0}},D5,1506}, //pond
    {1,255,1,1,{{0,0,0}},D6,1504}, //hi_bed
    {2,255,1,1,{{0,0,0}},D7,1503}  //lo_bed
  }
};
/*flags extern data structure*/
flags_t f {//MODIFY HAYpROG and HAStIMR
  0,//aUTOMA
  0,//fORCErESET
  5,//cREMENT
  7,//HAStIMR 00000111 =1+2+4=7
  0,//IStIMERoN
  7,//HAYpROG 00000111 =1+2+4=7
  1023,//HAYsTATEcNG
  0,//CKaLARM
  0,//ISrELAYoN
  {0,0,0,0,0,0,0,0,0,0}//tIMElEFT[10]
};

// nxtalarr_t nxtal {
//   0,//num
//   {//sr,aid,hms (h*60+m)*20+sr
//     {0,255,29000},
//     {1,255,29000},
//     {2,255,29000},
//     {3,255,29000},
//     {99,255,29001},
//     {99,255,29002},
//     {99,255,29006},
//     {99,255,29004},
//     {99,255,29000},
//     {99,255,29000}
//   }
// };
    

// const srpos_t outpos {
//   3, //numpos
//   {{2,D0}, {7,D3}, {8,D4}} //pod.sr,pod.portnum
// };
