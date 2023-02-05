/*this shouldn't change */

import{ devs, zones, initialState, devinfo, apploc} from './appdata.js'
import fs from 'fs'

const{appid, locid} =apploc
const devid = Object.keys(devs)[0]
const devobj = devs[devid] 

console.log("${devid}/CONFIG.cpp: ", `${devid}/CONFIG.cpp`);

const sql = fs.createWriteStream(`${appid}.sql`);
const inst = fs.createWriteStream(`${appid}.md`, { flags: 'a' });
const cfgh = fs.createWriteStream(`${devid}/CONFIG.h`);
const cfgc = fs.createWriteStream(`${devid}/CONFIG.cpp`);


const sq = `SELECT * FROM \`devs\` ORDER BY \`devs\`$a{devi}\` DESC;`
const de =`
DELETE FROM devs WHERE devs.devid = '${devinfo.devid}';
`

console.log(de);


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

/*Creating CONFIG.cpp ---------------------------------------------- */
const head = `
#include "CONFIG.h"

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
cfgc.write(head)

const MKdev =()=>{
  let sdev = `
/*dev extern device variables*/  
char devid[9]="${devid}";
char owner[254]="${devinfo.owner}";
char pwd[24]="${devinfo.pwd}";
char mqtt_server[60]="${devinfo.mqtt_server}";
char mqtt_port[6]="${devinfo.mqtt_port}";

`
return sdev
}
console.log('MKdev(): ', MKdev());
cfgc.write(MKdev());

const MKports = ()=>{
  let ports =`
/*ports for input and output
{sr, in, out, rec, isnew} */
const ports_t ports {
`
  devs[devid].map((d)=>{
    ports += `  {${d.sr}, ${d.hasOwnProperty('in') ? d.in : '-9'}, ${d.hasOwnProperty('out') ? d.out : '-9'}, ${d.rec}, 0},// ${d.label} \n`;
  })
  ports += `}`
  return ports
}
console.log(MKports());
cfgc.write(MKports())

const MKsens =()=>{
  let sen=`
/*SE constant declarations*/
const sen_t SE {
`  
  const sens = devobj.filter(d=>{
    return d.type === "se" || d.type==="cs"
  })
  const pts = sens.map((d)=>{
    return d.in
  })
  const pta = [...new Set(pts)]
  sen += `  ${pta.length}, // numstypes \n`
  sen += `  ${sens.length}, // numsens \n`
  if(pta.length==0){
    sen+= `  {} // stype:{nums,{sra,srb...},type,model}\n`
  }else{
    sen+= `  { // stype:{nums,{sra,srb...},type,model}\n`
    pta.map((e,j)=>{
      const dups =sens.filter((f)=>{
        return f.in == e
      })
      const sr = dups.map((s)=>{
        return s.sr
      })
      const nums = sr.length  
      let sesrs = `{ ` 
      sr.map((x,i)=>{
        sesrs += `${x}${i+1==nums?' ':','}`
      })
      sesrs += `}`
      sen += `    { ${nums}, ${sesrs}, "${dups[0].senses}", "${dups[0].model}" }${j+1==pta.length?' ':','} \n`
    })
    sen += `  }\n}`
  }
  return sen
}
console.log(MKsens());
cfgc.write(MKsens());

const MKsrs = ()=>{
  let srs =`
/*srs data structure to hold the current state of the entire device*/
const srs_t srs {
`
  const numsr = devobj.length 
  srs += `  ${numsr}, // numsr \n`
  const se = devobj.filter(d=>{
    return d.type === "se"
  })
  srs += `  ${se.length}, // numse \n `
  if (se.length==0){
    srs += ` {}, // se:{sr, reading,}\n`
  }else{  
    srs += ` { // se:{sr, reading} \n`
    se.map((d,i)=>{
      srs += `    {${d.sr}, ${d.reading}}${i+1==se.length?' ':','} // ${d.label}\n`
    })
    srs+= `  },\n`
  }
  const cs = devobj.filter(d=>{
    return d.type === "cs"
  })
  srs += `  ${cs.length}, // numcs \n `
  if (cs.length==0){
    srs += ` {}, // cs:{sr, reading, onoff, hi, lo}\n`
  }else{
    srs += ` { // cs:{sr, reading, onoff, hi, lo} \n`
    cs.map((d,i)=>{
      srs += `    {${d.sr}, ${d.reading}, ${d.onoff}, ${d.hi}, ${d.lo}}${i+1==cs.length?' ':','} // ${d.label}\n`
    })
    srs+= `  },\n`
  }
  const rel = devobj.filter(d=>{
    return d.type === "rel"
  })
  srs += `  ${rel.length}, // numrel \n `
  if (rel.length==0){
    srs += ` {}, // rel:{sr, onoff}\n`
  }else{
    srs += ` { // rel:{sr, onoff} \n`
    rel.map((d,i)=>{
      srs += `    {${d.sr}, ${d.onoff}}${i+1==rel.length?' ':','} // ${d.label}\n`
    })
    srs+= `  },\n`
  }
  const dif = devobj.filter(d=>{
    return d.type === "dif"
  })
  srs += `  ${dif.length}, // numdif \n `
  if (dif.length==0){
    srs += ` {}, // dif:{sr, onoff}\n`
  }else{
    srs += ` { // dif:{sr, sra, srb, diffon, diffoff, maxa, maxb, onoff} \n`
    dif.map((d,i)=>{
      srs += `    {${d.sr}, ${d.sra}, ${d.srb}, ${d.difon}, ${d.difoff}, ${d.maxa}, ${d.maxb}, ${d.onoff}}${i+1==dif.length?' ':','} // ${d.label}\n`
    })
    srs+= `  },\n`
  }
  srs += `}`
  return srs
}
console.log(MKsrs());
cfgc.write(MKsrs());


