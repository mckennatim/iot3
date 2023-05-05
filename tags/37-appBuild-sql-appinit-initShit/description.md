## getting connected
`ConnWIFI.getOnline()` is first called in `setup()`. It starts the `WIFImanager`. If the WIFImanager doesn't succeed in connecting to its saved credentials it starts `connectEspAP` on 192.168.4.1. If that doesn't complete in 120 seconds then the flag `haywifi=0` and the program goes on with its default schedule.



#### problem with losing power and losing connection
Once you lose the wificonnection the mq.reconn will just be in and endless loop


## explanation of the OLD design
from [abbrevaitions.md](./abbreviations.md):

srs
- sr: sesnor or relay ID
- se: sensors only, not controlling relays
  - {sr, reading...}
- cs: relays controlled by sensors. The sr ID is the same as the prg sr ID. The relay's port is listed in prg. A prg with 1 event with [hr,min,hilimit,lolimit] = [0,0,hilimit,lolimit] runs that all day
  - {sr, reading, onoff, hilimit, lolimit...} these are the default hilimit & lolimit.
- rel: relays. Can be controlled by <s>timers</s> prgs. The sr ID is the same as the prg sr ID. ```The relay's port is listed in prg. THIS IS STUPID``` 
  - {sr, onoff...}
- di: difference controller  

Since both cs a rel's can have an associated program, why do they need hilimit/lolimit or onoff in the srs structure?

Because it is the place that stores the the current and default values. 

### what happens in the loop

During the loop a message could come in from the outside. Or there could be a change in one of the sensor values. Or a running timer can display its progress or could reach the end of its run. Or it might be time to change a program. Or check the difference controller or custom code.

### things wrong with putting relay ports in prgs
Sensors are read every loop. `readSensors()` calls `setIfDif()` which sets HAYsTATEcNG to allow state to be published. More importantly, if it is a sensor that controls a relay (cs), sched.adjRelay(csidx, cs[idx]) is called. `sched.adjRelays()` does not actually adjust relays, just sets the f.ISrELAYoN flag. The loop triggers `sched.ckRelays()` every second.

A relay may not need a prg. Right now you have to set up a fake prg just to get to the relay.

`ckRelays()` cycles through all the prgs.sr's, and, gets the `onoff` value from either cs or rel. If their stored onoff value is different then what `digitalRead(port) says it writes new srs value to the relay with dgitalWrite(port, value)

### the fix for relay ports
OK, so you are checking relays every couple of seconds in loop(). Why not just cycle through the port list for outputs, digitalRead(port) then go through the cs and rel srs records and change if different.

    def.ckRelays(){ 
      for (int j=0; j<srs.numcs;j++){
        iscsidx_t ici = getTypeIdx(j); 
        int sr = srs.cs[ici.idx].sr
        int onoff = srs.cs[ici.idx].onoff
        int ponoff = digitalRead(ports[sr],output)
        if(onoff ! =ponoff){
          digitalWrite(sr, onoff)
        }
      }
      for (int j=0; j<srs.numrel;j++){
        iscsidx_t ici = getTypeIdx(j); 
        sr = srs.rel[ici.idx].sr
        onoff = srs.rel[ici.idx].onoff
        ponoff = digitalRead(ports[sr].output)
        if(onoff ! =ponoff){
          digitalWrite(sr, onoff)
        }
      }
    }
    
### OK so what happens with onewire devices? 
How does it work now? 

