const devinfo={
  "devid": "CYURD127",
  "owner": "tim@sitebuilt.net",
  "pwd": "geniot",
  "mqtt_server": "sitebuilt.net",
  "mqtt_port": "1884"
}

const apploc={
  "appid": "edgewater",
  "address": "12 Parley Vale, Jamaica Plain, MA 02130",
  "user": ["mckenna.tim@gmail.com", "noahmckenna@gmail.com"],
  "descr": "use toggle button to start sequentially timed relays controlling irrigation",
  "board": "long",
  "addrinfo": {
    "locid": "12ParleyVale",
    "lat": 42.31483255,
    "lon": -71.11045164674289,
    "tzone": "America/New_York"
  }
}

const cfgdata={
  "srs": 
  [
    {
      "sr": 0,
      "label": "zone1",
      "out": "D0",
      "ctype": "relay",
      "init": [0,15, 1, -1, -1],//[onoff,tsec,nxtsr,priosr,prionoff]
      "descr": "irrigation zone 1",
      "rec": false,
      "timr": false
    },    
    {
      "sr": 1,
      "label": "zone2",
      "out": "D6",
      "ctype": "relay",
      "init": [0,15,2,0,0],//[onoff,tsec,nxtsr,priosr,prionoff]
      "descr": "irrigation zone 2",
      "rec": false,
      "timr": false
    },    
    {
      "sr": 2,
      "label": "zone3",
      "out": "D7",
      "ctype": "relay",
      "init": [0,15,3,1,0],//[onoff,tsec,nxtsr,priosr,prionoff]
      "prg": [[0,0,0]],
      "descr": "irrigation zone 3",
      "rec": false,
      "timr": false
    },    
    {
      "sr": 3,
      "label": "zone4",
      "out": "D8",
      "ctype": "relay",
      "init": [0,15,-1,2,0],//[onoff,tsec,nxtsr,priosr,prionoff]
      "descr": "irrigation zone 4",
      "rec": false,
      "timr": false
    },    
    {
      "sr": 4,
      "label": "zone5",
      "out": "D1",
      "ctype": "relay",
      "init": [0,15,-1,-1,-1],//[onoff,tsec,nxtsr,priosr,prionoff]
      "descr": "special zone",
      "rec": false,
      "timr": false
    },    
  ],
  "inputs": 
  [
     {
       "ports": ["D3"],
       "itype": "toggle",
       "sensors": 1,
       "targets": [
         [[0,0]]
       ],
       "actions": [200]//not yet used
     }, ],
  "xdata": {  
    "CYURD006":
    [
      {
        "sr": 0, 
        "label": "temp_out",
        "descr": "outside temperature",
        "dt":[
          { 
            "xdidx": 0,
            "targets": [[2,1]]
          }
        ]
      },
      {
        "sr": 1, 
        "label": "temp_attic",
        "descr": "attic temperature",
        "dt":[
          { 
            "xdidx": 0,
            "targets": [[2,1]]
          }
        ]
      }
    ],
    "CYURD018":
    [
      {
        "sr": 0, 
        "label": "gh_hum",
        "descr": "greenhouse humidity",
        "dt":[
        ]
      }
    ],
  }
}

const senstypes=[
  "butn",
  "switch",
  "1-wire",
  "dht",
  "i2c",
  "spi",
  "toggle"
]

const ctrltypes={
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

const topics=[
  "devtime",
  "cmd",
  "prg",
  "req",
  "set"
]


export { cfgdata, devinfo, apploc, senstypes, ctrltypes, topics };

