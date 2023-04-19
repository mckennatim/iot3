# READMElog

## log
### 4/19/23 30-buttons-updInputs_redo 
about to blow up updInputs(case sen...invokeInp) -> i_updInputs 

To handle class intantiation like `ezButton button(D3);` using [preprocessor directives](https://www.cprogramming.com/reference/preprocessor/#:~:text=The%20C%20preprocessor%20modifies%20a,such%20as%20__FILE__.)

    #ifdef ESP32
      // Different ESP32 boards use different pins
      const int LED_PIN = 2;
    #else
      const int LED_PIN = LED_BUILTIN;
    #endif

### 4/13/23 29-build_appBuild_redone 
TODO 
* determine switch, toggle, butn
redone for new data structures. Also changed xdata's structure again to accomodate multiple xdata sr's per xdevid and multiple xdidx for each sr
### 4/5/23 28-ready_for_button_appdata
draft modiification in conf\128demo2\appdata.js
### 3/31/23 27-updCtrl-declare_sensor_arrays
    void invokeInp(int ix, int sen, int (*readSense)(int ix, int re)){
      int reads = inp[ix].numreadings;
      srand(time(0));
      for (int k=0;k<reads;k++){
        int nval = readSense(ix , k);//(max-min +1) + min
        int oval = getOldReading(inp[ix].tar[k].gets[0][0], inp[ix].tar[k].gets[0][1]);
        if((nval!=-99) & (nval<600) & (abs(nval-oval)>1)){ 
          // if conditions for saying something is changed

    int readSenseI2c(int ix, int re){
      return -99;// rand()%(60-5 + 1) + 5;
    }          

* [dht declaration](https://forum.arduino.cc/t/multiple-dht22-sensors/525773/2)
* [DS18B20 declaraion](https://forum.arduino.cc/t/solved-problem-declaring-an-array-of-dallas-temperature-sensors/307213/17)
### 3/23/23 26-invokeInp-maybe fried board
### 3/22/23 25-my_stripped_to_essentials_of_tag10
### 3/22/23 24-test-done
### 3/19/21 23-test-b-setXdata
### 3/18/23 22-test_bmain-button
### 3/17/23 21-test_bmain
last thing was implementing relay, tsec
### 3/9/23 20-test_amain
probably wrecked iot3/my, but now I know a bit more about c++ data structures and the wisdom on trying stuff out on the WSL c++ compiler instead of so much in platformio.
### 3/2/23 19-xdata-appbuilder
- [~] fix appCreate and appbuilder to deal with xdata
### 3/1/23 18-xdata_feature
[my/description.md # xdata NEW feature](my/description.md#xdata-new-feature)

TODO
- [] ck sending a new program in the AM
- [] test project with current sensors
- [] project for food forest
- [] test project with buttons
- [] design doorstrike
- [] create custom difference controller
- [] reflash music
- [] reflash attic
- [] reflash temp_out
- [~] fix appCreate and appbuilder to deal with xdata
- [] re-run appCreate with a programless relay and tstats should not have timeleft
- [] cleanup


### 2/28/23 17-tsec_description.md
TODO
- [] re-run appCreate with a programless relay and tstats should not have timeleft
- [] cleanup
- [x] have device subsrcibed to other devices

### 2/27/23 16-128demo2-tmr_prg
TODO
- [x]turn relay on for x seconds

req.pubTimr() put in sched.updateTimr() behind a f.cONNectd flag to run when hay active timers.

TODO 
- [x] figure out if there can be timers without programs
- [ ] cleanup
- [x] have device subsrcibed to other devices
- [x] get timers and programs to run

TODO 

- [x] now works as it should, except it as "new" instead of "rec" in the MQTT message is misleading. Fix would require breaking change in server 

TODO control bigdata and new

- [x] made `extern PubSubClient client;` in `CONFIG.h`
- class funtions of req can't use the client object, it was hacked in the constructor with cclient = client but that no longer works
- [x] fixed: document had capacity = JSON_OBJECT_SIZE(4) too small

TODO pubPrg() a sched see req.creaJSON

    id = 3
    ev(size)=2
    18:15
    sr: 3 aid: 0 hr: 21 min: 55
    3 1315
    sr: 3 is 1
    {
      "id": 3,
      "aid": 0,
      "ev": 2,
      "numdata": 1
    }{"id":3,"aid":0,"ev":2,"numdata":1}
    CYURD128/sched{"id":3,"aid":0,"ev":2,"numdata":1}

### 2/23/23 15-128demo2-mvp
Minimally viable project (mvp(compiles, runs on board with sensors, communicates with web app)) 128demo2 uses long board to test DHT11 of `cs` type, an `se` DS18B20 sensor and a `rel`. 

Worked on build system as described in [iot3/my/appCreate.md](my/appCreate.md)

TODO 
- [x] get timers and programs to run
- [ ] have device subsrcibed to other devices
- [x] getCONFIG.js produced in appCreate.js to have best working copies of COFIG and main files in conf/projectId/devid

### 2/7/23 14-wsclient_req-my_recd_sent

TODO build device to have changing states

### 2//7/23 13-hayNewMail_square1_now-respond-to-req
TODO respond to req

TODO was not parsing json from the database files. Compare to what's coming out of greenhouse
SOLUTION use zones.slice(0,-2) to get rid of last comma in json array

### 2/6/23 12-127_appCreate_build_copyCONF_compile-run
### 2/5/23 11-128_create_conf
TODO: Create a copy file to move CONFIG.h and .cpp to back to my

### 2/5/23 10-128doorStrike1

### 09-128_appbuild_upto_07

### 2/2/23 08-fix_srs_prgs_ports DOES NOT COMPILE
Design work in [description.md](descriptiom.md) 
- to fix data structures. 
- To not require progs  in order for relays to work, 
- srs.ti to srs.rel, 
- create a new ports data structure
- to slim down srs to contain only data on the state of the device
- rewrite ckRelays() to use the ne ports data structure

TODO slowly implement a little, compile and run. repeat
### 07-doorStrike-md-js-config
Aligns with /home/tim/www/react/v18/iot 

wifi connects, mqtt connects, it get date time and the requests from app.

Steps neeeded to set up a project. Also decribed in `gitinfo/wiki/fullstackIOT.md` 

TODO -> index.js in conf/127doorStrike should create all the database, initState, CONFIG.cpp and CONFIG.h to bootstrap a project.

### 06-req_changes-passing_client_in_pubstate
No longer is the req class instantiated with client. Client is passed as needed. `clpub` is removed, seems unnecesary. 

      if(f.cONNectd) req.pubState(f.HAYsTATEcNG, client);


### 05-processInc_deseriTime
Basic arduinojson 6 works

### 04-WIFI_MQTTconn-v2
Part of the process of keeping a default loop running even without connection. No more alarm every day. Every 6 hours, if !f.cONNectd, getOnline and mq.reconn run, setting hayWIFI and hayMQTT. By setting the lckconn= -every6hrs the timer gets set off to run at startup. If both hayWIFI and hayMQTT are true then f.cONNectd get set true. This flag is used to define default and connected operation.

Leaves off with handleCallback working and setting NEW_MAIL=1. Go from there.

    #include <Arduino.h>
    #include <EEPROM.h>
    #include <TimeLib.h>
    #include <TimeAlarms.h>
    // #include <ESP8266WiFi.h> //not needed as of tag 04
    #include <PubSubClient.h>
    #include "ConnWIFI.h" //getOnline()
    #include "CONFIG.h"
    #include "MQclient.h"//globals(extern) NEW_MAIL, itopic, ipayload + Console
    #include "Reqs.h"

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
    Reqs req(devid, client);

    void initShit(){
    }

    void setup() {
      Serial.begin(115200);
      EEPROM.begin(512);
      initShit();
      getOnline();
      client.setServer(mqtt_server, atoi(mqtt_port));
      client.setCallback(handleCallback); //in Req.cpp
      delay(2000);
    }

    void loop() {
      Alarm.delay(100); //needed so timealarms works
      unsigned long inow = millis();
      if(inow-lckconn >= every6hrs){
        lckconn = inow;
        if (!f.cONNectd){
            if (!f.hayWIFI) getOnline();
            mq.reconn(client);
            f.cONNectd = f.hayWIFI & f.hayMQTT;
            printf("f.hayWIFI = %d, f.hayMQTT = %d, f.cONNectd= %d\n",f.hayWIFI, f.hayMQTT, f.cONNectd);
        }
      }
      if (f.cONNectd){
        client.loop();
        if(NEW_MAIL){
            Serial.println("hay NEW_MAIL");
            req.processInc();
            NEW_MAIL=0;
        }
      }  
    } 

    *wm:STA IP Address: 192.168.1.99
    connected...yeey :)
    out of getOnline()
    Attempting remo MQTT connection...connected
    in mq.reconn->devid/time, <-/prg&/devtime
    f.hayWIFI = 1, f.hayMQTT = 1, f.cONNectd= 1
    Payload size is 107
    hay NEW_MAIL

### 03-getOnline_mq.reconn
WifiManager is working. mq.reconn is erroring out since the server doesn't know CYURD127

This is the core connection part of the old version. The problem with it is that there is no need to have getonline() in setup and there is nothing telling you both hayWIFI and hayMQTT are true. 

Also, the old style just doesn't work at keeping you connected. It will infinite loop if connection problems and there is no core set of functions that will work without a connection.


### 02-AlarmTimerOnce
needs Alarm.delay(1000); in loop to work

### 01-blink
for esp8266 and esp32

### 00 start iot3
The latest working is getting put into iot3/my slowly (since it is not compiling iin espBoth)
