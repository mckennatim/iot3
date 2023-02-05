const cfgdata = 
{
  "CYURD128": [
    {
      "sr": 0,
      "label": "contact",
      "type": "se",
      "senses": "onoff",
      "model": "NCcontact",
      "in": "D5",
      "reading": 1,
      "rec": 1,
      "descr": "a short descriton"
    },
    {
      "sr": 1,
      "label": "strike",
      "type": "rel",
      "out": "D1",
      "onoff": 0,
      "hayprg": 1,
      "rec": 1,
      "descr": "a short descriton"
    },
    {
      "sr": 2,
      "label": "ledRed",
      "type": "rel",
      "out": "D8",
      "onoff": 1,
      "rec": 0,
      "descr": "a short descrition"
    },
    {
      "sr": 3,
      "label": "ledGreen",
      "type": "rel",
      "out": "D7",
      "onoff": 0,
      "rec": 0,
      "descr": "a short descrition"
    },
    {
      "sr": 4,
      "label": "ledBlue",
      "type": "rel",
      "out": "D6",
      "onoff": 0,
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
      "reading": 44,
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
      "reading": 33,
      "onoff": 0,
      "hi": 70,
      "lo": 40,
      "hayprg": 1,
      "rec": 1,
      "descr": "a short descrition"
    }
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
  "devid": "CYURD128",
  "owner": "tim@sitebuilt.net",
  "pwd": "geniot",
  "mqtt_server": "sitebuilt.net",
  "mqtt_port": "1884"
}

const apploc ={
  "appid": "doorStrike1",
  "locid": "255ChestnutAve",
  "user": "mckenna.tim@gmail.com",
  "descr": ""
}

export {cfgdata, devinfo, apploc}

