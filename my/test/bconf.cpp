#include<stdio.h>
#include <string.h>
#include "bconf.h"

/*dev extern device variables*/  
char devid[9]="CYURD128";
char owner[254]="tim@sitebuilt.net";
char pwd[24]="geniot";
char mqtt_server[60]="sitebuilt.net";
char mqtt_port[6]="1884";

char xdevtpc[NUMXD][MAXDSTR] = 
{"CYURD006/srstate", "CYURD018/srstate"};

const tds_t tds[NUMTYP] ={
  {"difctrl", 5, {"onoff", "rdA", "rdB", "dif", "difgt0"}},
  {"tstat", 4, {"onoff", "reading", "setting", "dif"}},
  {"relay", 2, {"onoff", "tsec"}},
  {"ctrl", 3, {"bell", "reading", "setting"}},
  {"array", 8, {"a0", "a1", "a2", "a3", "a4", "a5", "a6", "a7"}},
};

srs_t srs[NUMSR] ={ //pow(2,sr) = HASpROG=0b0001011, HAStIMR =0b0001100,
  {0, D8, 0, {0, 67, 45, 9, 1}},//difctrl "tank-panel" prg
  {1, D7, 1, {1, 67, 70, 1}},//tstat "lr" prg
  {2, D6, 1, {0, 35, 41, 1}},//tstat "dr"
  {3, D5, 2, {0, 120}},//relay "door_strike" prg timr 32768 sec or ~9hrs
  {4, D4, 3, {0, 74, 130}},//hilimit "bbq" timr
  {5, -9, 4, {67, 65, 66, 68, 35}},//array "readings" (of readings)
  {6, -9, 4, {60, 15, 12, 8, 11, 2, 5, 12}}//array "currents" {of currents}
};

unsigned long stime[NUMSR] ;

char sensors[SENSTYPS][MAXSSTR] =
{"butn", "switch", "1-wire", "dht", "i2c", "spi"};

/*
{{D0}, "1-wire", 2, 
  {
    {2, {{0, 1}, {6,4}}},
    {1, {{0, 1}}}
  }
}
*/

inp_t inp[NUMINP] = {
  {{D0}, "butn", 1, 
    { {1, {{2, 2}}} }, 
    {1, 10} },
  {{D1}, "butn", 1, 
    { {1, {{2, 2}}} }, 
    {-1, -10} },
  {{D1}, "switch", 1,\
    { {1, {{3, 0}}} }, 
    {200} },
  {{D2}, "1-wire", 3, 
    { {2, {{0, 1}, {1, 1}}},  
      {1, {{5, 0}}}, 
      {2, {{5, 1}, {5, 2}}} }, 
    {} },
  {{D3}, "dht", 2, 
    { {1, {{5, 3}}}, 
      {1, {{5, 4}}} }, 
    {}  },                                               //
  {{SCL, SDA}, "i2c", 8, 
    { {1, {{6, 0}}}, 
      {1, {{6, 1}}}, 
      {1, {{6, 2}}}, 
      {1, {{6, 3}}}, 
      {1, {{6, 4}}}, 
      {1, {{6, 5}}}, 
      {1, {{6, 6}}}  }, 
    {}  }, // currents
  {{SCK, SDO, SDI, CS}, "spi", 1, 
    { {1, {{4, 1}}}  }, 
    {}  }                                                     // thermo
};

xdata_t xdata[NUMXD] = {
  {1, 0, {{{}},{{6, 7}}}},
  {0, 0, {{{0, 2}}} }
};

/*flags extern data structure*/
flags_t f {
  0,//cONNectd
  0,//hayWIFI
  0,//hayMQTT
  0,//aUTOMA
  0,//fORCErESET
  5,//cREMENT
  0b0001100,//HAStIMR 1100000 64+32=96
  0,//IStIMERoN
  198,//HAYpROG 11000010 =128+64+4=198
  0b1111111,//HAYsTATEcNG
  0,//CKaLARM
  0,//ISrELAYoN
  {0,0,0,0,0,0,0},//tIMElEFT[BITS]
  0b00100010,//dOrEC
  0b0001011//HASpROG
};


