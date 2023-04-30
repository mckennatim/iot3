const devinfo = {
  devid: "CYURD127",
  owner: "tim@sitebuilt.net",
  pwd: "******",
  mqtt_server: "sitebuilt.net",
  mqtt_port: "1884",
};

const apploc = {
  appid: "edgewater",
  address: "12 Parley Vale, Jamaica Plain, MA 02130",
  user: "mckenna.tim@gmail.com",
  descr:
    "use toggle button to start sequentially timed relays controlling irrigation",
  board: "long",
};

const cfgdata = {
  srs: [
    //sr must match index
  ],
  inputs: [

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

