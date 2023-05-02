#include <FS.h>                   //this needs to be first, or it all crashes and burns...
#include <Arduino.h> 
#include <WiFiManager.h>
#include "ConnWIFI.h"
#include "CONFIG.h" 

void getOnline(){
  Serial.println("Attempting to getOnline()");
  WiFiManager wifiManager;
  /*to solve blocking problem if wifi is down add portalTimeout*/
  wifiManager.setConfigPortalTimeout(60);
  Serial.print("f.hayWIFI: ");
  Serial.println(f.hayWIFI);
  // wifiManager.resetSettings();
  if (!wifiManager.autoConnect("connectEspAP")) {
    //this code runs on portal timout and drops through and out of autoconnect
    Serial.println("failed to connect and hit timeout");
    // ESP.reset();
    delay(5000);
    f.hayWIFI=0;
    Serial.print("f.hayWIFI: ");
    Serial.println(f.hayWIFI);
    Serial.println("5 sec later: dropping out of autoconnect no f.hayWIFI");
  } else{
    f.hayWIFI=1;
    Serial.println("connected...yeey :)");
  }
  Serial.println("out of getOnline()");
 }