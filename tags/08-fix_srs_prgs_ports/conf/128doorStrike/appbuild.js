/*this shouldn't change */

import{ devs, zones, initialState, devinfo, apploc} from './appdata.js'
import fs from 'fs'

const{appid, locid} =apploc
const devid = Object.keys(devs)[0]

console.log(': ', );

const sql = fs.createWriteStream(`${appid}.sql`);
const inst = fs.createWriteStream(`${appid}.md`, { flags: 'a' });
const cfgh = fs.createWriteStream(`${devid}/CONFIG.cpp`, { flags: 'a' });
const cfgc = fs.createWriteStream(`${devid}/CONFIG.h`, { flags: 'a' });


const sq = `SELECT * FROM \`devs\` ORDER BY \`devs\`$a{devi}\` DESC;`
const de =`
DELETE FROM devs WHERE devs.devid = '${devinfo.devid}';
`

console.log(de);

const MKports = ()=>{
  let ports = `/*ports */ \n ports_t ports{ \n`
  devs[devid].map((d)=>{
    ports += `  {${d.port.toUpperCase()}, ${d.io.toUpperCase()}}, \\\\ sr:${d.sr} ${d.label} \n`;
  })
  ports += `}`
  return ports
}


const insdev = `
REPLACE INTO \`devs\` (\`devid\`, \`owner\`, \`devpwd\`, \`locid\`, \`description\`) VALUES ( 
  '${devid}', 
  '${devinfo.owner}',
  '${devinfo.pwd}',
  '${locid}',
  '${apploc.descr}'
);
`
console.log('insdev: ', insdev);  
sql.write(insdev)

const al =`
REPLACE INTO \`app_loc\` (\`appid\`, \`locid\`, \`devs\`, \`zones\`) VALUES (
  '${appid}',
  '${locid}',
  '${JSON.stringify(devs)}',
  '${JSON.stringify(zones)}'
);
`
console.log('al: ', al);  
sql.write(al)

const alu =`
REPLACE INTO \`app_loc_user\` (\`appid\`, \`userid\`, \`locid\`, \`role\`,\`auth\`) VALUES (
  '${appid}',
  '${devinfo.owner}',
  '${locid}',
  'admin',
  1
); 
  
REPLACE INTO \`app_loc_user\` (\`appid\`, \`userid\`, \`locid\`, \`role\`,\`auth\`) VALUES (
  '${appid}',
  '${apploc.user}',
  '${locid}',
  'user',
  1
); 
  `
console.log('alu: ', alu);  
sql.write(alu)

const util = `
/*
These are SQL commands utlity commands
*/

SELECT * FROM \`devs\` ORDER BY \`id\` DESC;
SELECT * FROM \`app_loc_user\` ORDER BY \`id\` DESC;
SELECT * FROM \`app_loc\` ORDER BY \`id\` DESC;

`
console.log('util: ', util);  
sql.write(util)

let ini =`
/*  timer: {pro:[[]], timeleft:0, darr:[0,0,0,0]},
    tv: {pro:[[]], darr:[0,0,0,0]},
    pond: {pro:[[0,0,0],[19,15,1]], timeleft:0, darr:[0,0,0]},
*/
const initialState = {//pro must start at 0,0 
`
devs[devid].map((d)=>{
  ports += `  {${d.label} ${d.io.toUpperCase()}}, \\\\ sr:${d.sr} ${d.label} \n`;
})
  
console.log(MKports());

sql.end()
inst.end()
cfgc.end()
cfgh.end()
