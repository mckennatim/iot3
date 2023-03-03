
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

/*ports for input and output*/
ports_t ports {
  7, //numsr
  {//port:{sr, in, out, rec, isnew}
    {0, D2, -9, 1, 0},// temp2 DS18B20
    {1, D5, D8, 1, 0},// temp DHT11
    {2, D5, D7, 1, 0},// hum DHT11
    {3, -9, D6, 0, 0},// timr1 undefined
    {4, -9, D6, 0, 0},// ledBlue undefined
    {5, D0, D4, 1, 0},// lr DS18B20
    {6, D0, D1, 1, 0} // mb DS18B20
  }
};
/*SE constant declarations*/
const sen_t SE {
  3, // numstypes 
  5, // numsens 
  { // stype:{nums,{sra,srb...},type,model}
    { 1, { 0 }, "temp", "DS18B20" }, 
    { 2, { 1,2 }, "temp-hum", "DHT11" }, 
    { 2, { 5,6 }, "temp", "DS18B20" }  
  }
};

/*prgs extern data structure initalization*/ 
prgs_t prgs{  
  3, // numprgs 
  { // prg:{sr,aid,ev,numdata,prg[[]],hms} 
    {3, 255, 1, 1, {{0,0,0,}}, 1500},  
    {5, 255, 1, 2, {{0,0,69,67}}, 1502},  
    {6, 255, 1, 2, {{0,0,70,40}}, 1504}   
  }
};

/*flags extern data structure*/
flags_t f {
  0,//cONNectd
  0,//hayWIFI
  0,//hayMQTT
  0,//aUTOMA
  0,//fORCErESET
  5,//cREMENT
  0,//HAStIMR 1100000 64+32=96
  0,//IStIMERoN
  198,//HAYpROG 11000010 =128+64+4=198
  1023,//HAYsTATEcNG
  0,//CKaLARM
  0,//ISrELAYoN
  {0,0,0,0,0,0,0,0,0,0}//tIMElEFT[10]
};
