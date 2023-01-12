#ifndef CONFIG_h
#define CONFIG_h

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

#endif  