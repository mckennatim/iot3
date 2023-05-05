/*Project Creations Script. Creates:
* A generic project data file. with devs, devinfo and apploc 
* Copies appBuild.js to the project conf/129projctname

Modify the six items noted as MODIFY then run> node appCreate.js

Then in the new project directory modify appdata.js for your project particlulars
*/

import fs from 'fs'
import{ cfgdata, devinfo, apploc, senstypes, ctrltypes, topics} from './appdata.js'
const{devid, owner, pwd, mqtt_server, mqtt_port}= devinfo
const{appid, address, user, descr, board}=apploc

const folder =devid.substring(5)+appid
const devdir = `../conf/${folder}/${devid}`


const datafile = `../conf/${folder}/appdata.js`
const buildfile = `../conf/${folder}/appBuild.js`
import { fetchAddress } from './getAddress.js'


fetchAddress(address)
.then((addrinfo)=>{
  console.log('addrinfo: ', addrinfo);
  const{locid, lat,lon,tzone}=addrinfo
  apploc.addrinfo=addrinfo

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

fs.mkdir(`conf/${folder}`, (err)=>{
  console.log('err: in 127edge', err);
  console.log('folder: ', folder);
  if(!fs.existsSync(buildfile)){
    console.log('appBuild doesnot exist: ');
    fs.copyFile('appBuild.js',buildfile,(err)=>{
      if (err){
        console.log('err: ', err);
      }
      console.log('appBuild.js created ');
    })
    // if(!fs.existsSync(datafile)){
    //   console.log('appdata doesnot exist: ',err);
      const appdat = fs.createWriteStream(`${folder}/appdata.js`)
      appdat.write(JSON.stringify(devinfo))
      appdat.end()
      // fs.copyFile('appdata.js',datafile,(err)=>{
      //   if (err){
      //     console.log('err: ', err);
      //   }
    //   console.log('appdata.js created ');
      
    // }else{
    //   console.log('appdata.js already exists: ');
    // } 
    fs.mkdir(devdir, (err)=>{
      console.log(err)
      const cfgcp= fs.createWriteStream(`${devdir}/copyCONFIG.js`)
      cfgcp.write(cpcode)
      cfgcp.end()
      const cfgget= fs.createWriteStream(`${devdir}/getCONFIG.js`)
      cfgget.write(getcode)
      cfgcp.end()
    })
  }else{
    console.log('appBuild.js already exists: ');
  }  
  
  // fs.open(`conf/${folder}/${appid}.js`,"w+" ,()=>{})
})


fs.cpSync(`../boards/${board}`,`../conf/${folder}/${board}`, {recursive: true});

})
