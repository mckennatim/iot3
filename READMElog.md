# READMElog

## log

### 04-WIFI_MQTTconn-v2
Part of the process of keeping a default loop running even without connection. No more alarm every day. Every 6 hours, if !f.cONNectd, getOnline and mq.reconn run, setting hayWIFI and hayMQTT. If both hayWIFI and hayMQTT are true then f.cONNectd get set true. This flag is used to define default and connected operation.

    const long every6hrs = 21600000;
    const long every5sec = 5000;
    const long every2sec = 2000;

    signed long lckconn = -every6hrs;
    signed long lcktimr = 0;
    signed long lcksens = 0;

    WiFiClient espClient;
    PubSubClient client(espClient);
    Console console(devid, client);
    MQclient mq(devid, owner, pwd);

    void setup() {
    Serial.begin(115200);
    EEPROM.begin(512);
    initShit();
    setTime(8,29,0,1,1,11);
    client.setServer(mqtt_server, atoi(mqtt_port));
    client.setCallback(handleCallback); //in Req.cpp
    Serial.println("leaving setup");
    }

    void loop() {
    Alarm.delay(100); //needed so timealarms works
    unsigned long inow = millis();
    
    if(inow-lckconn >= every6hrs){
        Serial.print("dif: ");
        Serial.println(inow-lckconn);
        Serial.print("every6hrs: ");
        Serial.println(every6hrs);
        lckconn = inow;
        if (!f.cONNectd){
            if (!f.hayWIFI) getOnline();
            mq.reconn(client);
            f.cONNectd = f.hayWIFI & f.hayMQTT;
        }
    }
    }  


### 03-getOnline_mq.reconn
WifiManager is working. mq.reconn is erroring out since the server doesn't know CYURD127


### 02-AlarmTimerOnce
needs Alarm.delay(1000); in loop to work

### 01-blink
for esp8266 and esp32

### 00 start iot3
The latest working is getting put into iot3/my slowly (since it is not compiling iin espBoth)
