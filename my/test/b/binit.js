const initialState = {
  tank_panel: {onoff:0, rdA:32, rdB:31, dif:10,difgt0:1},
  lr: { darr: {onoff:0, reading:44, setting:69, dif:2} },
  dr: { darr: {onoff:0, reading:44, setting:69, dif:2} },
  door_strike: { darr: {onoff:1, tmsec:200}},
  bbq: { darr: {onoff:0, reading:45, setting:130}},
  hum: { darr: {reading:24, onoff:0, hi:38, lo:23}},
  timr1: { darr: [0], pro: [{hr:0, min:0, onoff:0]], timeleft: 0},
  readings: { darr: {temp_out:55, temp_attic:54, hum_attic:23}},
  currents: {darr: {fridge:0, dw:0, wd:10, boiler:12}}
}