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