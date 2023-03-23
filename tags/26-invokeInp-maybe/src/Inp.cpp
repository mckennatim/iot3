#include "Inp.h"
#include "CONFIG.h"
#include "Util.h"
#include <DallasTemperature.h>
#include <DHT.h>

OneWire oneWire_0(D2); 
DallasTemperature DS18B20_0(&oneWire_0);
OneWire oneWire_1(D3); 
DallasTemperature DS18B20_1(&oneWire_1);
int readDs18b20(int ix, int re){
  int port = inp[ix].port[0];
  switch(port){
    case D2:{
      return (int)DS18B20_0.getTempFByIndex(re);
      break;}
    case D3:{
      return (int)DS18B20_1.getTempFByIndex(re);
      break;}
    default:   
      printf("never heard of it! \n");
      return -99;
      break;  
  }
}


int readSenseSpi(int ix, int re){
  return -99; //rand()%(60-5 + 1) + 5;
}

int readSenseDht(int ix, int re){
  return -99; //rand()%(60-5 + 1) + 5;
}

int readSenseI2c(int ix, int re){
  return -99;// rand()%(60-5 + 1) + 5;
}

int readSwitch(int ix, int re){
  int port = inp[ix].port[0];
  if (debounce(port)) {
    digitalWrite(port, !digitalRead(port));
  }
  return digitalRead(port);
}

int readButton(int ix, int re){
  int port = inp[ix].port[0];
  int prev = inp[ix].actions.prev;
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
  } 
  if ((!down) & (prev)){ //release button, calc time pressed -> nval = lval|sval
    unsigned long int now = millis();//CNG
    unsigned long int dif = now - lONGpRESStIMR[ix];
    printf("button dif = %ld \n", dif);
    if(dif > lptime){
      retval = lval;
    }else retval = sval;
  }
  /*random button push similator
  int shlo[] = {0, sval, lval};
  int wsl = rand()%(2-0 + 1) + 0;
  retval = shlo[wsl];
  end of */
  return retval;
}

void i_updInputs(){
  for(int ix=0;ix<NUMINP;ix++){
    for(int sen=0;sen<SENSTYPS;sen++){
      if(strcmp(sensors[sen],inp[ix].type)==0){
        switch (sen){
          case 0: //butn
            invokeInp(ix, sen, &readButton);
            break;
          case 1: //switch
            invokeInp(ix, sen, &readSwitch);
            break;
          case 2: //1-wire
            invokeInp(ix, sen, &readDs18b20);
            break;
          case 3: //dht
            invokeInp(ix, sen, &readSenseDht);
            break;
          case 4: //i2c
            invokeInp(ix, sen, &readSenseI2c);
            break;
          case 5: //spi
            invokeInp(ix, sen, &readSenseSpi);
            break;
          default:
            break;  
        }
      }
    }
  }
}

void invokeInp(int ix, int sen, int (*readSense)(int ix, int re)){
  int reads = inp[ix].numreadings;
  srand(time(0));
  for (int k=0;k<reads;k++){
    int nval = readSense(ix , k);//(max-min +1) + min
    int oval = getOldReading(inp[ix].tar[k].gets[0][0], inp[ix].tar[k].gets[0][1]);
    if((nval!=-99) & (nval!=oval)){
      for(int m=0;m<inp[ix].tar[k].numtargs;m++){//for all targets
        int sr = inp[ix].tar[k].gets[m][0];
        /* for switch with tsec>0*/
        unsigned long int tsec = 0;
        if((sen == 1) & (oval == 0)& (nval ==1) & (srs[sr].data[1]>0)){//i=1=switch is on 
          tsec =srs[sr].data[1]; 
          sTRTsWtIMR[sr] = millis(); //millis() //CNG//
        }
        if((sen == 1) & (oval == 1)& (nval == 1)& (srs[sr].data[1]>0)){        
          unsigned long starttime = sTRTsWtIMR[sr];
          unsigned long endtime = millis(); //CNG// max tsec =32767
          if(starttime-endtime > tsec*1000) {
            nval=0;
            sTRTsWtIMR[sr] = 0;
          }
        }
        /* end of switch with tsec>0*/
        int da = inp[ix].tar[k].gets[m][1]; //target data
        /* for button short/long press*/
        if((nval == inp[ix].actions.lval) | (nval == inp[ix].actions.sval)){
          nval = oval + nval;
        }
        /* end of button short/long press*/
        srs[sr].data[da]=nval;
        u_setFlag(sr, &f.HAYsTATEcNG); 
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
  return (state == 0xff00);
}