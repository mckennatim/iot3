/*dog is good*/
import * as c from '../../build/constants.js' 



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
  "srs":[//sr must match index
    { 
      "sr": 0,
      "label": "lr",
      "out": "D6",
      "ctype": "tstat",
      "init": [0,35,42,1],//"onoff", "reading", "setting", "dif"
      "prg": [[0,0,68]],
      "descr": "dining room tstat"
    },
    {
      "sr": 1,
      "label": "tv",
      "out": "D6",
      "ctype": "tstat",
      "init": [0,65,72,1],
      "prg": [[0,0,68]],
      "descr": "dining room tstat"
    },
    {
      "sr": 2,
      "label": "sw1",
      "out": "D7",
      "ctype": "relay",
      "init": [0],
      "descr": "light switch",
    }    
  ],
  "inputs":[
    {
      "ports": ["D2"],
      "itype": "1-wire",
      "targets": [
        [[0,1]],
        [[1,1]]
      ],
      "actions": [0]
    },
    {
      "ports": ["D3"],
      "itype": "switch",
      "targets": [
        [[2,0]]
      ],
      "actions": [200]
    }
  ],
  "xdata": {}
}


const senstypes =["butn", "switch", "1-wire", "dht", "i2c", "spi", "toggle"]
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
