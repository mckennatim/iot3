const cfgdata =
{
  "CYURD128": [
    {
      "sr": 0,
      "label": "temp2",
      "type": "se",
      "senses": "temp",
      "model": "DS18B20",
      "in": "D2",
      "reading" : 45,
      "rec": 1,
      "descr": "demo temp sensor"
    },

    {
      "sr": 1,
      "label": "temp",
      "type": "cs",
      "senses": "temp-hum",
      "model": "DHT11",
      "in": "D5",
      "out": "D8",
      "reading" : 44,
      "onoff": 0,
      "hi": 69,
      "lo": 67,
      "rec": 1,
      "descr": "temp side of DHT11"
    },
    {
      "sr": 2,
      "label": "hum",
      "type": "cs",
      "model": "DHT11",
      "in": "D5",
      "out": "D7",
      "reading" : 24,
      "onoff": 0,
      "hi": 90,
      "lo": 60,
      "rec": 1,
      "descr": "temp side of DHT11"
    },
    {
      "sr": 3,
      "label": "timr1",
      "type": "rel",
      "out": "D6",
      "onoff" : 0,
      "hayprg": 1,
      "haytimr": 1,
      "rec": 0,
      "descr": "a relay controlled by timer"
    }
  ],
  "CYURD006": [
    {
      "sr": 0,
      "label": "temp_out",
      "type": "se",
      "reading": 55,
      "descr": "outside temperature"
    }
  ]
}

const models = "DSB1820, DS18B20b, DHT11, DHT22, BH1750, HTU21D, MAX31855, ANALOG, DIG1, DIG2 "

const devinfo = {
  "devid": "CYURD128",
  "owner": "tim@sitebuilt.net",
  "pwd": "geniot",
  "mqtt_server": "sitebuilt.net",
  "mqtt_port": "1884"
}

const apploc ={
  "appid": "demo2",
  "locid": "12ParleyVale",
  "user": "mckenna.tim@gmail.com",
  "descr": "demo of long board with relay, DHT11 and DSB1820"
}

export {cfgdata, devinfo, apploc, models}
