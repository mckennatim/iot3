
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
  7, //numsr
  {//port:{sr, in, out, rec, isnew}
    {0, D5, -9, 1, 0},// contact 
    {1, -9, D1, 1, 0},// strike 
    {2, -9, D8, 0, 0},// ledRed 
    {3, -9, D7, 0, 0},// ledGreen 
    {4, -9, D6, 0, 0},// ledBlue 
    {5, D0, D4, 1, 0},// lr 
    {6, D0, D1, 1, 0},// mb 
  }
}
/*SE constant declarations*/
const sen_t SE {
  2, // numstypes 
  3, // numsens 
  { // stype:{nums,{sra,srb...},type,model}
    { 1, { 0 }, "onoff", "NCcontact" }, 
    { 2, { 5,6 }, "temp", "ds18b20" }  
  }
}
/*srs data structure to hold the current state of the entire device*/
const srs_t srs {
  7, // numsr 
  1, // numse 
  { // se:{sr, reading} 
    {0, 1}  // contact
  },
  2, // numcs 
  { // cs:{sr, reading, onoff, hi, lo} 
    {5, 44, 0, 69, 67}, // lr
    {6, 33, 0, 70, 40}  // mb
  },
  4, // numrel 
  { // rel:{sr, onoff} 
    {1, 0}, // strike
    {2, 1}, // ledRed
    {3, 0}, // ledGreen
    {4, 0}  // ledBlue
  },
  0, // numdi 
  {}, // dif:{sr, onoff}
}
/*prgs extern data structure initalization*/ 
prgs_t prgs{  
  3, // numprgs 
  { // prg:{sr,aid,ev,numdata,prg[[]],hms} 
    {1, 255, 1, 1, {{0,0,0,}}, 1500},  
    {5, 255, 1, 2, {{0,0,69,67}}, 1502},  
    {6, 255, 1, 2, {{0,0,70,40}}, 1504}   
  }
}

/*flags extern data structure*/
flags_t f {
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
