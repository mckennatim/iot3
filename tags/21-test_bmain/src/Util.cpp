#include "CONFIG.h"
#include "Util.h"

int getRelSt(int sr){
  int rval = -99;
  for (int j=0; j<srs.numcs;j++){
    if(srs.cs[j].sr == sr){
      rval = srs.cs[j].onoff;
    }
  }
  for (int j=0; j<srs.numrel;j++){
    if(srs.rel[j].sr == sr){
      rval = srs.rel[j].onoff;
    }
  }
  return rval;
}