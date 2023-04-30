const devinfo={
  "devid": "CYURD127",
  "owner": "tim@sitebuilt.net",
  "pwd": "******",
  "mqtt_server": "sitebuilt.net",
  "mqtt_port": "1884"
}

const apploc={
  "appid": "edgewater",
  "address": "12 Parley Vale, Jamaica Plain, MA 02130",
  "user": "mckenna.tim@gmail.com",
  "descr": "use toggle button to start sequentially timed relays controlling irrigation",
  "board": "long",
  "addrinfo": {
    "locid": "12ParleyVale",
    "lat": 42.31483255,
    "lon": -71.11045164674289,
    "tzone": "America/New_York"
  }
}

const cfgdata = {
  srs: [ //sr must match index
    {
      "sr": 0,
      "label": "power",
      "out": "D1",
      "ctype": "power",
      "init": [1],//[onoff]
      "descr": "power (led)",
    },    
    {
      "sr": 1,
      "label": "zone1",
      "out": "D6",
      "ctype": "relay",
      "init": [0,120,2],//[onoff,tesc,(dsr)]
      "prg": [[0,0,68]],
      "descr": "irrigation zone 1",
    },    
    {
      "sr": 2,
      "label": "zone2",
      "out": "D7",
      "ctype": "delay",
      "init": [0,120,1,0],//[onoff,tesc, rsr, prior]
      "prg": [[0,0,68]],
      "descr": "delayed irrigation zone 2",
    },    
  ],
  inputs: [
    {
      "ports": ["D5"],
      "itype": "toggle",
      "sensors": 1,
      "targets": [
        [[1,0]]
      ],
      "actions": [200]//not yet used
    }
  ],
  xdata: {
  },
};

const senstypes = ["butn", "switch", "1-wire", "dht", "i2c", "spi", "toggle"];
const ctrltypes = {
  difctrl: {
    lab: ["onoff", "rdA", "rdB", "dif", "difgt0"],
    cmd: [3],
    prg: [3],
  },
  tstat: {
    lab: ["onoff", "reading", "setting", "dif"],
    cmd: [0, 2, 3],
    prg: [3],
  },
  relay: {
    lab: ["onoff", "tsec", "dsr"],
    cmd: [0, 1],
    prg: [1],
  },
  delay: {
    lab: ["onoff", "tsec", "rsr", "prior"],
    cmd: [0, 1],
    prg: [1],
  },
  ctrl: {
    lab: ["bell", "reading", "setting"],
    cmd: [2],
    prg: [2],
  },
  power: {
    lab: ["onoff"],
    cmd: [0],
    prg: [],
  },
  array: {
    lab: ["a0", "a1", "a2", "a3", "a4", "a5", "a6", "a7"],
    cmd: [],
    prg: [],
  },
  se: {
    lab: ["reading"],
    cmd: [],
    prg: [],
  },
  cs: {
    lab: ["onoff", "reading", "hi", "lo"],
    cmd: [0, 2, 3],
    prg: [2, 3],
  },
  re: {
    lab: ["onoff"],
    cmd: [0],
    prg: [0],
  },
};
const topics = ["devtime", "cmd", "prg", "req", "set"];

export { cfgdata, devinfo, apploc, senstypes, ctrltypes, topics };

