#include "CONFIG.h"

bool haywifi=true;
char devid[9]="CYURD127";
char owner[254]="mckenna.tim@gmail.com";
char pwd[24]="geniot";
char mqtt_server[60]="sitebuilt.net";
char mqtt_port[6]="1884";

/*INCOMING const mqtt topics*/
const topics_t TPC {
  5,
  {"devtime", "cmd", "prg", "req", "set"}
};

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
  0,//hayWIFI
  0,//hayMQTT
  0,//cONNectd
  {0,0,0,0,0,0,0,0,0,0}//tIMElEFT[10]
};