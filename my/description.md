## getting connected
`ConnWIFI.getOnline()` is first called in `setup()`. It starts the `WIFImanager`. If the WIFImanager doesn't succeed in connecting to its saved credentials it starts `connectEspAP` on 192.168.4.1. If that doesn't complete in 120 seconds then the flag `haywifi=0` and the program goes on with its default schedule.



#### problem with losing power and losing connection
Once you lose the wificonnection the mq.reconn will just be in and endless loop


## explanation of the design
from [abbrevaitions.md](abbreviations.md):

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

## what happens in the loop

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


## functional flow

MQTT fires a `handleCallback` when a message is received which sets NEW_MAIL and puts the message topic and payload in extern (global) `itopic` and `ipayload`

`req.processInc()` runs from loop() when there is NEW_MAIL

responding to incoming prg message

- CYURD073/prg {"id":0,"pro":[[0,0,61,59],[9,0,62,60]]} JsonArr `[[0,0,61,59],[9,0,62,60]]` looks for the sr in prgs that matches the id of MQTTmess. Then it sets the f.CKaLARM bit and copies the prg and JsonArr. into prg.

      req.processInc(ipayload) => 
          sched.deseriProg(kstr) => 
              set:f.CKaLARM for sr
              sched.copyProg(prgs.prg[i], events) 

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

## timers
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



## messages

 CYURD128/flags {"aUTOMA":true,"fORCErESET":false,"cREMENT":5,"HAStIMR":1023,"IStIMERoN":0,"HAYpROG":1023,"HAYsTATEcNG":1023,"CKaLARM":0,`"ISrELAYoN":4`,"tIMElEFT":[0,0,0,0,0]}




 