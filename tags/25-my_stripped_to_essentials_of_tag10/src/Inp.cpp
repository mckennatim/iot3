#include "Inp.h"
#include "CONFIG.h"
#include "Util.h"
#include <DallasTemperature.h>
#include <DHT.h>

OneWire oneWire(D2); 
DallasTemperature DS18B20(&oneWire);

int readSense(const int *port, int ix){
  return 14; //rand()%(60-5 + 1) + 5;
}

int readSenseI2c(const int *port, int ix){
  return 15;// rand()%(60-5 + 1) + 5;
}

int readSwitch(const int *port, int ix){
  if (debounce(port[0])) {
    digitalWrite(port[0], !digitalRead(port[0]));
  }
  return digitalRead(port[0]);
}

int readButton(const int *port, int h){
  srand(time(0));
  int prev = inp[h].actions.prev;
  unsigned long int lptime = inp[h].actions.lptime;
  // int shlo[] = [inp[h].actions.lval;
  int lval = inp[h].actions.lval;
  int sval = inp[h].actions.sval;
  int retval = 0;
  if (debounce(port[0])) {
    digitalWrite(port[0], !digitalRead(port[0]));
  }
  int down =  digitalRead(port[0]);
  if (down & (prev==0)){ //press button, start long press timer
    lONGpRESStIMR[h] = millis(); //CNG
  } 
  if ((!down) & (prev)){ //release button, calc time pressed -> nval = lval|sval
    unsigned long int now = millis();//CNG
    unsigned long int dif = now - lONGpRESStIMR[h];
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
// void i_updInputs(){
//   DS18B20.requestTemperatures();
//   printf("%d \n",(int)DS18B20.getTempFByIndex(0));
// }
void i_updInputs(){
  for(int ix=0;ix<NUMINP;ix++){
    for(int sen=0;sen<SENSTYPS;sen++){
      if(strcmp(sensors[sen],inp[ix].type)==0){
        switch (sen){
          case 0: //butn
            invokeInp(ix, sen, &readButton);
            break;
          case 1: //switcinp
            invokeInp(ix, sen, &readSwitch);
            break;
          case 2: //1-wire
            invokeInp(ix, sen, &readSenseI2c);
            break;
          case 3: //xt
            invokeInp(ix, sen, &readSenseI2c);
            break;
          case 4: //i2c
            invokeInp(ix, sen, &readSenseI2c);
            break;
          case 5: //spi
            invokeInp(ix, sen, &readSenseI2c);
            break;
          default:
            printf("never heard of it! \n");
            break;  
        }
      }
    }
  }
}

void invokeInp(int ix, int sen, int (*readSense)(const int *port, int sen)){
  int reads = inp[ix].numreadings;
  srand(time(0));
  for (int k=0;k<reads;k++){
    int nval = readSense(inp[ix].port , sen);//(max-min +1) + min
    int oval = getOldReading(inp[ix].tar[k].gets[0][0], inp[ix].tar[k].gets[0][1]);
    if(nval!=oval){
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

bool debounce(int port) {
  static int state = 0;
  state = (state<<1) | digitalRead(port) | 0xfe00;
  return (state == 0xff00);
}