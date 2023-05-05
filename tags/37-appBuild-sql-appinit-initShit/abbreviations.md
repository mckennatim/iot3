# abreviation

## sensors

- htu: [HTU21D temp and humidity sensor](https://www.adafruit.com/product/1899) 
- DS18B20: [DS18B20 1-wire thermometer](https://www.analog.com/media/en/technical-documentation/data-sheets/ds18b20.pdf)
- DHT11 DHT12: [DHT11 DHT12 AM2302 temp/hum sensors](https://learn.adafruit.com/dht)

## ports
portnames are built into the software as constants

Allowed ports for esp8266 and ports used in the 8266_2in-4out board are shown in [boards and ports](./boards-ports.md)   

output
 - D1, D2, D3, D4, D5, D6, D7 
 
input
- D1, D2, D5, D6, D7 
- D3, D4 are pulled up but OK for input
- A0 is the one analog input

Ports are described and specified in the program by a data struct similar to...

      /*ports for input and output
      */
      ports_t ports {
        4, //numsr
        {//port:{sr, in, out, rec, isnew}
          {0, D2, -9, 0, 0},// temp2 DS18B20
          {1, D5, D8, 0, 0},// temp DHT11
          {2, D5, D7, 0, 0},// hum DHT11
          {3, -9, D6, 0, 0} // timr1 undefined
        }
      };

Each `sr` (sensor/relay) is listed with their ports. Ports are either used for input or output. Input ports take in sensor readings and button pushes and the like. Outputs contol relays, led's etc. `rec` is a boolean which indicates to the project whether or not you want to record changes in values in a database.    

## abbreviations
used in data sturctures, MQTT, appdata.js and web apps
srs
- sr: sensor or relay ID
- se: sensors only, not controlling relays
  - {sr, reading...}
- cs: relays controlled by sensors. The sr ID is the same as the prg sr ID. A prg with 1 event with [hr,min,hi,lo] = [0,0,hilimit,lolimit] runs that all day
  - {sr, reading, onoff, hi, lo...} these are the default hilimit & lolimit.
- rel: relays controlled by timers. The sr ID is the same as the prg sr ID. The relay's port is listed in ports. 
  - {sr, onoff...}
- di: difference controller  (not yet implemented)
- numsr: total number of sensors and relays
- numse: number of sensors (not controlled)
- numcs: number of sensors operating relays
- numrel: number of relays on timers
- numdi: number of difference controllers, each has a relay
- srs: structure holding all of the above information

prgs
- prg: a day's program for a particular relay
  - {sr,aid,ev,numdata,prg[[hr,min,max,min]],port,hms}
  - ev: a day's program has a number of events. 
  - prg[11][4]: an 2d array. 11 represents the max # of events allowed/allocated. 4 represents the maxdata allowed/allocated for each event Each event happens at an [hr,min ...]. Besides hr,min, the event can either turn a relay onoff [hr,min,onoff] or it can set a min/max or hilimit/lowlimit [hr,min hi, lo] for a sensor&relay
  - numdata: can be  either 1 or 2. 1 means [hr,min,onoff], 2 means [hr,min,hi,low]
  - port: output port connected to relay 
  - hms: the hour and minute to trigger timer to load the days programs from the server
  - timeleft - an array holding how much time is left for a running program that has turned something on
- prgs: structure holding all the prg's operating on relays or senor&relay
  - numprgs: how many prg operating on relays or sensors&relays 
  - prg[numprgs]: an array of multiple prg
    