### proposed modified data structures 
    /*SE constant declarations*/  
    const sen_t SE {
      3,//numtypes of different sensor types
      5,//numsens of sensors(numsens)
      { // {nums,{sr,sr},senses,model}
        {1, {0}, "light", "BH1750"},//assumes SCL is D1(5) and SDA is D2(4)
        {2, {1,2}, "temp", "DS18B20a"},  
        {2, {3,4}, "temp-hum", "DHT11"},
        {1, {8}, "onoff" "contact"}
        /* not used
        {1, {5}, "hygrometer", "ANALOG"}
        {1, {6}, "thermoco", "MAX31855"}//not used
        {2, {3,4}, "temp", "DS18B20b"},  *?
      }
      
     /*ports for input and output
      {sr, in, out, rec, isnew} */
    const ports_t ports {
      {0, D4, -9, 1, 0},// se single sensor controlling nothing
      {1, D0, D6, 1, 0},// onewireA controlling relay
      {2, D0, D7, 1, 0},// onewireA controlling relay
      {3, D1, -9, 1, 0},// dhtlike temp controlling nothing
      {4, D1, D3, 1, 0},// dhtlike hum controlling relay
      {5, -9, D2, 1, 0},// relay controlled by app default or timer
      {6, -9, D4, 1, 0},// relay controlled by app default or timer
      {7, -9, D4, 1, 0},// relay controlled by app default or timer
      {8, D9, -9, 0, 0},// contact sensor9,
      {9, -9, D0, 1, 0}
    }

    /*srs_t the state of the machine, all the sensor values, relay states,
    ho/lo limits, differences and maxvals*/ 
    srs_t srs {
      8,
      2,//se{{sr,reading}} 
      {
        {0,45},
        {3,45},
      },
      3,//cs{{sr,reading, onoff hi, lo}}
      {
        {1,44,0,63,61}, 
        {2,44,0,80,84},
        {4,44,0,63,61},
      },
      2,//rel {sr, onoff}
      {
        {5,0}
        {6,0}
        {7,0}
      },
      1,// di {sr, sra,srb,diffon, diffoff, maxa, maxb, onoff} 
      {
        {9,3,6,9,200,200,0}
      }
    };

    *prgs extern data structure initalization*/ 
      prgs_t prgs{
        2,//numprgs
        { //prg: {sr,aid,ev,numdata,prg[[hr,min,max,min]],port,hms}
          {1,255,1,2,{{0,0,80,78}},1504}, //ds18b
          {4,255,1,2,{{0,0,80,78}},1504}, //hum
          {6,255,1,2,{{0,0,0}},1503}  //timr1
          {7,255,1,2,{{0,0,0},{6,15,1}},1503}  //timr2
        }
      };    

#custom code for `doorsStrike` project usein the new data structures and ckRelay()

    loop(){
      ...
      if (inow - lcksens > every2sec) {
          lcksens = inow;
          //readSensors();
          if(f.HAYsTATEcNG>0){
              if(f.cONNectd) req.pubState(f.HAYsTATEcNG, client);
              f.HAYsTATEcNG=0;
          }
          customLoop();
          // diffCtrl();  
      }   
    }

CYURD002/cmd{id:sr, sra:[1] or sra:[69,67]}, processed by req.desirCmd() can be used to modify srs.rel onoff value which in the loop gets flagged by f.HAYsTATEcNG to publish the states that have changed and runs ckRelays iver all the cs and rel srs's.



    void customLoop() {
      // listening for doorStrike command or change in door contact
      iscsidx_t str = getTypeIdx(0);
      iscsidx_t con = getTypeIdx(1);
      iscsidx_t red = getTypeIdx(2);
      iscsidx_t gre = getTypeIdx(3);
      iscsidx_t blu = getTypeIdx(4);
      int bitr = pow(2,2);
      int bitg = pow(2,3);
      int bitb = pow(2,4);
      int bits = pow(2,0);
      int bitl = bitg + bitr + bitb;
      read contact and strike from srs
      int stro = srs.cs[str.idx].onoff 
      int cono = srs.cs[con.idx].onoff 
      if (cono == 1){
        setlight blue
        srs.cs[red.idx].onoff = 0
        srs.cs[gre.idx].onoff = 0
        srs.cs[blu.idx].onoff = 1
        if (strike on){
          turn strike off
          srs.cs[str.idx].onoff = 0
          /* mark strike relays for state change ->ckRelays()*/
          f.HAYsTATEcNG=f.HAYsTATEcNG | bits;
        }
      }else{
        if(st0 == 1 ){
          turn light green
            srs.cs[red.idx].onoff = 0
            srs.cs[gre.idx].onoff = 1
            srs.cs[blu.idx].onoff = 0
        } else{
          turn light red
            srs.cs[red.idx].onoff = 1
            srs.cs[gre.idx].onoff = 0
            srs.cs[blu.idx].onoff = 0
        }
      }
      /* mark led relays for state change->ckRelays()*/
      f.HAYsTATEcNG=f.HAYsTATEcNG | bitl; 
    }


maybe everthing default on the chip like reading sensors and responding to ckRelays should be in Def.cpp    


Loop watches for HAYsTATEcNG, runs reqs.pubState() if it is >0.

### 128doorStrike0 version -code should generate
    /*SE constant declarations*/  
    const sen_t SE {
      1,//numtypes 
      1,//numsens of sensors
      { // {nums,{sr,sr},type,model}
        {1, {0}, "contact", "NCcontact"},
      }
    }

    /*ports for input and output
      {sr, in, out, rec, isnew} */
    const ports_t ports {
      {0, D5, -9, 1, 0},// contact
      {1, -9, D1, 1, 0},// strike
      {2, -9, D8, 1, 0},// ledRed
      {3, -9, D7, 1, 0},// ledGreen
      {4, -9, D6, 1, 0},// ledBlue
    }

    /*srs_t the state of the machine, all the sensor values, relay states,
    ho/lo limits, differences and maxvals*/ 
    srs_t srs {
      5,
      1,//se{{sr,reading}} 
      {
        {0,1}, //contact
      },
      0,//cs{{sr,reading, onoff hi, lo}}
      {},
      4,//rel {sr, onoff}
      {
        {1,0}, //strike
        {2,1}, //ledRed
        {3,0}, //ledGreen
        {4,0}, //ledBlue
      },
      0,// di {sra,srb,diffon, diffoff, maxa, maxb, srrel, onoff} 
      {}
    };    

    *prgs extern data structure initalization*/ 
      prgs_t prgs{
        0,//numprgs
        {} //prg: {sr,aid,ev,numdata,prg[[hr,min,max,min]],port,hms}  
      }
        

## topics of communication
### sent by device
`/time` is sent on startup via `mq.reconn(client)` and every day at midnight via `Alarm.alarmRepeat(0,minu,0, getTime);` It cases the server to return `/devtime` and `/prg`

    CYURDOO4/time "startupORdaily->devid/time, <-/prg&/devtime"

`/srstate` is sent whenever the sensor or relay value changes or if setpoints like hilimit or lolimit are changed. 

    CYURDOO4/srstate {"id":0, "darr":[64, 1, 81, 78], "new"}

`/sched` sends the schedule current running for a particular sesnor or relay. One of the devices built in timers is reserved for downloading the day's schedule from the mqtt broker->database at around midnight every day.
    
    CYURDOO4/sched {"id":2,"aid":2,"ev":3,"numdata":1,"pro":[[0,0,1],[7,45,0],[19,0,1]]}

`timr` sends and array of timeleft on all relays that are run by a timers. "IStIMERoN" and "ISrELAYoN" is a binary representation of which relays and timers are currently on.

    CYURD002/timr{"cREMENT":5,"IStIMERoN":4,"ISrELAYoN":5,"tIMElEFT":[0,0,26215,0,0]}

### receive by device, sent by app

`cmd` sends a cmd that overides the hi/lo or onoff of the currently running schedule]

    CYURD002/cmd{id:sr, sra:[1] or sra:[69,67]}

`devtime`

`req`

`prg`

`set`

testme

# scratchpad

### every day if it is not connected to WIFI
When the dailyAlarm() happens, before getting the time and the day's prg(s), there is a check to see if WIFI is cconnected. If not it trys to connect again.

    at daily alarm
        maxtries = 0
        if(haywifi==0){
            haywifi=1;
            getOnline();
        }

If that works then `haywifi=1` again. 

    if maxtries <10 
        if(!client.connected() && haywifi){
            maxtries +=1
            mq.reconn(client);
        }else{
            client.loop(); //if MQTT is connected
        } 
    }  

    inow - millis()
        if (inow - before > anhour)

    3x a day ck connection
        if !cONNectd
            if !haywifi
                getOnline()
                success ->hayWIFI
            mq.reconn()
            success ->hayMQTT
            cONNectd = hayWIFI & hayMQTT
            if cONNectd
                getTimeSched()



    const long every6hrs = 21600000;
    const long every5sec = 5000;
    const long every2sec = 2000;

    unsigned long lckconn = 0;
    unsigned long lcktimr = 0;
    unsigned long lcksens = 0;

    void setup(){
        Serial.begin(115200);
        EEPROM.begin(512);
        initShit();
        client.setServer(mqtt_server, atoi(mqtt_port));
        client.setCallback(handleCallback); //in Req.cpp
    }

    void loop(){
        unsigned long inow = millis();
        
        if(inow-lckconn >= every6hrs){
            lckconn= inow; //reset 
            if (!cONNectd){
                if (!hayWIFI)
                    getOnline();
                    // success ->hayWIFI
                mq.reconn(client);
                //success ->hayMQTT
                cONNectd = hayWIFI & hayMQTT
                if (cONNectd)
                    getTimeSched()
            }
        }

        if (cONNectd){
            client.loop();
            if(NEW_MAIL){
                Serial.println("hay NEW_MAIL");
                req.processInc();
                NEW_MAIL=0;
            }
        }

        if (f.CKaLARM>0){
            sched.ckAlarms(); //whatever gets scheduled should publish its update
            if (cONNectd){
                req.pubPrg(f.CKaLARM);
                req.pubTimr();
            }
            f.CKaLARM=f.CKaLARM & 0; //11110 turnoff CKaLARM for 1
        }


        if(inow-lcktimr >= every5sec){
            lcktimr = inow;
            if (f.IStIMERoN >0){
                sched.updTimers();
                if(cONNectd) req.pubTimr();
            }
            sched.ckRelays();
        }    

        if (inow - lcksens > every2sec) {
            lcksens = inow;
            readSensors();
            if(f.HAYsTATEcNG>0){
                if(cONNectd) req.pubState(f.HAYsTATEcNG);
                f.HAYsTATEcNG=0;
            }
            customLoop();
            diffCtrl();  
        }
    }  
      



