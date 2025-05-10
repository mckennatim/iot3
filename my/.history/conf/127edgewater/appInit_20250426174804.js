/*  
temp_out: darr:[]
tstat: {pro:[[0,0,65,63]], timeleft:0, darr:[reading,onoff,66,64]},
tv: {pro:[[0,0,hi,lo]], darr:[reading,onoff,hi,lo]},
pond: {pro:[[0,0,0],[19,15,1]], timeleft:0, darr:[0,0,0]},
//solar: darr:[sra_rdg, srb_rdg, difon, difoff,maxa, maxb onoff]
*/
const initialState = {//pro must start at 0,0
    zone1: {darr: [0,15,1,-1,-1] },
    zone2: {darr: [0,15,2,0,0] , timeleft: 0},
    zone3: {darr: [0,15,3,1,0] },
    zone4: {darr: [0,15,-1,2,0] },
    zone5: {darr: [0,15,-1,-1,-1] },
    temp_out: {darr: [0]},
    temp_attic: {darr: [0]},
    gh_hum: {darr: [0]}
}

const ctrltypes = 
{
  "difctrl": {
    "lab": [
      "onoff",
      "rdA",
      "rdB",
      "dif",
      "difgt0"
    ],
    "cmd": [
      3
    ],
    "prg": [
      3
    ]
  },
  "tstat": {
    "lab": [
      "onoff",
      "reading",
      "setting",
      "dif"
    ],
    "cmd": [
      0,
      2,
      3
    ],
    "prg": [
      3
    ]
  },
  "relay": {
    "lab": [
      "onoff",
      "tsec",
      "nxtsr",
      "priosr",
      "prionoff"
    ],
    "cmd": [
      0,
      1
    ],
    "prg": [
      1
    ]
  },
  "ctrl": {
    "lab": [
      "bell",
      "reading",
      "setting"
    ],
    "cmd": [
      2
    ],
    "prg": [
      2
    ]
  },
  "array": {
    "lab": [
      "a0",
      "a1",
      "a2",
      "a3",
      "a4",
      "a5",
      "a6",
      "a7"
    ],
    "cmd": [],
    "prg": []
  },
  "se": {
    "lab": [
      "reading"
    ],
    "cmd": [],
    "prg": []
  },
  "cs": {
    "lab": [
      "onoff",
      "reading",
      "hi",
      "lo"
    ],
    "cmd": [
      0,
      2,
      3
    ],
    "prg": [
      2,
      3
    ]
  },
  "re": {
    "lab": [
      "onoff"
    ],
    "cmd": [
      0
    ],
    "prg": [
      0
    ]
  }
}  

const appid = "edgewater" 

const dev1 = "CYURD127" 

export {initialState, appid, ctrltypes, dev1}
