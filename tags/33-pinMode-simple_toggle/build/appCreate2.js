import fs from 'fs'
import{ cfgdata, devinfo, apploc, senstypes, ctrltypes, topics} from './appdata.js'
const{devid, owner, pwd, mqtt_server, mqtt_port}= devinfo
const{appid, address, user, descr, board}=apploc

const folder =devid.substring(5)+appid
const projdir = `../conf/${folder}`
const devdir = `../conf/${folder}/${devid}`


const datafile = `../conf/${folder}/appdata.js`
const buildfile = `../conf/${folder}/appBuild.js`
import { fetchAddress } from './getAddress.js'

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
const adatend = 
`const cfgdata = {
  srs: [ //sr must match index
  ],
  inputs: [
  ],
  xdata: {
  },
};

const senstypes = ["butn", "switch", "1-wire", "dht", "i2c", "spi", "toggle"];
const ctrltypes = {
  difctrl: {
    lab: ["onoff", "rdA", "rdB", "dif", "difgt0"],
    cmd: [3],
    prg: [3],
  },
  tstat: {
    lab: ["onoff", "reading", "setting", "dif"],
    cmd: [0, 2, 3],
    prg: [3],
  },
  relay: {
    lab: ["onoff", "tsec", "dsr"],
    cmd: [0, 1],
    prg: [1],
  },
  delay: {
    lab: ["onoff", "tsec", "rsr", "prior"],
    cmd: [0, 1],
    prg: [1],
  },
  ctrl: {
    lab: ["bell", "reading", "setting"],
    cmd: [2],
    prg: [2],
  },
  power: {
    lab: ["onoff"],
    cmd: [0],
    prg: [],
  },
  array: {
    lab: ["a0", "a1", "a2", "a3", "a4", "a5", "a6", "a7"],
    cmd: [],
    prg: [],
  },
  se: {
    lab: ["reading"],
    cmd: [],
    prg: [],
  },
  cs: {
    lab: ["onoff", "reading", "hi", "lo"],
    cmd: [0, 2, 3],
    prg: [2, 3],
  },
  re: {
    lab: ["onoff"],
    cmd: [0],
    prg: [0],
  },
};
const topics = ["devtime", "cmd", "prg", "req", "set"];

export { cfgdata, devinfo, apploc, senstypes, ctrltypes, topics };

`


fetchAddress(address)
.then((addrinfo)=>{
  console.log('addrinfo: ', addrinfo);
  const{locid, lat,lon,tzone}=addrinfo
  apploc.addrinfo=addrinfo
  console.log('apploc: ', apploc);
  
  fs.mkdir(devdir, {recursive:true}, (err)=>{
    console.log('ferror: ', err);
    fs.copyFile('appBuild.js',buildfile,(err)=>{
      if (err){
        console.log('err: ', err);
      }
      console.log('appBuild.js created ');
    })
    fs.cpSync(`../boards/${board}`,`../conf/${folder}/${board}`, {recursive: true});
    const cfgcp= fs.createWriteStream(`${devdir}/copyCONFIG.js`)
    cfgcp.write(cpcode)
    cfgcp.end()
    const cfgget= fs.createWriteStream(`${devdir}/getCONFIG.js`)
    cfgget.write(getcode)
    cfgget.end()
    if(!fs.existsSync(`${projdir}/appdata.js`)){
      const appdat= fs.createWriteStream(`${projdir}/appdata.js`)
      appdat.write(`const devinfo=${JSON.stringify(devinfo,null,2)}\n\n`)
      appdat.write(`const apploc=${JSON.stringify(apploc,null,2)}\n\n`)
      appdat.write(adatend)
      appdat.end()
    }
  })
})