const MKprgs=()=>{
  let pstr = `
/*prgs extern data structure initalization*/ 
prgs_t prgs{  
`
  const prgs = devobj.filter(d=>{
    if(d.hasOwnProperty('hayprg')) {
      return d.hayprg===1
    }
  })
  pstr +=`  ${prgs.length}, // numprgs \n`
  if(prgs.length == 0){
    pstr += `  {}\n`
  }else{
    pstr += `  { // prg:{sr,aid,ev,numdata,prg[[]],hms} \n`
    prgs.map((e,i)=>{
      switch(e.type){
        case "cs":{
          pstr += `    {${e.sr}, 255, 1, 2, {{0,0,${e.hi},${e.lo}}}, ${1500+2*i}}${i+1==prgs.length?' ':','}  \n`
          break;
        }
        case "rel":{
          pstr += `    {${e.sr}, 255, 1, 1, {{0,0,${e.onoff},}}, ${1500+2*i}}${i+1==prgs.length?' ':','}  \n`
          break;
        }
        case "dif":{
          pstr += `    {${e.sr}, 255, 1, 2, {{0,0,${e.difon},${e.difoff}}}, ${1500+2*i}}${i+1==prgs.length?' ':','}  \n`
          break;
        }
        default:{
          break
        }
      }
    })
    pstr += `  }\n`
  }
  pstr += `}\n`
  return pstr
}
console.log(MKprgs());
cfgc.write(MKprgs());

const flags =`
/*flags extern data structure*/
flags_t f {
  0,//aUTOMA
  0,//fORCErESET
  5,//cREMENT
  96,//HAStIMR 1100000 64+32=96
  0,//IStIMERoN
  198,//HAYpROG 11000010 =128+64+4=198
  1023,//HAYsTATEcNG
  0,//CKaLARM
  0,//ISrELAYoN
  {0,0,0,0,0,0,0,0,0,0}//tIMElEFT[10]
};
`
cfgc.write(flags)

/*app intiState --------------------------------------------------------*/

const MKinitState =()=>{
  let ini = `
/*  
temp_out: darr:[]
tstat: {pro:[[0,0,65,63]], timeleft:0, darr:[reading,onoff,66,64]},
tv: {pro:[[0,0,hi,lo]], darr:[reading,onoff,hi,lo]},
pond: {pro:[[0,0,0],[19,15,1]], timeleft:0, darr:[0,0,0]},
//solar: darr:[sra_rdg, srb_rdg, difon, difoff,maxa, maxb onoff]
*/
const initialState = {//pro must start at 0,0   
`
  if(devobj.length == 0){
    ini += `  {}\n`
  }else{
    ini += `  { // prg:{sr,aid,ev,numdata,prg[[]],hms} \n`
    devobj.map((e,i)=>{
      switch(e.type){
        case "se":{
          ini += `    ${e.label}: {darr:[${e.reading}]}${i+1==devobj.length?' ':','}  \n`
          break; 
        }
        case "cs":{
          let prg=''
          if(e.hayprg){
            prg= `, pro:[[0,0,${e.hi},${e.lo}]], timeleft:0`
          }
          ini += `    ${e.label}: {darr:[${e.reading}, ${e.onoff}, ${e.hi}, ${e.lo}]${prg}}${i+1==devobj.length?' ':','}  \n`
          break;
        }
        case "rel":{
          let prg=''
          if(e.hayprg){
            prg= `, pro:[[0,0,${e.onoff}]], timeleft:0`
          }
          ini += `    ${e.label}: {darr:[${e.onoff}]${prg}}${i+1==devobj.length?' ':','}  \n`
          break;
        }
        case "dif":{
          break;
        }
        default:{
          break
        }
      }
    })
    ini += `  }\n`
  }
  ini += `}\n`
  return ini
}
console.log(MKinitState())
  
sql.end()
inst.end()
cfgc.end()
cfgh.end()
