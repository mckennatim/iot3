/*Project Creations Script. Creates:
* A generic project data file. with devs, devinfo and apploc 
* Copies appbuild.js to the project conf/129projctname

Modify the four items noted as MODIFY then run> node appCreate.js

Then in the new project directory modify appata.js for your project particlulars
*/

import fs from 'fs'
const appid = "doorStrike1" /*MODIFY*/
const locid = "255ChestnutAve" /*MODIFY*/
const user = "mckenna.tim@gmail.com" /*MODIFY*/

const cfgdata = 
{
  "CYURD128": [/*MODIFY  ^^^^^ all mdifications are above here  ^^^^^*/
    {
      "sr": 0,
      "label": "contact",
      "type": "se",
      "senses": "onoff",
      "model": "NCcontact",
      "in": "D5",
      "reading" : 1,
      "rec": 1,
      "descr": "a short descriton"
    },

    {
      "sr": 1,
      "label": "strike",
      "type": "rel",
      "out": "D1",
      "onoff" : 0,
      "hayprg": 1,
      "rec": 1,
      "descr": "a short descriton"
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
  "CYURD006":[
    {
      "sr": 0,
      "label": "temp_out",
      "descr": "outside temperature"
    }
  ]
}

const devid =Object.keys(cfgdata)[0]
const folder =devid.substring(5)+appid

const jsfile = `const cfgdata = 
${JSON.stringify(cfgdata, null,2)}

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

export {cfgdata, devinfo, apploc}

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
console.log('jsfile: ', jsfile);

const datafile = `conf/${folder}/appdata.js`
fs.mkdir(`conf/${folder}`, (err)=>{
  console.log('err: ', err);
  console.log('folder: ', folder);
  if(!fs.existsSync(datafile)){
    console.log('appdata.js doesnt exist: ');
    const jsf= fs.createWriteStream(datafile,{ overwrite: false })
    jsf.write(jsfile)
    jsf.end()
  }
  // fs.open(`conf/${folder}/${appid}.js`,"w+" ,()=>{})
})

// const cfghfile = `conf/${folder}/${devid}/CONFIG.h`
// const cfgcfile = `conf/${folder}/${devid}/CONFIG.cpp`

// fs.mkdir(`conf/${folder}/${devid}`,()=>{
//   if(!fs.existsSync(cfghfile)){
//     console.log('CONFIG.h does not exist');
//     // const jsf= fs.createWriteStream(cfghfile,{ overwrite: false })
//     // jsf.write(cfgh)
//     // jsf.end()
//   }else{
//     console.log('CONFIG.h already exists: ');
//   }
//   if(!fs.existsSync(cfgcfile)){
//     console.log('CONFIG.cpp does not exist');
//     // const jsf= fs.createWriteStream(cfgcfile,{ overwrite: false })
//     // jsf.write(cfgc)
//     // jsf.end()
//   }else{
//     console.log('CONFIG.cpp already exists: ');
//   }
// })

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


// export {devid, appid, folder}