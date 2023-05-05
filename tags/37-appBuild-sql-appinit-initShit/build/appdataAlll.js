import * as c from './constants.js' 



const devinfo = {
  "devid": "CYURD128",
  "owner": "tim@sitebuilt.net",
  "pwd": "geniot",
  "mqtt_server": "sitebuilt.net",
  "mqtt_port": "1884"
}

const apploc ={
  "appid": "switch",
  "locid": "12ParleyVale",
  "user": "mckenna.tim@gmail.com",
  "descr": "demo of long board with 2 switches controlling relays, and 2 DSB1820",
  "board": "long"
}

const cfgdata =
{
  "srs":[
    {
      "sr": 0,
      "label": "solar",
      "out": "D8",
      "ctype": "difctrl",
      "init": [0,67,45,9,1],
      "prg": [[0,0,11], [9,15,10]],
      "descr": "solar dif ctrl",
      "rec": true,
      "timr": true
    },
    {
      "sr": 1,
      "label": "dr",
      "out": "D6",
      "ctype": "tstat",
      "init": [0,35,42,1],
      "prg": [[0,0,68]],
      "descr": "dining room tstat",
      "rec": true,
      "timr": true
    },
    {
      "sr": 1,
      "label": "bbq",
      "out": "D6",
      "ctype": "ctrl",
      "init": [0,35,42],
      "prg": [[0,0,68]],
      "descr": "bbq temp",
      "rec": true,
      "timr": true
    },
    {
      "sr": 2,
      "label": "sw1",
      "out": "D7",
      "ctype": "relay",
      "init": [0,120],
      "prg": [[0,0,68]],
      "descr": "light switch",
      "rec": true,
      "timr": true
    },    
    {
      "sr": 2,
      "label": "currents",
      "out": "D7",
      "ctype": "array",
      "init": [14,12,56,45],
      "descr": "current readings",
      "rec": true,
      "timr": true
    }    
  ],
  "inputs":[
    {
      "ports": ["D2"],
      "itype": "1-wire",
      "sensors": 3,
      "targets": [
        [[0,2]],
        [[1,1],[3,1]],
        [[2,1]]
      ]
    },
    {
      "ports": ["D2"],
      "itype": "dht",
      "sensors": 2,
      "targets": [
        [[1,1],[3,1]],
        [[2,1]]
      ]
    },
    {
      "ports": ["D1"],
      "itype": "switch",
      "sensors": 1,
      "targets": [
        [[2,0]]
      ],//
      "actions": [200]
    },
    {
      "ports": ["D1"],
      "itype": "butn",
      "sensors": 1,
      "targets": [
        [[2,0]]
      ],//sval,lval,prev,lptime
      "actions": [-1, -10, 0, 500]
    },
  ],
  "xdata": 
  {
    "CYURD006":
    [
      {
        "sr": 2, 
        "dt":[
          { 
            "xdidx": 2,
            "targets": [[0,1]]
          },
          { 
            "xdidx": 1,
            "targets": [[1,1]]
          }
        ]
      },
      {
        "sr": 0, 
        "dt":[
          { 
            "xdidx": 0,
            "targets": [[2,1]]
          }
        ]
      }
    ],
    "CYURD118":
    [
      {
        "sr": 3, 
        "dt":[
          { 
            "xdidx": 3,
            "targets": [[3,2]]
          }
        ]
      }
    ]
  }
}


const senstypes =["butn", "switch", "1-wire", "dht", "i2c", "spi","ext"]
const  ctrltypes= {
  "difctrl":{
    "lab":["onoff", "rdA", "rdB", "dif", "difgt0"],
    "cmd":[3],
    "prg":[3],
  },
  "tstat":{
    "lab":["onoff", "reading", "setting", "dif"],
    "cmd":[0,2,3],
    "prg":[3],
  },
  "relay":{
    "lab":["onoff", "tsec"],
    "cmd":[0,1],
    "prg":[3],
  },
  "ctrl":{
    "lab":["bell", "reading", "setting"],
    "cmd":[2],
    "prg":[2],
  },
  "array":{
    "lab":["a0", "a1", "a2", "a3", "a4", "a5", "a6", "a7"],
    "cmd":[],
    "prg":[],
  },
  "se":{
    "lab":["reading"],
    "cmd":[],
    "prg":[],
  },
  "cs":{
    "lab":["onoff", "reading", "hi", "lo"],
    "cmd":[0,2,3],
    "prg":[2,3],
  },
  "re":{
    "lab":["onoff"],
    "cmd":[0],
    "prg":[0],
  }
}
const topics= ["devtime", "cmd", "prg", "req", "set"]  


export {cfgdata, devinfo, apploc, senstypes, ctrltypes, topics}
