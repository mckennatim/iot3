#include<stdio.h>
#include <string.h>
#include "aconf.h"
#include "autil.h"
#include <ArduinoJson.h>

char sepayload[60] = "{\"id\":0, \"darr\":[54]}";
char dipayload[100] = "{\"id\":0, \"darr\":[0, 54,67, 10, 1, 0]}";

int id7 = 0;
int id5 = 0;
int darr5[1]={54};
int darr7[6]={1, 67, 46, 10, 1, 1};
char c[20] = "CYURD006/srstate";//idev 0
char d[20] = "CYURD018/srstate";//idev 1

int getXdataIdx(int xsr){
  int xd=-1;
  for(int i=0;i<NUMXD;i++){
    if(xdata[i][0]==xsr){xd= i;}
  }
  return xd;
}

/*get sr and darr from ArduinoJson on ipayload*/
void setXdata(char* idev, int sr, int darr[]){
  int xsr =getXsr(sr, whichDev(idev));
  int tdidx = getTdsIdx(xsr);
  int xdidx = getXdataIdx(xsr);
  xdata[xdidx][2] = sr; 
  for (int i=0;i<tds[tdidx].numdl;i++){
    xdata[xdidx][i+3] = darr[i];
  }
}

void printXdata(){
  for (int i=0;i<NUMXD;i++){
    printf("{");
    for (int j=0;j<MAXD+3;j++){
      printf("%d, ",xdata[i][j]);
    }
    printf("}, \n");
  }
}

int main()   // define the main functio
{  

  printf("Welcome to JavaTpoint \n"); 
  printf("getStird %d \n", getStoredValue(0, "di", "rdB"));
  printf("%s \n",xdevtpc[0]);
  printf("%d \n", xdata[1][8]);
  printf("%s \n", tds[4].dats[4]);

  char e[9];
  size_t l = strlen(d);
  char f[20];
  devOfDevTpc(xdevtpc[0],e);
  printf("%d, %c, %d, %s\n",(int)l, d[1], d[1]=='Y', e);
  printf("%s \n",devid);
  printf("%d \n", isThisXdata(d));
  printf("whiichDev %d \n", whichDev(d));
  int xsr = getXsr(1, whichDev(d));
  printf("xsr = %d \n",xsr );
  int numdl = tds[getTdsIdx(xsr)].numdl;
  printf("numdl = %d \n",numdl );
  setXdata(d, 1, darr7);
  printXdata();
}