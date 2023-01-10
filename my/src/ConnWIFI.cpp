#include <FS.h>                   //this needs to be first, or it all crashes and burns...
#include <Arduino.h> 
#include <WiFiManager.h>
#include "ConnWIFI.h"
#include "CONFIG.h" 


void getOnline(){
  Serial.println("Attempting to getOnline()");
  WiFiManager wifiManager;
  /*to solve blocking problem if wifi is down add portalTimeout*/
  wifiManager.setConfigPortalTimeout(120);
  Serial.print("haywifi: ");
  Serial.println(haywifi);
  //wifiManager.resetSettings();
  if (!wifiManager.autoConnect("connectEspAP")) {
    //this code runs on portal timout and drops through and out of autoconnect
    Serial.println("failed to connect and hit timeout");
    // ESP.reset();
    delay(5000);
    haywifi=0;
    Serial.print("haywifi: ");
    Serial.println(haywifi);
    Serial.println("5 sec later: dropping out of autoconnect no haywifi");
  } else{
    Serial.println("connected...yeey :)");
  }
  Serial.println("out of getOnline()");
 }