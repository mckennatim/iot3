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

typical uses in secstidif are:

### PORTS for INPUT

    const portsin_t inpo {
      D2, //DS18b20a
      D1, //DS18b20b
      D5, //dht11
      A0, //ANALOG
      D6, //SPIdo
      D8, //SPIcs
      D7};//shares i2c D2 and D1
   
 ### PORTS for OUTPUT

 - Each prg has a PORT
   - Each cs(sr) is associated with a prg(sr), even if it is the same prg all day
   - Each ti(sr) is associated with a prg(sr), even if it is the same prg all day
 - Each difference controller has a PORT
 

## structs and mqtt 
srs
- sr: sesnor or relay ID
- se: sensors only, not controlling relays
  - {sr, reading...}
- cs: relays controlled by sensors. The sr ID is the same as the prg sr ID. The relay's port is listed in prg. A prg with 1 event with [hr,min,hilimit,lolimit] = [0,0,hilimit,lolimit] runs that all day
  - {sr, reading, onoff, hilimit, lolimit...} these are the default hilimit & lolimit.
- ti: relays controlled by timers. The sr ID is the same as the prg sr ID. The relay's port is listed in prg. 
  - {sr, onoff...}
- di: difference controller  
- numsr: total number of sensors and relays
- numse: number of sensors (not controlled)
- numcs: number of sensors operating relays
- numti: number of relays on timers
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
- prgs: structure holding all the prg's operating on relays or senor&relay
  - numprgs: how many prg operating on relays or sensors&relays 
  - prg[numprgs]: an array of multiple prg
    