### Possible setups

If wifi goes out during the day, try to reconnect the next day. If mq disconnect doesn't reconnect after 5 tries, ignore the internet until the next day. 

What if MQTT is not connected, you would eventually want to ignore it and then move on with the default schedule. Maybe every time it tries to run mq.reconn() it starts a timer? If its been trying for 4 minutes then stop trying 


cONNECTED = haywifi & haymqtt

doDefault

* read sensors every 2 seconds
* check schedules 
* run schedules
* react to state change f.HAYsTATEcHNG


while connected

* check MAIL
* check f.HAYsTATEcHNG


##  strike prototype using long PCB

1. Decide on a PCB. Loo at possible inputs

{
  sr: 0, 
}


    /*PORTS for INPUT*/
    const portsin_t inpo {
      DO, //DONT USE out longPCB p5        
      D1, //out longPCB p1
      D2, //in longPCB J4,5,8,9 onewire
      D5, //in longPCB J10
      D6, //SPIdo out longPCB p4
      D7, //shares i2c D2 and D1 out longPCB p3
      D8, ///SPIcs pulled gnd, boot fails if hi out longPCB p2
      A0, //ANALOG
      D3, //pulled up
      D4};//pulled up
    /*SE constant declarations*/  

# new versiom iot3/my

## functional flow

