
#include "CONFIG.h"

void customInit(){
  if(0==1){
    ;
  }
};
void customLoop() {
  if(0==1){
    ;
  }
};


/*dev extern device variables*/  
char devid[9]="CYURD128";
char owner[254]="tim@sitebuilt.net";
char pwd[24]="geniot";
char mqtt_server[60]="sitebuilt.net";
char mqtt_port[6]="1884";

/*SPECS
CONFIG extern const device variables
INCOMING const mqtt topics*/
const topics_t TPC {
  5,
  {"devtime", "cmd", "prg", "req", "set"}
};

/*ports for input and output
 */
const ports_t ports {
  4, //numsr
  {//port:{sr, in, out, rec, isnew}
    {0, D2, -9, 1, 0},// temp2 
    {1, D5, D8, 1, 0},// temp 
    {2, D5, D7, 1, 0},// hum 
    {3, -9, D6, 0, 0} // timr1 
  }
};
/*SE constant declarations*/
const sen_t SE {
  2, // numstypes 
  3, // numsens 
  { // stype:{nums,{sra,srb...},type,model}
    { 1, { 0 }, "temp", "DS18B20" }, 
    { 2, { 1,2 }, "temp-hum", "DHT11" }  
  }
};

/*srs data structure to hold the current state of the entire device*/
srs_t srs {
  4, // numsr 
  1, // numse 
  { // se:{sr, reading} 
    {0, 45}  // temp2
  },
  2, // numcs 
  { // cs:{sr, reading, onoff, hi, lo} 
    {1, 44, 0, 69, 67}, // temp
    {2, 24, 0, 60, 90}  // hum
  },
  1, // numrel 
  { // rel:{sr, onoff} 
    {3, 0}  // timr1
  },
  0, // numdi 
  {} // dif:{sr, sra, srb, diffon, diffoff, maxa, maxb, onoff} 
};

/*prgs extern data structure initalization*/ 
prgs_t prgs{  
  0, // numprgs 
  {}
};

/*flags extern data structure*/
flags_t f {
  0,//cONNectd
  0,//hayWIFI
  0,//hayMQTT
  0,//aUTOMA
  0,//fORCErESET
  5,//cREMENT
  96,//HAStIMR 1100000 64+32=96
  0,//IStIMERoN
  198,//HAYpROG 11000010 =128+64+4=198
  1023,//HAYsTATEcNG
  0,//CKaLARM
  0,//ISrELAYoN
  {0,0,0,0,0,0,0,0,0,0}//tIMElEFT[10]
};
