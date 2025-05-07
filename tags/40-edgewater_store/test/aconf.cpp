#include<stdio.h>
#include <string.h>
#include "aconf.h"

/*dev extern device variables*/  
char devid[9]="CYURD128";
char owner[254]="tim@sitebuilt.net";
char pwd[24]="geniot";
char mqtt_server[60]="sitebuilt.net";
char mqtt_port[6]="1884";

char xdevtpc[NUMXD][20] = 
{"CYURD006/srstate", "CYURD018/srstate"};

ports_t ports[NUMSR]={
  //port:{sr, in, out, type, xd, label rec, isnew}
  {0, D2, -9, "se", -1, "temp2", 0, 0},// temp2 DS18B20
  {1, D5, -1, "cs", -1, "temp", 0, 0},// temp DHT11
  {2, D5, D7, "cs", -1, "hum", 0, 0},// hum DHT11
  {3, -9, D6, "cr", -1, "timr", 0, 0},// hum DHT11
  {4, D3, D1, "cr", -1, "btn1", 0, 0}, // btn1 NO-btn
  {5, -10, -1, "se", 0, "temp_out", 0, 0},// {xsr, in-10=sr, grp-1, type, datxidx, label, rec, isnew}
  {6, -1, D8, "di", -1, "out_reset", 0, 0},
  {7, -9, -99, "di", 1, "solar", 0, 0 }
};
/*proposed combination of cr&rel
 {4, D3|-9|-13, D1, "cr|rel", -1, "btn1|timr|xonoff", 0}, 
notice isnew now in f.iSnEW*/
srs_t srs[NUMSR] ={
  {0, 0, {67}},
  {1, 1, {67, 0, 71, 68}},
  {2, 2, {35, 0, 54, 45}},
  {3, 3, {0}},
  {4, 3, {0, 2}},
  {5, 0, {67}},
  {6, 6, {0, 67, 65, 10, 1, 1}},
  {7, 7, {67}}
};

int xdata[NUMXD][MAXD+3]={//{xsr, devtpc, osr, data...}
  {7, 1, 1, 0, 55, 44, 10, 1, 1},//solar di read only
  {5, 0, 0, 56, -9, -9, -9, -9, -9}//temp_out se
};

const tds_t tds[NUMTYP] ={
    {"se", 1, {"reading"}},
    {"cs", 4, {"reading", "onoff", "hi", "lo"}},//tstat controlled by sensor
    {"cr", 2, {"onoff", "tsec"}},//controlled relay if tsec=0=infinity
    {"di", 6, {"onoff", "rdA", "rdB", "dif", "hyst", "gd"}},
};




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
srs data config
{0, port, {rdA:temp_attic, rdB:temp_out, diff dg} attic_fan}
{1, port, {onoff, reading0 setting, dif}, tstat}}
{sr, port, {onoff, reading1 setting, dif}, tstat}}
{sr, port, {onoff, tsec}, switch}}
{sr, port, {bell, reading, setting}, turkey}}
{sr, -9, {ckt:a0,ckt2:a1,ckt3:a2,ckt4:a3,ckt5:a4 ...a15}} //i2c
{sr, -9, {a[0]...}} //spi
{sr, -9, {reading, reading}}
*/

/*srs data
{0, D4, {onoff, rdA, rdB, diff, dg}}, di   canprog dif
{1, D5, {onoff, reading, hi, lo}},// tstat setting = (h+lo)/2 canprog hi,lo
{2, D6, {onoff, reading, hi, lo}},// tstat setting = (h+lo)/2 canprog hi,lo
{5, D7, {onoff, tsec}}//switch? canprog onoff, tsec
{5, D7, {bell, reading=a[0], setting}}//thermo canprog setting, bell  
{6, D8, {onoff, readin=a[0], hi, lo}}tstat canprog hilo cancmd onoff
{7, -9, {a0,a1,a2,a3,a4 ...a15}}//currents for circuit mirror input data

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