/*
uint8_t lr[8] = { 0x28, 0xFF, 0xD2, 0x80, 0xB1, 0x16, 0x03, 0xFF };


inputs
{sr, {port}, "butn", targets:{{{sr11, setting}}}, action:{+1,+10} butn +butn}//gets read, 
{sr, {port}, "butn", targets:{{{sr:11, "setting"}}}, action:{-1,-10} -butn}
{sr, {port}, "1-wire", targets:{{{sr, reading0}, {sr rdA}},{{sr, reading1}}, {{sr, reading}},{{sr, reading}}}, action:null, 1-wire, DS20B18 temp}
{sr, {port}, "dht11", target:{{{sr, reading},{sr, rdA}},{sr, humidity}}, action:null, DHT11 temp_attic}
{sr, {SCL,SDA}, "i2c", {{{sr6, reading}}}} //i2c ADS115
{sr, {SCK, SD0, SDI, CS[0]...}, "spi" {{{sr7, usage}}}}} //spi

/*srs data
{0, D4, {onoff, rdA, rdB, diff, dg}}, di   canprog dif
{1, D5, {onoff, reading, hi, lo}},// tstat setting = (h+lo)/2 canprog hi,lo
{2, D6, {onoff, reading, hi, lo}},// tstat setting = (h+lo)/2 canprog hi,lo
{5, D7, {onoff, tsec}}//switch? canprog onoff, tsec
{5, D7, {bell, reading=a[0], setting}}//thermo canprog setting, bell  
{6, D8, {onoff, readin=a[0], hi, lo}}tstat canprog hilo cancmd onoff
{7, -9, {a0,a1,a2,a3,a4 ...a15}}//currents for circuit mirror input data
*/

// ports_t ports[NUMSR]={
//   //port:{sr, in, out, type, xd, label rec, isnew}
//   {0, D2, -9, "se", -1, "temp2", 0, 0},// temp2 DS18B20
//   {1, D5, -1, "cs", -1, "temp", 0, 0},// temp DHT11
//   {2, D5, D7, "cs", -1, "hum", 0, 0},// hum DHT11
//   {3, -9, D6, "cr", -1, "timr", 0, 0},// hum DHT11
//   {4, D3, D1, "cr", -1, "btn1", 0, 0}, // btn1 NO-btn
//   {5, -10, -1, "se", 0, "temp_out", 0, 0},// {xsr, in-10=sr, grp-1, type, datxidx, label, rec, isnew}
//   {6, -1, D8, "di", -1, "out_reset", 0, 0},
//   {7, -9, -99, "di", 1, "solar", 0, 0 }
// };
/*proposed combination of cr&rel
 {4, D3|-9|-13, D1, "cr|rel", -1, "btn1|timr|xonoff", 0}, 
notice isnew now in f.iSnEW*/


// int xdata[NUMXD][MAXD+3]={//{xsr, devtpc, osr, data...}
//   {7, 1, 1, 0, 55, 44, 10, 1, 1},//solar di read only
//   {5, 0, 0, 56, -9, -9, -9, -9, -9}//temp_out se
// };




/*
some input ports provide multiple data
some multiple input ports provide just one data

const connections elucidates the interconnections





/*cmd
{id, darr[], tsec}//c++ figures out by sr what kind of data it is s
sr= cs=[hi,lo]
    rel=[onoff],tsec or [onoff]
*/

/*prog
{id, pro:[[]]} id tells you which sr gets prog copied to, cs gets the current data, ckALARMS get set 

program data mirrors const
input data
{0, D2}
{1, }
*/



/*
inputs
{sr, {port}, "butn", targets:{{{sr11, setting}}}, action:{+1,+10} butn +butn}//gets read, 
{sr, {port}, "butn", targets:{{{sr:11, "setting"}}}, action:{-1,-10} -butn}
{sr, {port}, "1-wire", targets:{{{sr, reading0}, {sr rdA}},{{sr, reading1}}, {{sr, reading}},{{sr, reading}}}, action:null, 1-wire, DS20B18 temp}
{sr, {port}, "dht11", target:{{{sr, reading},{sr, rdA}},{sr, humidity}}, action:null, DHT11 temp_attic}
{sr, {SCL,SDA}, "i2c", {{{sr6, reading}}}} //i2c ADS115
{sr, {SCK, SD0, SDI, CS[0]...}, "spi" {{{sr7, usage}}}}} //spi

when xdata comes in how do you know where to put it? from above or somewhere else?
xdata
{CYURD006, 0, target:{{{}}, {{sr, rdB}},{{}}, {{}}}, action:null, temp_out} //xdata
*/




/*
what if you wanted to use max3185 for a temp alarm?
what if you wanted know the boiler usage based upon which zones were on? it is not meaningful information instantaneously. You would want to record which zones were calling, wheter the boiler was on. all these things are recorded in the database.

The data is being compared every time the process inputs happens?? by setting flags??
*/

