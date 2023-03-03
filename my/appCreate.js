/*Project Creations Script. Creates:
* A generic project data file. with devs, devinfo and apploc 
* Copies appbuild.js to the project conf/129projctname

Modify the six items noted as MODIFY then run> node appCreate.js

Then in the new project directory modify appdata.js for your project particlulars
*/

import fs from 'fs'

/*MODIFY  items starting here */

const devid = "CYURD128" 
const appid = "demo2" 
const locid = "12ParleyVale" 
const user = "mckenna.tim@gmail.com" 
const board = "long.pdf"
const descr = "demo of long board with relay, DHT11 and DSB1820"

/*MODIFY end of data to modify */

const folder =devid.substring(5)+appid
const devdir = `conf/${folder}/${devid}`

const cfgdata =
`const cfgdata =
{
  "${devid}": [
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
      "hi": 68,
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
      "hayprg": true,
      "haytimr": true,
      "rec": 0,
      "descr": "a relay controlled by timer"
    },
    {
      "sr": 4,
      "label": "ledBlue",
      "type": "rel",
      "out": "D6",
      "onoff" : 0,
      "rec": 0,
      "haytimr": true,
      "descr": "a short descrition"
    },
    {
      "sr": 5,
      "label": "lr",
      "type": "cs",
      "senses": "temp",
      "model": "DS18B20",
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
      "model": "DS18B20",
      "in": "D0",
      "out": "D1",
      "reading" : 33,
      "onoff": 0,
      "hi": 70,
      "lo": 40,
      "rec": 1,
      "descr": "a short descrition",      
      "hayprg": 1
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
    //   "rec": 1,
    //   "hayprg" : 1
    // },
  ],
  "CYURD006": [
    {
      "sr": 0,
      "label": "temp_out",
      "type": "se",
      "reading": 55,
      "descr": "outside temperature",
      "inxdata": true,
      "inapp": true
    },
    {
      "sr": 4,
      "label": "ledBlue",
      "type": "rel",
      "onoff" : 0,
      "rec": 0,
      "descr": "a short descrition",
      "inxdata": false,
      "inapp": true
    }
  ],
  "CYURD007": [
    {
      "sr": 0,
      "label": "temp_out2",
      "type": "cs",
      "onoff": 0,
      "hi": 70,
      "lo": 40,
      "reading": 55,
      "descr": "outside temperature",
      "inxdata": true,
      "inapp": true
    }
  ]
}

const models = "DSB1820, DS18B20b, DHT11, DHT22, BH1750, HTU21D, MAX31855, ANALOG, DIG1, DIG2 "

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
  "descr": "${descr}"
}

export {cfgdata, devinfo, apploc, models}
`

console.log('cfgdata: ', cfgdata);

const datafile = `conf/${folder}/appdata.js`
fs.mkdir(`conf/${folder}`, (err)=>{
  console.log('err: ', err);
  console.log('folder: ', folder);
  if(!fs.existsSync(datafile)){
    console.log('appdata.js doesnt exist: ');
    const jsf= fs.createWriteStream(datafile,{ overwrite: false })
    jsf.write(cfgdata)
    jsf.end()
  }
  // fs.open(`conf/${folder}/${appid}.js`,"w+" ,()=>{})
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

const cpcode = `
import fs from 'fs'

const dest= '../../../src'

fs.copyFile('CONFIG.h', \`\${dest}/CONFIG.h\`,(err)=>{
  if (err){
    console.log('err: ', err);
  }
  console.log('CONFIG.h is copied to my/src ');
})

fs.copyFile('CONFIG.cpp', \`\${dest}/CONFIG.cpp\`,(err)=>{
  if (err){
    console.log('err: ', err);
  }
  console.log('CONFIG.cpp is copied to my/src ');
})

`
const getcode = `
import fs from 'fs'

const src= '../../../src'

fs.copyFile(\`\${src}/CONFIG.h\`, \`CONFIG.h\`,(err)=>{
  if (err){
    console.log('err: ', err);
  }
  console.log('CONFIG.h is copied from my/src ');
})

fs.copyFile(\`\${src}/CONFIG.cpp\`, \`CONFIG.cpp\`,(err)=>{
  if (err){
    console.log('err: ', err);
  }
  console.log('CONFIG.cpp is copied from my/src ');
})

fs.copyFile(\`\${src}/main.cpp\`, \`main.cpp\`,(err)=>{
  if (err){
    console.log('err: ', err);
  }
  console.log('main.cpp is copied from my/src ');
})

`

fs.mkdir(devdir, (err)=>{
  console.log(err)
  const cfgcp= fs.createWriteStream(`${devdir}/copyCONFIG.js`)
  cfgcp.write(cpcode)
  cfgcp.end()
  const cfgget= fs.createWriteStream(`${devdir}/getCONFIG.js`)
  cfgget.write(getcode)
  cfgcp.end()
})

fs.copyFile(`boards/${board}`,`conf/${folder}/${board}`,(err)=>{
  if (err){
    console.log('err: ', err);
  }
  console.log(`conf/${folder}/${board} created`);
})
