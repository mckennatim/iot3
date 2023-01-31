
// bool haywifi=true;
// char devid[9]="CYURD127";
// char owner[254]="tim@sitebuilt.net";
// char pwd[24]="geniot";
// char mqtt_server[60]="sitebuilt.net";
// char mqtt_port[6]="1884";

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
  "descr": "doorStrike with strike RGB LEDs and contacts. Part of entry system project that has doorbells, camera and aprtment UI."
}
const appid  = "doorStrike"

// /*PORTS index is srid#*/
// ports_t ports{
//   {D4,INPUT},//sr 0 contact
//   {D8,OUTPUT},//sr 1 red
//   {D7,OUTPUT},//sr 2 green
//   {D6,OUTPUT},//sr 3 blue
//   {D1,OUTPUT}// s4 4 strike
// }


/*CONFIG extern structures (initial values, changeable)*/
/*srs extern data structure initalization
state of relays and sensors  
srs_t srs {
  5,//numsr
  1,//sumse {sr,eading,rec,isnew}
  {{0,1,1,0}},
  0,//numcs {sr,reading,onoff,hi,lo,rec,isnew}
  {},
  4,//numrel
  { //rel{sr,onoff,rec,isnew}
    {1,0,1,0},{2,0,1,0},{3,0,1,0},{4,0,1,0}
  },
  0,//numdi difference control
  {}
};
*/



const devs = 
{ 
  "CYURD127": [
    {
      "sr": 0,
      "label": "contact" ,
      "type": "se",
      "io": "input",
      "port": "D4",
    },
    {
      "sr": 1,
      "label": "redLED",
      "type": "rel",
      "io": "output",
      "port": "D8",
    },
    {
      "sr": 2,
      "label": "greenLED",
      "type": "rel",
      "io": "output",
      "port": "D7",
    },
    {
      "sr":3,
      "label": "blueLED",
      "type": "rel",
      "io": "output",
      "port": "D6",
    },
    {
      "sr": 4,
      "label": "strike",
      "type": "rel",
      "io": "output",
      "port": "D1"
    }
  ]
}
//   "CYURD128": [
//     {
//       "sr": 5,
//       "label": "heatcrtl1     ",
//       "type": "cs",
//       "io": "INPUT",
//       "port": "D3" 
//     },
//     {
//       "sr" : 6,
//       "label": "tstat",
//       "connectedTo": "heatctrl1",
//       "type": "cs",
//       "io": "INPUT",
//       "port": "D0" 
//     },
//     {
//       "sr" : 7,
//       "label": "tstat",
//       "connectedTo": "heatctrl1",
//       "type": "cs",
//       "io": "OUTPUT",
//       "port": "D4" 
//     }
//   ]  
// }

  
const zones = 
[
  {
    "id": "contact",
    "name": "NC magnetic door contact",
    "img": "contact.jpg"
  },
  {
    "id": "redLED",
    "name": "closed & locked: redLED",
    "img": "redLED.jpg"
  },
  {
    "id": "greenLED",
    "name": "unlocked: greenLED",
    "img": "greenLED.jpg"
  },
  {
    "id": "blueLED",
    "name": "open: blueLED",
    "img": "blueLED.jpg"
  },
  {
    "id": "strike",
    "name": "Door Strike",
    "img": "strike.jpg"
  }
]




const initialState = {//pro must start at 0,0 
  contact: {darr:[1]},
  redLED: {darr:[1]},
  greenLED: {darr:[1]},
  blueLED: {darr:[1]},
  strike: {darr:[0]},
}

export {devs, zones, initialState, devinfo, apploc}


