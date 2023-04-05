#include "Sched.h"
#include <ArduinoJson.h>
#include <TimeLib.h>
#include "CONFIG.h"
// #include "Reqs.h"
#include "Util.h"
#include "Inp.h"


extern char ipayload[250];

void s_ckAlarms(int sr, int b){
  int idx = u_getPrgIdx(sr);
  if (idx >= 0){
    int dax = prgs[idx].dax;
    /*set current program setting in srs*/
    int cur, nxt, tleft; 
    findCurNxt(idx, cur, nxt, tleft);
    printf("prgidx=%d,cur:%d setting:%d, nxt:%d tleft=%d\n",idx, cur, prgs[idx].prg[cur][2],nxt,tleft);
    srs[sr].data[dax] = prgs[idx].prg[cur][2];
    i_updCtrl(sr, 0);  
    int onoff= digitalRead(srs[sr].port);
    if((onoff==1) & (u_getFlag(sr, f.HAStIMR))) {
      tIMElEFT[sr] = tleft*60;
      u_setFlag(sr, &f.IStIMERoN);
    }
  }
}

void findCurNxt(int idx, int &cur, int &nxt, int &tleft){
  int ev = prgs[idx].ev;
  int hr =hour();
  int min = minute();
  tleft = 0;
  printf("time= %d:%d \n", hr, min);
  for(int j=0; j<ev;j++){
    if (hr == prgs[idx].prg[j][0]){
      if (min < prgs[idx].prg[j][1]){
        cur = j-1;
        break;
      }
    }
    if (hr < prgs[idx].prg[j][0]){
      cur= j-1;
      break;
    }
    cur =j;
  }
  nxt = cur+1;
  if (nxt>=ev) nxt=0;  
  if (nxt==0) tleft = (23-hr)*60+(59-min) +1;
  else {
    int nxthr = prgs[idx].prg[nxt][0];
    int nxtmin = prgs[idx].prg[nxt][1];
    if(nxtmin < min){//12:25 -> 14:05
      nxtmin=nxtmin+60;
      nxthr--;
    }
    tleft= (nxthr-hr)*60 + (nxtmin - min);
  }
}