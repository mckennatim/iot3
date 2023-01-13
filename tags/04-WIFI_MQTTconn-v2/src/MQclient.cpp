#include "MQclient.h"
#include <Arduino.h>
#include <PubSubClient.h>
#include "CONFIG.h"


char itopic[40];
char ipayload[256];
bool NEW_MAIL=0;

Console::Console(char* devid, PubSubClient& client ){
  cdevid = devid;
  cclient = client;
}

void Console::log(char* dd){
	char log[20];
	strcpy(log,cdevid);
	strcat(log,"/log");
  if (cclient.connected()){
    cclient.publish(log, dd, true);
  }		
}

MQclient::MQclient(char* devid, char* owner, char* pwd){
	cdevid = devid;
  sowner = owner;
  spwd = pwd;
}

void MQclient::reconn(PubSubClient& client) {
  Serial.print("Attempting remo MQTT connection...");
  if (client.connect(cdevid, sowner, spwd)) {
    Serial.println("connected");
    for (int i=0;i<TPC.numtopics;i++){
      char topic[25];
      strcpy(topic, cdevid);
      strcat(topic,"/"); 
      strcat(topic,TPC.scribedTo[i]);
      client.subscribe(topic);
      //Serial.println(topic);     
    }
    char dd []= "in mq.reconn->devid/time, <-/prg&/devtime";
    Serial.println(dd);
    char time[20];
    strcpy(time,cdevid);
    strcat(time,"/time");  
    client.publish(time, dd, true);    
    return;
  } else {
    Serial.print("failed, rc=");
    Serial.print(client.state());
    delay(5000);
    Serial.println(" try again in 5 seconds");
  }
}

void handleCallback(char* topic, byte* payload, unsigned int length){
  size_t b = 15;
  Serial.print("Payload size is ");
  Serial.println(length);
  for (size_t i=0;i<strlen(topic);i++) {
    itopic[i] = topic[i];
    if (topic[i] == '/'){b = i;}
    if(i>b){
      itopic[i-b-1] = topic[i];
    }
  }
  itopic[strlen(topic)-b-1] = '\0';
  for (unsigned int i=0;i<length;i++) {
    ipayload[i] = (char)payload[i];
  }
  ipayload[length] = '\0';
  NEW_MAIL = 1;
}