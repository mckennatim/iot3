#include "Inp.h"
#include "CONFIG.h"
#include "Util.h"
#include "Reqs.h"
#include <DallasTemperature.h>
#include <DHT.h>
#include <ezButton.h>

ezButton button(D3);
OneWire Wire[2] ={OneWire(inp[0].port[0]), OneWire(D5)}; 
DallasTemperature DS18B20[2] = {DallasTemperature(&Wire[0]), DallasTemperature(&Wire[1])};
boolean Active1Wire[2]= {1,0};
int readDs18b20(int ix, int re){
  int bus = inp[ix].actions.sval;
  int temp = (int)DS18B20[bus].getTempFByIndex(re);
  return temp;
}

void i_butnLoop(){
  button.loop();
}

int readSenseSpi(int ix, int re){
  return -99; //rand()%(60-5 + 1) + 5;
}

DHT dht[1] ={{D5, DHT11}};
void sensorInit(){
  for (auto& sensor : dht) {
    sensor.begin();
  }
  button.setDebounceTime(50);
}
void sensorInit();
int readSenseDht(int ix, int re){
  int bus = inp[ix].actions.sval;
  int read;
  if (re==0){
    read = (int)dht[bus].readTemperature(true);
  }else{
    read =(int)dht[bus].readHumidity(); 
  }
  return read;
}

int readSenseI2c(int ix, int re){
  return -99;// rand()%(60-5 + 1) + 5;
}

int readToggle(int ix, int re){
  int port = inp[ix].port[0];
  return !digitalRead(port);
}

int readSwitch(int ix, int re){
  int butnState = button.getState();
  int bst = 0;
  if(button.isPressed()){
    bst=1;
    Serial.println("The button is pressed");
  }
  if(button.isReleased()){
    bst=2;
    Serial.println("The button is Released");
  }
  printf("bst:%d,butnState=%d, pressed:%d, released:%d \n",bst, butnState, button.isPressed(), button.isReleased()) ;
  return bst;
  // int port = inp[ix].port[0];
  // return digitalRead(port);
}

int readButton(int ix, int re){
  int port = inp[ix].port[0];
  int prev = bUTNpREV[ix];
  unsigned long int lptime = inp[ix].actions.lptime;
  // int shlo[] = [inp[ix].actions.lval;
  int lval = inp[ix].actions.lval;
  int sval = inp[ix].actions.sval;
  int retval = 0;
  if (debounce(port)) {
    digitalWrite(port, !digitalRead(port));
  }
  int down =  digitalRead(port);
  if (down & (prev==0)){ //press button, start long press timer
    lONGpRESStIMR[ix] = millis(); //CNG
    bUTNpREV[ix] = 1;
  } 
  if ((!down) & (prev)){ //release button, calc time pressed -> nval = lval|sval
    unsigned long int now = millis();//CNG
    unsigned long int dif = now - lONGpRESStIMR[ix];
    bUTNpREV[ix] = 1;
    printf("button dif = %ld \n", dif);
    if(dif > lptime){
      retval = lval;
    }else retval = sval;
  }
  return retval;
}

