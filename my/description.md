## explanation of the design

Since both cs a ti's have an associated program, why do they need hilimit/lolimit or onoff in the srs structure?

Because it is the place that stores the the current values. 

## what happens in the loop

During the loop a message could come in from the outside. Or there could be a change in one of the sensor values. Or a running timer can display its progress or could reach the end of its run. Or it might be time to change a program. Or check the difference controller or custom code.

Sensors are read every loop. readSensors() calls setIfDif() which sets HAYsTATEcNG. 

Loop watches for HAYsTATEcNG, runs pubState() if it is >0.

## topics of communication
### sent by device

`srstate` is sent whenever the sensor or relay value changes or if setpoints like hilimit or lolimit are changed. 

    CYURDOO4/srstate {"id":0, "darr":[64, 1, 81, 78], "new":0}

`sched` sends the schedule current running for a particular sesnor or relay. One of the devices built in timers is reserved for downloading the day's schedule from the mqtt broker->database at around midnight every day.
    
    CYURDOO4/sched {"id":2,"aid":2,"ev":3,"numdata":1,"pro":[[0,0,1],[7,45,0],[19,0,1]]}

`timr` sends and array of timeleft on all relays that are run by a timers. "IStIMERoN" and "ISrELAYoN" is a binary representation of which relays and timers are currently on.

    CYURD002/timr{"cREMENT":5,"IStIMERoN":4,"ISrELAYoN":5,"tIMElEFT":[0,0,26215,0,0]}