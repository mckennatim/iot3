import fs from 'fs'
const appid = "doorStrike"
const locid = "255ChestnutAve"
const user = "mckenna.tim@gmail.com"

const devs = 
{
  CYURD128:[
    {
      "sr": 0,
      "type": "se",//se, cs, rel 
      "params": "{1}", // {reading}
      "label": "sensor" ,
      "io": "input",
      "rec": true
    },
    {
      "sr": 1,
      "type": "rel",//se, cs, rel 
      "params": "[0]", // {setting}
      "label": "led" ,
      "io": "output",
      "rec": true
    },
    {
      "sr": 3,
      "type": "cs", //se, cs, rel 
      "params": 1, //on/off or hi/lo
      "label": "sensor" ,
      "input": "D",
      "output": "",
      "rec": true
    },
    {
      "sr": 4,
      "type": "cs",//se, cs, rel 
      "params": "[0]", // {reading,hi,lo)on/off or hi/lo
      "label": "timed relay" ,
      "io": "output",
      "port": "D5",
      "rec": true,
      "prg": "{{0,0,1}}" 
    },
    {
      "sr": 5,
      "type": "cs",//se, cs, rel 
      "params": "[33,0,69,67]", // {reading, onoff,hi,lo}
      "label": "tstat" ,
      "io": "output",
      "port": "D5",
      "rec": true,
      "prg": "{{0,0,63,61}}" //{{hr,min.h,lo}}
    },
  ]
}
const devid =Object.keys(devs)[0]
const folder =devid.substring(5)+appid

const jsfile = `const devs = 
${JSON.stringify(devs, null,2)}

const zones= [
  {
    "id": "contact",
    "name": "NC magnetic door contact",
    "img": "contact.jpg"
  }
]


const devinfo = {
  "devid": "${devid}",
  "owner": "tim@sitebuilt.net",
  "pwd": "geniot",
  "mqtt_server": "sitebuilt.net",
  "mqtt_port": "1884"
}

const apploc ={
  "appid": "${appid}",
  "locid": "${locid}",
  "user": "${user}",
  "descr": ""
}

/*  timer: {pro:[[]], timeleft:0, darr:[0,0,0,0]},
    tv: {pro:[[]], darr:[0,0,0,0]},
    pond: {pro:[[0,0,0],[19,15,1]], timeleft:0, darr:[0,0,0]},
*/
const initialState = {//pro must start at 0,0 
  contact: {darr:[1]},
}

export {devs, zones, initialState, devinfo, apploc}

`
const cfgh = 
`
#ifndef CONFIG_h
#define CONFIG_h

#include <TimeLib.h>
#include <TimeAlarms.h>
#include <ESP8266WebServer.h>

void customInit();
void customLoop();

/*dev */
extern bool haywifi;
extern char devid[9];
extern char owner[254];
extern char pwd[24];
extern char mqtt_server[60];
extern char mqtt_port[6];

/*INCOMING topics*/
struct topics_t {
    int numtopics;
    char scribedTo[6][8]; 
};
extern const topics_t tpcsSub ;

/*srs data structure declarations*/ 
struct se_t {//sensors
    int sr;
    int reading;
    bool rec;
    bool isnew;
};
struct cs_t {//controlled sensors
    int sr;
    int reading;
    bool onoff;
    int hilimit;
    int lolimit;
    bool rec;
    bool isnew;   
};
struct rel_t {//timers
    int sr;
    bool onoff;
    bool rec; 
    bool isnew;  
};
struct di_t {//diff control
    int sa;
    int sb; 
    int don;
    int doff;
    int maxa;
    int maxb;
    int port;
    bool onoff;
    bool rec;
    bool isnew;
};
`

const cfgc=
`#include "CONFIG.h"

void customInit(){
  if(0==1){
    ;
  }
};
void customLoop() {
  if(0==1){
    ;
  }
};
`
const datafile = `conf/${folder}/appdata.js`
fs.mkdir(`conf/${folder}`, ()=>{
  if(!fs.existsSync(datafile)){
    console.log('appdata doesnt exist: ');
    const jsf= fs.createWriteStream(datafile,{ overwrite: false })
    jsf.write(jsfile)
    jsf.end()
  }
  // fs.open(`conf/${folder}/${appid}.js`,"w+" ,()=>{})
})

const cfghfile = `conf/${folder}/${devid}/CONFIG.h`
const cfgcfile = `conf/${folder}/${devid}/CONFIG.cpp`

fs.mkdir(`conf/${folder}/${devid}`,()=>{
  if(!fs.existsSync(cfghfile)){
    console.log('CONFIG.h does not exist');
    const jsf= fs.createWriteStream(cfghfile,{ overwrite: false })
    jsf.write(cfgh)
    jsf.end()
  }else{
    console.log('CONFIG.h already exists: ');
  }
  if(!fs.existsSync(cfgcfile)){
    console.log('CONFIG.cpp does not exist');
    const jsf= fs.createWriteStream(cfgcfile,{ overwrite: false })
    jsf.write(cfgc)
    jsf.end()
  }else{
    console.log('CONFIG.cpp already exists: ');
  }
})

const buildfile = `conf/${folder}/appbuild.js`
if(!fs.existsSync(buildfile)){
  console.log('appbuild doesnot exist: ');
  fs.copyFile('appbuild.js',buildfile,(err)=>{
    if (err){
      console.log('err: ', err);
    }
    console.log('appbuild.js created ');
  })
}else{
  console.log('appbuild.js already exists: ');
}


export {devid, appid, folder}