MQTT fires a `handleCallback` when a message is received which sets NEW_MAIL and puts the message topic and payload in extern (global) `itopic` and `ipayload` and `idev`. Then `idev` is dev/topic like `CYURD006/srstate`

`req.processInc()` runs from loop() when there is NEW_MAIL

### responding to incoming prg message

- CYURD073/prg {"id":0,"pro":[[0,0,61,59],[9,0,62,60]]} JsonArr `[[0,0,61,59],[9,0,62,60]]` looks for the sr in prgs that matches the id of MQTTmess. Then it sets the f.CKaLARM bit and copies the prg and JsonArr. into prg.

      req.processInc(ipayload) => 
          sched.deseriProg(kstr) => 
              set:f.CKaLARM for sr
              sched.copyProg(prgs.prg[i], events) 

### responding to an xdata message  like CYURD006/ststate  
The state of external devices (xdata) can be subscribed to. `getXdata` stores that state in the `xdata` data structure.         

### in loop()   

`f.CKaLARM` is set to have programs scanned... 
1. on startup of an app. `req.processInc()case 0` 
2. whenever a new program comes in on an MQTT message. `sched.deseriProg()`
3. whenever an alarm rings at the end of a program event `alarmRings()`

`ckAlarms()` is called from `main.cpp` every 2 seconds if a `f.CKaLARM` is set for any sr. After `sched2.ckAlarms()` runs `f.CKaLARM=f.CKaLARM & 0;` are cleared. in `main.cpp`

`ckAlarms()` cycles through prgs. For each prg it creates a pointer to that prg,sending that to `setCur(prg_t& p, int &cur, int &nxt)` which returns a `cur` set set of hilo values [hi,lo] or [onoff] and a `nxt` set of [hr,min]. For both `cs` an `rel` an Alarm is set `Alarm.alarmOnce(hr,min,sr, alarmRings)` to go off when the time gets to nxt[h,m]. (The callback: `alarmRings()` sets the bit in f.CKaLARMS once time is up). Prgs are `sortPrgHms()`, printed to console `showArray()` and `f.HAYsTATEcNG` is set

The `srs.cs[idx of sr].hi&lo`  are set and the relays are adjusted if needed. 

The `srs.rel[idx of sr].onoff` is set to current prg[onoff]. If relay is on, `f.ISrELAYoN` and `f.IStIMERoN` are set and `f.tIMElEFT[sr]=getTleft()*60`;

    ckAlarms() =>
      cycles through prgs 
      setCur(prg_t& p, int &cur, int &nxt) => prg[cur] and prg[nxt]
      Alarm.alarmOnce() cb>> alarmRings sets:f.CKaLARMS
      sortPrgHms()
      showArray()
      set:f.HAYsTATEcNG  
      for cs
        sets `srs.cs[idx of sr].hi&lo` to cur[hi,lo]
        adjRelay() if needed
      for rel
        sets `srs.rel[idx of sr].onoff` to cur[onoff]  
        if relay is on 
          set f.ISrELAYoN and f.IStIMERoN
          get setTleft() and put it in f.tIMElEFT[sr]=tleft*60;
          if hay only one prg [0,0,1] mask f.IStIMERoN

`sched2.updTimers()`  is called from `main.cpp:loop()` every 2 seconds 

`sched2.ckRelays()` is called from `main.cpp:loop()` every 2 seconds

`readSensors();` is called from `main.cpp:loop()` every 2 seconds

#### in loop(), only if MQTT (and WIFI) are connected...
- `req.processInc();` is called every time there is `NEW_MAIL`
- When `f.HAYsTATEcNG>0` `req.pubState()` is called
- When `f.CKaLARM>0` `req.pubPrg(f.CKaLARM)` is called

