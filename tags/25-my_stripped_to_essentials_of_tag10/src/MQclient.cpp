#include "MQclient.h"
#include <Arduino.h>
#include <PubSubClient.h>
#include "CONFIG.h"


char idev[17];
char itopic[40];
char ipayload[256];
unsigned int ipaylen;
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
    f.hayMQTT =1;
    Serial.println("connected");
    for (int i=0;i<NUMTOPICS;i++){
      char topic[25];
      strcpy(topic, cdevid);
      strcat(topic,"/"); 
      strcat(topic,subTopics[i]);
      client.subscribe(topic);
      //Serial.println(topic);     
    }
    char dd []= "in mq.reconn->devid/time, <-/prg&/devtime";
    Serial.println(dd);
    char time[20];
    strcpy(time,cdevid);
    strcat(time,"/time");
    printf("client is connected %d\n", client.connected());  
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
  strcpy(idev,topic);
  size_t b = 15;
  ipaylen = length;
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