/*Project Creations Script. Creates:
* A generic project data file. with devs, devinfo and apploc 
* Copies appbuild.js to the project conf/129projctname

Modify the four items noted as MODIFY then run> node appCreate.js

Then in the new project directory modify appata.js for your project particlulars
*/

import fs from 'fs'
const appid = "doorStrike" /*MODIFY*/
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

const cpycf =
`
import fs from 'fs'


`