### adjRelays(int sr, cs_t& te
- Called when a srs`cs` reading changes from
  - main().loop()->readSensors()->setIfDir()->setSrs()
- Called when a `cs` /cmd comes in changin hi & lo
- Does Compare reading to hi & lo and if there should be a change
  - sets cs.onoff
  - digitalWrites(port[sr].out, onoff) 

XDATA COULD BE for`cs`

Can cs use xdata? Set a local relay based un external reading? Add funtionality to a tstat? Have a second tstat connected to the exdata sensor that say turns on a ceiling fan if a room is occupied and it is hot in the room? ports like {9,-12,D4,"cs",xd=2, rec=0}. If so
- Called when xdata changes

XDATA COULD BE for `di`
  - Called when rdA or rdB change whe readSensors() set CKrELAYS
  - Called when xdata (used as rdA or rdB) changes
  - Called when di/cmd comes in desiriCmd

Does 

XDATA COULD BE for `cr`
  - Called when some external relay changes, changing local relay for some tsec. 0=infinity. CAN `cr` HAVE A `pgm`? A switch that goes on at certain times of the day? Sounds like `rel`. Maybe `cr` and `rel` functionality can be combined.

XDATA COULD BE for `rel`  
  - Called when some switch changes somewhere else

##  
Timers are different from programs. They are contained if the `f.tIMElEFT` array.

Timers `f.tIMElEFT` are set in `Sched::ckAlarms()` whenever the loop() is run and `f.CKaLARM>0`.

Timers are updated whenever `f.tIMElEFT>0` in loop() via `updTimers()`

> In `updTimers()` each sr timer > 0 is counted down via `deductCrement()` which resets `f.tIMElEFT` and, if timer is down to 0, masks `f.IStIMERoN` . The state of the timers is published  via `req.pubPrg()` if `f.cONNectd`.

### timers tsec NEW feature

`iot3/my` has already been improved in that prgs is disentangled from ports. You don't need a prog to turn a relay on or off. Now, in addition to being able to turn a relay on and off with simpler code, you can turn a relay on for a set amount of time, even just 1 second. The cmd to do it would look like `CYURD128/cmd {"id":3,"sra":[1],"tsec":23}` . For `srs.rel` types, if you put in a value for `tsec > 0` the command will ignore what's in "sra" and just turn on the relay for `tsec` seconds.

In `reqs.deseriCmd()case 2` the code that starts the timer and turns on the relay is 

      int bit = pow(2,id);
      f.HAStIMR=f.HAStIMR | bit;
      srs.rel[ici.idx].onoff = 1;
      digitalWrite(ports.port[id].out, 1);
      f.tIMElEFT[id]=tsec;

In `deductCrement()` called by `sched.updTimers()` the code that turns off the `srs.rel[x].onoff` when time runs out for a `f.HAStIMER` is

    /*tsec code for turning off srs.rel[x].onoff when times up*/
    iscsidx_t ici = req.getTypeIdx(id);
    int bit = pow(2,id);
    if((bit & f.HAStIMR)==bit){
      srs.rel[ici.idx].onoff = 0;
      int mask = 1023-bit;
      f.HAStIMR = f.HAStIMR & mask;
    }
    /*tsec code end */

Things to check are
1. initalize f.HAStIMER to 0;
2. Can you have a project with relays without programs?

### ckrelays() called every 2 seconds in loop

    every 2 seconds
      go thourgh all the ports (srs) that have an output port
      for cs, rel check the current state value and update port


### xdata NEW feature

A device like an outdoor reset might use a microcontroller at the boiler in combination with info from a diffeent microcontroller sensing outdoor temperature in a different part of the house. It can give permission to the boiler device to listen to its readings and act on them within a different application.

The data structure that stores the state of other devices is `xdata`. It can hold data from `numdevs` external devices in an `xda` array. The members of the array each have the `xda_t` structure which includes  an `xdev[17]` like `CYURD006/srstate` AND an srs_t structure to hold the sensors and relays that you want to listen to.

    struct xda_t {
      char xdev[17];
      srs_t xrs;
    };
    struct xdata_t {
      int numdevs;
      xda_t xda[1];
    };
    extern xdata_t xdata;

The project device can subscribe to another device by looking in `CONFIG.cpp` and scanning throught the `xdevs`. This happens in `MQclient::reconn(PubSubClient& client)`

    for (int i=0; i<=xdata.numdevs;i++){
      client.subscribe(xdata.xda[i].xdev);
    }