/*declarations
Adafruit_ADS1115 ads(0x48);
*/

/*
intialize //butn
f.iNPUTS = {0,0,0,0}
SHORTPRESS = 500ms
ANYPRESS =200
unsigned long pressedTime  = 0;
unsigned long releasedTime = 0;
*/



/*
Whwenever you write to data there is a HAYstatechange, 
{"btn", dsb1820, dhtll, }
void processInputs(){
  for(int i=0;i<MUMINP;i++){
    int sr= getInSr(i)
    int inp_hndlr=getInputHandler(i)) 
    switch inp_hndlr{
      case 0://btn
        //int getCrement()
        int crement;
        int port = input.port
        currentState = digitalRead(port;
        if(f.iNPUTS[i] == HIGH && currentState == LOW)        // button is pressed
          pressedTime = millis();
        else if(f.iNPUTS[i] == LOW && currentState == HIGH)  // button is released
          releasedTime = millis();
        long pressDuration = releasedTime - pressedTime;  
        if (presDuration< SHORTPRESS && pressDuration > ANYPRESS){
            crement = input.action[0]
        }else{
            crement = input.action[1]
        }
        //end getCrement()
        //{sr, {port}, targets:{{{sr11, daIdx}}}, action:{+1,+10} butn +butn}
        for(targes j){ //input data[j]
          for locs k{
            int osr = getOutIdx(input[i].target[j][k][0])
            int srIdx = getDataIdx(output[osr])
            int cv = srs[srIdx].data[input[i].target[j][k][1]]
            srs[srIdx].data[input[i].target[j][k][1]]= cv+crement; 
          }
          f.HayStateNG {o,o,1,o,o,o,}
        }
      case 1: //dsi8b20
      //{sr, {port}, "ds18b20", targets:{{{sr, reading0}, {sr rdA}},{{sr, reading1}}}, action:null, 1-wire, temp}
        for(targes j){ //input data[j]
          int reading= (int)DS18B20.getTempFByIndex(j);
          for locs k{
            int osr = getOutIdx(input[i].target[j][k][0])
            int srIdx = getDataIdx(output[osr])
            srs[srIdx].data[input[i].target[j][k][1]]= reading;
        }
      case 7: //ADS115
    }
  }
}
*/

/*srs data
void ckRelays() 
  for(int i=0; i<NUMSRS;i++){
    if(hayStChg){
      pubState(i)
    }
    int port = srs[i].port); 
    if (port >= 0){
      int onoff= srs[i].data[srsDatIdx(srsConf[i], "onoff")]
      if (digitalRead(port != onoff){
        digitalWrite(srs[i].port, onoff)
      }
    }
  }

const initialState = {
  //pro must start at 0,0
  temp2: { darr: reading:45} },
  temp: { darr: {reading:44, onoff:0, "hi":69, lo:67] },
  hum: { darr: {reading:24, onoff:0, hi:38, lo:23] },
  timr1: { darr: [0], pro: [{hr:0, min:0, onoff:0]], timeleft: 0},
  temp_out: { darr: {reading:55} },
  currents: {breaker1:0, breaker2:0}
}
*/

// ports_t ports[NUMSR]={
//   //port:{sr, in, out, type, xd, label rec, isnew}
//   {0, D2, -9, "se", -1, "temp2", 0, 0},// temp2 DS18B20
//   {1, D5, -1, "cs", -1, "temp", 0, 0},// temp DHT11
//   {2, D5, D7, "cs", -1, "hum", 0, 0},// hum DHT11
//   {3, -9, D6, "cr", -1, "timr", 0, 0},// hum DHT11
//   {4, D3, D1, "cr", -1, "btn1", 0, 0}, // btn1 NO-btn
//   {5, -10, -1, "se", 0, "temp_out", 0, 0},// {xsr, in-10=sr, grp-1, type, datxidx, label, rec, isnew}
//   {6, -1, D8, "di", -1, "out_reset", 0, 0},
//   {7, -9, -99, "di", 1, "solar", 0, 0 }
// };

/*
loop(){
  unsigned long inow = millis();  
  updSensors(inow))

  readSwitch(inow)
  get prior state
  read current state
  if switch is changing from off to on & tmsec>0
    set start for this button
  if sitch is on
    read start
    if (isnow - start) > tmsec
      turn off switch
 


}

switchdata {{}}
*/