void i_updInputs(){
  for(int ix=0;ix<NUMINP;ix++){
    for(int sen=0;sen<SENSTYPS;sen++){
      if(strcmp(sensors[sen],inp[ix].type)==0){  
        switch (sen){
          case 0: //butn
            break;
          case 1: //switch
            break;
          case 2: //1-wire
          {
            int bus = inp[ix].actions.sval;
            DS18B20[bus].requestTemperatures();
            delay(1000);
            for (int k=0;k<inp[ix].numreadings;k++){
              int oval = getOldReading(inp[ix].tar[k].gets[0][0], inp[ix].tar[k].gets[0][1]);
              int temp = (int)DS18B20[bus].getTempFByIndex(k);
              // printf("k = %d, the temp is %d, oval is %d\n",k, temp, oval);
              for(int m=0;m<inp[ix].tar[k].numtargs;m++){
                int sr = inp[ix].tar[k].gets[m][0];
                int da = inp[ix].tar[k].gets[m][1]; //sr data index
                srs[sr].data[da]=temp;
                if(abs(temp-oval)>=1){
                  u_setFlag(sr, &f.HAYsTATEcNG);
                }
              }
            }
            break;
          }
          case 3: //dht
            break;
          case 4: //i2c
            break;
          case 5: //spi
            break;
          case 6: //toggle
          {
            button.getState();
            int bst = 0;
            if(button.isPressed()){
              bst=1;
              Serial.println("The button is pressed");
            }
            if(button.isReleased()){
              bst=2;
              Serial.println("The button is Released");
            }
            if(bst==1){
              for(int m=0;m<inp[ix].tar[0].numtargs;m++){
                int sr = inp[ix].tar[0].gets[m][0];
                int da = inp[ix].tar[0].gets[m][1]; //sr data index
                srs[sr].data[da] = !srs[sr].data[da];
                u_setFlag(sr, &f.HAYsTATEcNG);
              }
              break;
            }
          }  
          default:
            break;  
        }
      }
    }
  }
}



int getOldReading(int sr, int da){
  return srs[sr].data[da];
}

int debounce(int port) {
  static int state = 0;
  state = (state<<1) | digitalRead(port) | 0xfe00;
  printf("port:%d, reading:%d, state:%d, ==%d\n", port, digitalRead(port), state, state == 0xff00);
  return (state == 0xff00);
}

void i_updCtrl(int sr, int x){
  int typidx = srs[sr].typidx;
  switch (typidx){
    case 0: //diffctrl;
    {
      int conoff =  srs[sr].data[0];
      int onoff=0; 
      int cdif = abs(srs[sr].data[1]-srs[sr].data[2]);
      int dif = srs[sr].data[3];
      int difgt0 = srs[sr].data[4];
      if(cdif > dif){
        onoff = difgt0 ? 1 : 0;
      }else {
        onoff = difgt0 ? 0 : 1;
      }
      if(onoff!=conoff) srs[sr].data[0] = onoff;
      if(digitalRead(srs[sr].port)!=onoff) digitalWrite(srs[sr].port, onoff);
      if (onoff==1) u_setFlag(sr, &f.ISrELAYoN);
      break;
    }   
    case 1: //tstat
    {
      int conoff =  srs[sr].data[0];
      int onoff=0; 
      int reading = srs[sr].data[1];
      int setting = srs[sr].data[2];
      int dif = srs[sr].data[3];
      if(reading<(setting-dif)) onoff = 1;
      if(reading>(setting+dif)) onoff = 0;
      if(conoff != onoff) srs[sr].data[0]=onoff;
      if(digitalRead(srs[sr].port)!=onoff) digitalWrite(srs[sr].port, onoff);
      if (onoff==1) u_setFlag(sr, &f.ISrELAYoN);
      break; 
    }
    case 2: //relay
    {
      int conoff =  srs[sr].data[0];
      if(digitalRead(srs[sr].port)!=conoff) digitalWrite(srs[sr].port, conoff);
      if (conoff==1) u_setFlag(sr, &f.ISrELAYoN);
    } 
      break; 
    case 3: //ctrl
    {
      int conoff =  srs[sr].data[0];
      int onoff;
      int reading =  srs[sr].data[1];
      int setting =  srs[sr].data[2];
      onoff = setting-reading > 0 ? 1: 0;
      if(conoff != onoff){
        if(digitalRead(srs[sr].port)!=onoff) digitalWrite(srs[sr].port, onoff);
      }
      if (onoff==1) u_setFlag(sr, &f.ISrELAYoN);
    }
      break; 
    case 4: //array
    {
    }
      break; 
    default:
      printf("never heard of it! \n");
      break;  

  } 
}
