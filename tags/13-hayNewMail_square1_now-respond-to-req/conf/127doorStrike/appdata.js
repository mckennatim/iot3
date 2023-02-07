const cfgdata =
{
  "CYURD127": [
    {
      "sr": 0,
      "label": "contact",
      "type": "se",
      "senses": "onoff",
      "model": "NCcontact",
      "in": "D5",
      "reading" : 1,
      "rec": 1,
      "descr": "a short description"
    },

    {
      "sr": 1,
      "label": "strike",
      "type": "rel",
      "out": "D1",
      "onoff" : 0,
      "hayprg": 1,
      "rec": 1,
      "descr": "a short description"
    },
    {
      "sr": 2,
      "label": "ledRed",
      "type": "rel",
      "out": "D8",
      "onoff" : 1,
      "rec": 0,
      "descr": "a short descrition"
    },
    {
      "sr": 3,
      "label": "ledGreen",
      "type": "rel",
      "out": "D7",
      "onoff" : 0,
      "rec": 0,
      "descr": "a short descrition"
    },
    {
      "sr": 4,
      "label": "ledBlue",
      "type": "rel",
      "out": "D6",
      "onoff" : 0,
      "rec": 0,
      "descr": "a short descrition"
    },
    {
      "sr": 5,
      "label": "lr",
      "type": "cs",
      "senses": "temp",
      "model": "ds18b20",
      "in": "D0",
      "out": "D4",
      "reading" : 44,
      "onoff": 0,
      "hi": 69,
      "lo": 67,
      "hayprg": 1,
      "rec": 1,
      "descr": "a short descrition"
    },
    {
      "sr": 6,
      "label": "mb",
      "type": "cs",
      "senses": "temp",
      "model": "ds18b20",
      "in": "D0",
      "out": "D1",
      "reading" : 33,
      "onoff": 0,
      "hi": 70,
      "lo": 40,
      "hayprg": 1,
      "rec": 1,
      "descr": "a short descrition"
    }
    // {
    //   "sr": 7,
    //   "label": "solardif",
    //   "type": "dif",
    //   "out": "D9",
    //   "sra": 5,
    //   "srb" : 6,
    //   "difon": 42,
    //   "difoff": 48,
    //   "maxa": 170,
    //   "maxb": 140,
    //   "onoff" : 0,
    //   "hayprg" : 1,
    //   "rec": 1
    // },
  ],
  "CYURD006": [
    {
      "sr": 0,
      "label": "temp_out",
      "descr": "outside temperature"
    }
  ]
}

const devinfo = {
  "devid": "CYURD127",
  "owner": "tim@sitebuilt.net",
  "pwd": "geniot",
  "mqtt_server": "sitebuilt.net",
  "mqtt_port": "1884"
}

const apploc ={
  "appid": "doorStrike",
  "locid": "255ChestnutAve",
  "user": "mckenna.tim@gmail.com",
  "descr": ""
}

export {cfgdata, devinfo, apploc}
