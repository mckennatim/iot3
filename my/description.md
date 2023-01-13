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
- ti: relays controlled by timers. The sr ID is the same as the prg sr ID. The relay's port is listed in prg. 
  - {sr, onoff...}
- di: difference controller  

Since both cs a ti's have an associated program, why do they need hilimit/lolimit or onoff in the srs structure?

Because it is the place that stores the the current and default values. 

## what happens in the loop

During the loop a message could come in from the outside. Or there could be a change in one of the sensor values. Or a running timer can display its progress or could reach the end of its run. Or it might be time to change a program. Or check the difference controller or custom code.

Sensors are read every loop. readSensors() calls setIfDif() which sets HAYsTATEcNG. 

Loop watches for HAYsTATEcNG, runs pubState() if it is >0.

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

### sent by app

`cmd` sends a cmd that overides the hi/lo or onoff of the currently running schedule]

    CYURD002/cmd{id:sr, sra:[1] or sra:[69,67]}

`devtime`

`req`

`prg`

`set`

dogmommess

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
      



Possible setups

If wifi goes out during the day, try to reconnect the next day. If mq disconnect doesn't reconnect after 5 tries, ignore the internet until the next day. 

What if MQTT is not connected, you would eventually want to ignore it and then move on with the default schedule. Maybe every time it tries to run mq.reconn() it starts a timer? If its been trying for 4 minutes then stop trying 


cONNECTED = haywifi & haymqtt

doDefault

* read sensors every 2 seconds
* check schedules 
* run schedules


while connected

* check MAIL
* check f.HAYsTATEcNGG