In the callback that runs when a new message comes in, an new `extern char idev[17]` is stored

    void handleCallback(char* topic, byte* payload, unsigned int length){
      strcpy(idev,topic);

When the subscribed NEW_MAIL comes in, `Reqs::processInc()`, after looking to see if the message is for the native device, runs `Reqs::getXdata(idev, ipayload)` looking for the `xdata.xda[x].xdev == xdevtpc (idev), then storing the data that it is interested in.

    void Reqs::getXdata(char* xdevtpc, char* xpayload){
      char xpayl[100];
      strcpy(xpayl,xpayload);
      for (int i=0; i<=xdata.numdevs;i++){
        if(strcmp(xdevtpc, xdata.xda[i].xdev)==0){
          DynamicJsonDocument rot(1000);
          deserializeJson(rot, xpayload);  
          int id = rot["id"];
          JsonArray darr = rot["darr"];
          srs_t xrs = xdata.xda[i].xrs;
          for (int i=0; i<xrs.numse;i++){
            if(xrs.se[i].sr==id){
              xrs.se[i].reading = darr[0];
              printf("xdevtpc:%s, xpayload:%s \n", xdevtpc, xpayl);
            }
          }
          for (int i=0; i<xrs.numcs;i++){
            if(xrs.cs[i].sr==id){
              xrs.cs[i].reading = darr[0];
              xrs.cs[i].onoff = darr[1];
              xrs.cs[i].hi = darr[2];
              xrs.cs[i].lo = darr[3];
              printf("xdevtpc:%s, xpayl:%s \n", xdevtpc, xpayl);
            }
          }
          for (int i=0; i<xrs.numrel;i++){
            if(xrs.rel[i].sr==id){
              xrs.rel[i].onoff = darr[0];
              printf("xdevtpc:%s, xpayl:%s \n", xdevtpc, xpayl);
            }
          }
        }
      }
    }

An actual project might have a relay operation influenced by some xdata as well as some native data. The xdata structure can be set up by appbuild.js. Once CONFIG.cpp is set up, custom code can be put there to do things like a difference controller.
 
## considered changes 

    ports_t ports {
      5, //numsr
      {//port:{sr, in, out, type, xd, label rec, isnew}
        {0, D2, -9, "se", -1, "temp2" 0, 0},// temp2 DS18B20
        {1, D5, -1, "cs", -1, "temp", 0, 0},// temp DHT11
        {2, D5, D7, "cs", -1, "hum", 0, 0},// hum DHT11
        {3, -9, D6, "rel" -1, "timr" 0, 0},// timr1 undefined
        {4, D3, D1, "cr", -1, "btn1, 0, 0} // btn1 NO-btn
        {5, -9, -1, "se", 0, "temp_out, 0, 0}// temp_out na
        {6, -1, D8, "di", -1, "out_reset, 0,0"},  
      }
    };

implications 

- when ckrelays goes through ports it could find multiple outs that are the same 
  - for diff only port 6 out reset matters 
- 


    srs_t srs  {
      5, // numsr 
      1, // se.length 
      { // se:{sr, reading} 
        {0, 45}  // temp2
      },
      2, // cs.length 
      { // cs:{sr, reading, onoff, hi, lo} 
        {1, 44, 0, 69, 67}, // temp
        {2, 24, 0, 38, 23}  // hum
      },
      1, // cr.length 
      { // cr:{sr, tsec, onoff} 
        {4, 1200, 0}  // btn1
      },
      1, // rel.length 
      { // rel:{sr, onoff} 
        {3, 0}  // timr1
      },
      1, // dif.length 
      { //dif: {sr, onoff, rdA, rdB, dif, hyst}
        {6, onoff, rdA, rdB, diff, hyst} // out_reset
      }
    };

ckrelays will need to be modified

    void Sched::ckRelays(){
      for (int i=0; i<ports.numports;i++){
        if (ports.port[i].out>=0){
          iscsidx_t ici = req.getTypeIdx(ports.port[i].sr);
          switch(ici.srtype){
            case 0:
              //cannot change a se reading from afar
              break;
            case 1:
              if(srs.cs[ici.idx].onoff != digitalRead(ports.port[i].sr)){
                digitalWrite(ports.port[i].out, srs.cs[ici.idx].onoff);
              }
              break;
            case di:
              if(srs.di[ici.idx].onoff != digitalRead(ports.port[i].sr)){
                digitalWrite(ports.port[i].out, srs.di[ici.idx].onoff);
              }
              break;

What else does ckRelays(di) need to do? How does srs.di[ici.idx].onoff get set based on dif controller? On every read

read buttons needs to happen every loop() How does read work for di(with xdata) and cr(buttons)?

readSensors goes through all the sensors
   
    const sen_t SE {
      3, // numstypes 
      4, // numsens 
      { // stype:{nums,{sra,srb...},type,model}
        { 1, { 0 }, "temp", "DS18B20" }, 
        { 2, { 1,2 }, "temp-hum", "DHT11" }, 
        { 1, { 4 }, "btn", "NO-btn" }  
      }
    };

    for(int i=0;i<SE.numtypes;i++){//check all the types incl.
      if(strcmp(SE.stype[i].model, "DS18B20")==0){
        for(int j=0;j<SE.stype[i].nums;j++) {
          read sensor
          get old value 
          setIfDif() => setSrs()
      if(strcmp(SE.stype[i].model, "NO-btn")==0){
        for(int j=0;j<SE.stype[i].nums;j++) {   
          read debouce
          get cr.onoff old value
          setIfDif() 
            set onoff, tsec 
            if on turn on timer for that sr
              f.tIMElEFT[id]=tsec+f.cREMENT;   

- What happens for button?
- How does xdata get set? 
  - NO xdata never gets set          
- How does cmd to change cr work?  
  - It could change onoff or tsec

case 2://rel CYURD128/cmd {"id":3,"sra":[1],"tsec":23}
    printf("tsec:%d \n",tsec);
    if (tsec>0){
      if id3 is "rel"
        int bit = pow(2,id);
        f.HAStIMR=f.HAStIMR | bit;
        srs.rel[ici.idx].onoff = 1;
        digitalWrite(ports.port[id].out, darr[0]);
        f.tIMElEFT[id]=tsec+f.cREMENT;  
 case cr       
      if id3 is "cr"
        int bit = pow(2,id);
        f.HAStIMR=f.HAStIMR | bit;
        srs.cr[ici.idx].onoff = darr[0];
        srs.cr[ici.idx].tsec = tsec
        digitalWrite(ports.port[id].out, darr[0]);
        f.tIMElEFT[id]=tsec+f.cREMENT;  

How does tsec f.timeleft turn it off?

#### how to find xdata value from ports
    ptypes_t ptypes
    char[5][3]

    types = {"se","cs","cr","rel","di"}
    typnd =    {1,4,2,1,5}
    maxpda = 5 + 3 //max(typnd)+3




    ports_t ports {
      5, //numsr
      {//port:{sr, in, out, type, xd, label rec, isnew}
        {0, D2, -9, "se", -1, "temp2" 0, 0},// temp2 DS18B20
        {1, D5, -1, "cs", -1, "temp", 0, 0},// temp DHT11
        {2, D5, D7, "cs", -1, "hum", 0, 0},// hum DHT11
        {3, -9, D6, "rel" -1, "timr" 0, 0},// timr1 undefined
        {4, D3, D1, "cr", -1, "btn1, 0, 0} // btn1 NO-btn
        {5, -10, -1, "se", 0, "temp_out, 0, 0}
    // {xsr, in-10=sr, grp-1, type, datxidx, label, rec, isnew}
        {6, -1, D8, "di", -1, "out_reset, 0, 0"},  
      }
    };

if you know are scanning ports and get to and sr with and xd>-1 you have to find which xda[] it is in and then which sr it is from there
- xd could be xda[idx]
- sr could be -(in-10)
- xdata[xd][type][10-in] if you could
    for allports 
      if xd>=0
        xrs xda[xd].xrs
        ici = getTypIdx(10-in, xda[xd].xrs)
        if(strcmp(ports[sr].type,"se")===0)
          reading = xda[xd].xrs.se.reading
        if(strcmp(ports[sr].type,"cs")===0)
        if(strcmp(ports[sr].type,"cr")===0)
        if(strcmp(ports[sr].type,"rel")===0)
        if(strcmp(ports[sr].type,"di")===0)

#### adjRelay for diff ctrl (with xdata)
  adjRelays (di)

#### alt xdata data stru
    const ${numxdt}

    struct xdevtpc_t {
      int numxdt;
      char xdtstrarr[${numxdt}][20];
    } 

    const xdevtpc_t xdevtpc {
      ${numxdt},
      {"CYURD006/ststate", "CYURD018/srstate"}//datx

    char datax[${numxdt}][8] //declaration

    datax = {// fill not used with -9
      {7, 1, 1, 0, 55, 44, 10, 1, 1},//{xsr, devtpc, osr, onoff, rdA, rdB, diffd, hyst,gd}
      {5, 0, 0, 56, -9, -9, -9, -9 , -9}//{xsr, devtpc, osr, reading}
    }

    int xdata = getXdata(int devidx, int sr, int datidx){
      darr = datax[devidx]
      for (int i=0;i<=xdevtpc, i++ ){
        if(sr==darr[i][1]){
          return darr[i][3] //returns reading
        }
      }
    }

    bool isThisXdata(idev){
      bool ret == false
      if (strcmp(devid, idev.substr(0,8))==1)
        ret = true
      return ret  
    }


    void setXdata(){
      dtidx = findTopicSrIdx(tpc, osr)
        for i numdxtpc
          if stcmp (tpc, datax[i])
            tpcidx = i
        for j -> numxdt
          if datax[j][1] == tpcidx  &&  datax[j][2]== osr
            return j  
      typidx = findtyp(datax[dtidx][xsr]) //"se","cs"etc OR just the idx
        type = ports[xsr].type
        for k types
          if strcmp(type, types[k]) 
          return k   
      nd = typnd[typidx]
      for i to nd
        for y = i, <nd
        datax[dtidx][i+2] = osr
        datax[dtidx][i+3] = darr[i]
    }

    void ckDif(){ //set rdA, rdB, diff, hyst,gd and onoff
      for ports
        [sr, port sra, srb]
        if port.type == "di"{
          sr = port.sr
          port= port.out           
          ab = port.in 
          for ports
            if port.out = ab -> 
                sra = port.sr
                char typa = port.type
                xdia = port.xd
                for the rest of ports 
                  if(port.out = ab)
                      srb = port.sr
                      char typb = port.type
                      xdib = port.xd
        rdA = srs.di[0].rdA = getRd(sra, typa, xdia)              
        rdB = srs.di[0].rdB = getRd(srb. typeb, xdia)
        diff = srs.di[0].diff
        hyst = srs.di[0].hyst 
        onoffOld = srs.di[0].onoff //old
        gd = srs.di[0].gd //gd=1->greater difference turns on relay
        if (abs(rdA-rdB) >= dif+hyst) && onoffOld == !gd
          srs.di[0].onoff =gd  
        }
        if (abs(rdA-rdB) < dif- hyst) && onoffOld == gd
          srs.di[0].onoff =!gd  
        }
        if readPort(port.out) != srs.di[0].onoff{
          writePort(port.out, srs.di[0].onoff)
        }
      } 
    }

If being rdA-rdB is greater than dif causes you to wan to turn on the relay

     102 >= 100+2 on 102 !< 100-2 x
     101 !>= 100+2 x 101 !< 100-2 x
     103 >= 100+2 on 103 !< 100-2 x
     100 !>= 100+2 x 100 !< 100-2 x
     98 !>= 100+2 x 98 !< 100-2 x
     97 !>= 100+2 x 97 !< 100-2 off
     101 !>= 100+2 x 101 !< 100-2 x
     103 >= 100+2 on 103 !< 100-2 x

- A greater diff might want you to turn relay on
- A greater diff might want you to turn relay off

    struct di_t {//diff control
        int sr;
        int onoff;
        int rdA;
        int rdB;
        int dif;
        bool hyst;
        bool gt;
    };


    int getRd(sr, typ, xdi){
      if xdi==-1
        return getStoredReading(sr)
      if xdi >=0
        for j -> numxdt
          if datax[xdi][1] == tpcidx  &&  datax[idx][0]== sr
            return datax[xdi][3] 
    }

    int sztd = 6;

    struct td_t {
      char type[8];
      int numd;
      char* dname[7];
    } 

    td_t td[6] = 
    {
      {"se", 1, {"reading"}},
      {"cs", 1, {"reading", "onoff", "hi", "lo"}},
      {"cr", 1, {"tsec, "onoff"}},
      {"rel", 1, {"onoff"}},
      {"di", 1, {"onoff", "rdA", "rdB", "dif", "hyst", gd"}},
    }


    int getStoredValue(int sr, char* typ, int xdi, char* dnam){
      typidx = getTypeDataIdx(typ , dname)
      if xdi==-1
        return getStoredReading(sr)
      if xdi >=0
        for j -> numxdt
          if datax[xdi][1] == tpcidx  &&  datax[idx][0]== sr
            return datax[xdi][3] 
    }

    int getTypIdx(char* type){
      int ret;
      for (int i =o; i<sztd;i++){
        if(strcmp(td[i].type, type)==0){
          ret =i;
        }
      }
      return ret;
    }

### overall changes

xdata
For each external data array an sr is created in ports. A port.xd>-1 indicates that it is external data. Port.type indicates what type of data array it is. External data never has an input port.

You could have external data controlling an array. In that case there is an output port and the onoff value of the external data sets the relay.

    {5, -10, D6, "rel", 0, "temp_out, 0, 0}

Here the ports sr is 5, the output port is D6, the data structure is of the "rel" type and it is the [0]'th entry in the xdevtpc.datax array. The input port is used to encode the original sr (osr) of the external data as osr-10. Any time the external data changed it would get processed incoming. When the port state is different than the datax onoff value it should change the relay.


  and Port stores {xsr, in-10=sr, grp-1, type, datxidx, label, rec, isnew}stores the  has an original device and sr as well and get 
- if xdata comes in `processInc()-> if(isThisXdatya(idev)) {setXdata(idev,ipayload)}` puts it into datax 
- xdata is only used internally on the device
- xdata is only relavent if connected


ports

## messages



 CYURD128/flags {"aUTOMA":true,"fORCErESET":false,"cREMENT":5,"HAStIMR":1023,"IStIMERoN":0,"HAYpROG":1023,"HAYsTATEcNG":1023,"CKaLARM":0,`"ISrELAYoN":4`,"tIMElEFT":[0,0,0,0,0]}





 