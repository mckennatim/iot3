const devinfo = {
  "devid": "CYURD128",
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

/*  timer: {pro:[[]], timeleft:0, darr:[0,0,0,0]},
    tv: {pro:[[]], darr:[0,0,0,0]},
    pond: {pro:[[0,0,0],[19,15,1]], timeleft:0, darr:[0,0,0]},
*/
const initialState = {//pro must start at 0,0 
  contact: {darr:[1]},
}

const devs = 
{
  "CYURD128": [
    {
      "sr": 0,
      "params": 1,
      "label": "contact",
      "type": "se",
      "io": "input",
      "port": "D5"
    },
    {
      "sr": 1,
      "label": "ledRed",
      "type": "rel",
      "io": "output",
      "port": "D6"
    },
    {
      "sr": 2,
      "label": "ledGreen",
      "type": "rel",
      "io": "output",
      "port": "D7"
    },
    {
      "sr": 3,
      "label": "ledBlue",
      "type": "rel",
      "io": "output",
      "port": "D8"
    },
    {
      "sr": 4,
      "label": "strike",
      "type": "rel",
      "io": "output",
      "port": "D1"
    },
  ]
}

const zones= [
  {
    "id": "contact",
    "name": "NC magnetic door contact",
    "img": "contact.jpg"
  },
  {
    "id": "ledRed",
    "name": "ledRed - door is locked",
    "img": "ledRed.jpg"
  },
  {
    "id": "ledGreen",
    "name": "ledGreen - door is unlocked",
    "img": "ledGreen.jpg"
  },
  {
    "id": "ledBlue",
    "name": "ledBlue - door is open",
    "img": "ledBlue.jpg"
  },
  {
    "id": "strike",
    "name": "NC magnetic door strike",
    "img": "strike.jpg"
  },
]




export {devs, zones, initialState, devinfo, apploc}

