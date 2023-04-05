const cfgdata =
{
  "srs":[
    {
      "sr": 0,
      "label": "solar",
      "out": D8,
      "ctype": "difctrl",
      "init": [0,67,45,9,1],
      "cmdidx": [3],
      "prg": [[0,0,11]],
      "descr": "solar dif ctrl"
    },
    {
      "sr": 2,
      "labe": "dr",
      "out": D6,
      "ctype": "tstat",
      "init": [0,35,42,1],
      "cmdidx": [0,2,3],
      "prg": [[0,0,68]],
      "descr": "dining room tstat"
    }
  ],
  "inputs":[
    {
      "port": D2,
      "itype": "1-wire",
      "sensors": 3,
      "targets": [
        [[1,1]],
        [[2,1]]
      ]
    },
    {
      "port": D0,
      "itype": "button",
      "sensors": 1,
      "targets": [
        [[2,2]]
      ],
      "actions": [1,10,500]
    },
      {
      "port": D1,
      "itype": "button",
      "sensors": 1,
      "targets": [
        [[2,2]]
      ],
      "actions": [-1,-10,500]
    }
  ],
  "xdata": [
    {
      "CYURD006":[
        {
          "sr": 2, 
          "dt":[
            { 
              "didx": 2,
              "targets": [[0,1]]
            }
          ]
        }
      ]
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
  "appid": "demo2",
  "locid": "12ParleyVale",
  "user": "mckenna.tim@gmail.com",
  "descr": "demo of long board with relay, DHT11 and DSB1820"
}

export {cfgdata, devinfo, apploc}
