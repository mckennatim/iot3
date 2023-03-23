/*this shouldn't change */

import{ cfgdata, devinfo, apploc} from './appdata.js'
import fs from 'fs'

const{appid, locid} =apploc
const devid = Object.keys(cfgdata)[0]
const devobj = cfgdata[devid] 
let numtypes = 0
let numsr =devobj.length
let numse =0
let numcs=0
let numcr=0
let numrel=0
let numdi=0
let numprgs =0
const numxdevs = Object.keys(cfgdata).length - 1 //#of xdevs


console.log("${devid}/CONFIG.cpp: ", `${devid}/CONFIG.cpp`);

const sql = fs.createWriteStream(`${appid}.sql`);

const appd = fs.createWriteStream(`appInit.js`);
const cfgh = fs.createWriteStream(`${devid}/CONFIG.h`);
const cfgc = fs.createWriteStream(`${devid}/CONFIG.cpp`);

let ddd = {}
let dbz = `
[
`
let numzon=0
const keys = Object.keys(cfgdata)
const z = keys.map((devid,i)=>{
  numzon += cfgdata[devid].length
  const dd =cfgdata[devid].map((f,j)=>{
    dbz += `  {"id": "${f.label}", "name": "${f.descr}", "img": "${f.label}.png" },\n`
    return {"sr": f.sr, "label": f.label }
  })
  ddd[devid]=dd
})
dbz = dbz.slice(0,-2)
dbz += `
]`

console.log('dbz: ', dbz);
const devs = JSON.stringify(ddd,null,2);


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
  '${devs}', 
  '${dbz}'
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

SELECT * FROM \`devs\` ORDER BY \`id\` DESC LIMIT 3;
SELECT * FROM \`app_loc_user\` ORDER BY \`id\` DESC LIMIT 3;
SELECT * FROM \`app_loc\` ORDER BY \`id\` DESC LIMIT 3;

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

/*SPECS
CONFIG extern const device variables
INCOMING const mqtt topics*/
const topics_t TPC {
  5,
  {"devtime", "cmd", "prg", "req", "set"}
};
`
return sdev
}
console.log('MKdev(): ', MKdev());
cfgc.write(MKdev());

const MKports = ()=>{
  let ports =`
/*ports for input and output*/
ports_t ports {
`
  ports += `  ${numsr}, //numsr
  {//port:{sr, in, out, rec, isnew}\n`
  cfgdata[devid].map((d,i)=>{
    ports += `    {${d.sr}, ${d.hasOwnProperty('in') ? d.in : '-9'}, ${d.hasOwnProperty('out') ? d.out : '-9'}, ${d.rec}, 0}${i+1==numsr?' ':','}// ${d.label} ${d.model}\n`;
  })
  ports += `  }
};`
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
    return d.type === "se" || d.type==="cs" || d.type==="cr"
  })
  const pts = sens.map((d)=>{
    return d.in
  })
  const pta = [...new Set(pts)]
  numtypes =pta.length
  sen += `  ${numtypes}, // numstypes \n`
  sen += `  ${sens.length}, // numsens \n`
  if(numtypes==0){
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
      sen += `    { ${nums}, ${sesrs}, "${dups[0].senses}", "${dups[0].model}" }${j+1==numtypes?' ':','} \n`
    })
    sen += `  }
};\n`
  }
  return sen
}
console.log(MKsens());
cfgc.write(MKsens());

let introsrs =`/*?rs data structure to hold the current state of device*/
srs_t srs `

const MKsrs = (ansr)=>{
  let srs = `{\n`
  const numsr = ansr.length 
  srs += `      ${numsr}, // numsr \n`
  const se = ansr.filter(d=>{
    return d.type === "se"
  })
  numse = se.length > numse ? se.length : numse
  srs += `      ${se.length}, // se.length \n `
  if (se.length==0){
    srs += `    {}, // se:{sr, reading,}\n`
  }else{  
    srs += `     { // se:{sr, reading} \n`
    se.map((d,i)=>{
      srs += `        {${d.sr}, ${d.reading}}${i+1==se.length?' ':','} // ${d.label}\n`
    })
    srs+= `      },\n`
  }
  const cs = ansr.filter(d=>{
    return d.type === "cs"
  })
  numcs= cs.length > numcs ? cs.length : numcs
  srs += `      ${cs.length}, // cs.length \n `
  if (cs.length==0){
    srs += `     {}, // cs:{sr, reading, onoff, hi, lo}\n`
  }else{
    srs += `     { // cs:{sr, reading, onoff, hi, lo} \n`
    cs.map((d,i)=>{  
      srs += `        {${d.sr}, ${d.reading}, ${d.onoff}, ${d.hi}, ${d.lo}}${i+1==cs.length?' ':','} // ${d.label}\n`
    })
    srs+= `      },\n`
  }
  const cr = ansr.filter(d=>{
    return d.type === "cr"
  })
  numcr= cr.length > numcr ? cr.length : numcr
  srs += `      ${cr.length}, // cr.length \n `
  if (cr.length==0){
    srs += `     {}, // cr:{sr, tsec, onoff}\n`
  }else{
    srs += `     { // cr:{sr, tsec, onoff} \n`
    cr.map((d,i)=>{  
      srs += `        {${d.sr}, ${d.tsec}, ${d.onoff}}${i+1==cr.length?' ':','} // ${d.label}\n`
    })
    srs+= `      },\n`
  }
  const rel = ansr.filter(d=>{
    return d.type === "rel"
  })
  numrel=rel.length > numrel ? rel.length : numrel
  srs += `      ${rel.length}, // rel.length \n `
  if (rel.length==0){
    srs += `     {}, // rel:{sr, onoff}\n`
  }else{
    srs += `     { // rel:{sr, onoff} \n`
    rel.map((d,i)=>{
      srs += `        {${d.sr}, ${d.onoff}}${i+1==rel.length?' ':','} // ${d.label}\n`
    })
    srs+= `      },\n`
  }
  const dif = ansr.filter(d=>{
    return d.type === "dif"
  })
  numdi = dif.length > numdi ? dif.length : numdi
  srs += `      ${dif.length}, // dif.length \n `
  if (dif.length==0){
    srs += `     {} // dif:{sr, sra, srb, diffon, diffoff, maxa, maxb, onoff} \n`
  }else{
    srs += `     { // dif:{sr, sra, srb, diffon, diffoff, maxa, maxb, onoff} \n`
    dif.map((d,i)=>{
      srs += `        {${d.sr}, ${d.sra}, ${d.srb}, ${d.difon}, ${d.difoff}, ${d.maxa}, ${d.maxb}, ${d.onoff}}${i+1==dif.length?' ':','} // ${d.label}\n`
    })
    srs+= `       },\n`
  }
  srs += `    }`
  return srs
}
console.log(`${introsrs} ${MKsrs(devobj)};\n`);
cfgc.write(`${introsrs} ${MKsrs(devobj)};\n`);

const MKxdata=()=>{
  let xdstr = `
/*xdata extern data struct initalization*/ 
xdata_t xdata { 
  ${numxdevs}, //numxdevs
` 
  if(numxdevs==0){
    xdstr += `{}
    };
    `
  } else {
    for(let i= 1; i<=numxdevs; i++){
        const xdev = Object.keys(cfgdata)[i]
        const fdata = cfgdata[xdev].filter((d)=>{
            return d.inxdata == true
        })
        // console.log('cfgdata[xdev]: ', xdev, i, Object.keys(cfgdata[xdev][i]));
        // if(cfgdata[xdev][i].inexdata)
        const devtpc =xdev +"srstate"

        xdstr += `  { \n    "${devtpc}", //xdev \n`
        xdstr += `    ${MKsrs(fdata)}\n  }${i==numxdevs?' ':',\n'}`
    }
    xdstr += `\n};`
  }
  return xdstr
}
console.log(MKxdata());
cfgc.write(MKxdata());

const MKprgs=()=>{
  let pstr = `
/*prgs extern data structure initalization*/ 
prgs_t prgs{  
`


  const prgs = devobj.filter(d=>{
    if(d.hasOwnProperty('hayprg')) {
      return d.hayprg == true
    }
  })
  numprgs =prgs.length
  pstr +=`  ${numprgs}, // numprgs \n`
  if(numprgs == 0){
    pstr += `  {}\n`
  }else{
    pstr += `  { // prg:{sr,aid,ev,numdata,prg[[]],hms} \n`
    prgs.map((e,i)=>{
      switch(e.type){
        case "cs":{
          pstr += `    {${e.sr}, 255, 1, 2, {{0,0,${e.hi},${e.lo}}}, ${1500+2*i}}${i+1==numprgs?' ':','}  \n`
          break;
        }
        case "cr":{
          pstr += `    {${e.sr}, 255, 1, 2, {{0,0,${e.onoff},${e.tsec}}, ${1500+2*i}}${i+1==numprgs?' ':','}  \n`
          break;
        }
        case "rel":{
          pstr += `    {${e.sr}, 255, 1, 1, {{0,0,${e.onoff}}}, ${1500+2*i}}${i+1==numprgs?' ':','}  \n`
          break;
        }
        case "dif":{
          pstr += `    {${e.sr}, 255, 1, 2, {{0,0,${e.difon},${e.difoff}}}, ${1500+2*i}}${i+1==numprgs?' ':','}  \n`
          break;
        }
        default:{
          break
        }
      }
    })
    pstr += `  }\n`
  }
  pstr += `};\n`
  return pstr
}
console.log(MKprgs());
cfgc.write(MKprgs());

const flags =`
/*flags extern data structure*/
flags_t f {
  0,//cONNectd
  0,//hayWIFI
  0,//hayMQTT
  0,//aUTOMA
  0,//fORCErESET
  5,//cREMENT
  0,//HAStIMR 1100000 64+32=96
  0,//IStIMERoN
  198,//HAYpROG 11000010 =128+64+4=198
  1023,//HAYsTATEcNG
  0,//CKaLARM
  0,//ISrELAYoN
  {0,0,0,0,0,0,0,0,0,0}//tIMElEFT[10]
};
`
cfgc.write(flags)


/*create CONFIG.h ----------------------------------------------- */
const configha = 
`
#ifndef CONFIG_h
#define CONFIG_h

#include <TimeLib.h>
#include <TimeAlarms.h>
#include <ESP8266WebServer.h>
#include <PubSubClient.h>

extern PubSubClient client;

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
extern const topics_t TPC ;

/*PORTS*/
struct port_t {
  int sr;
  int in;
  int out;
  int rec;
  int isnew;
};
struct ports_t {
  int numports;
  port_t port[${numsr}]; /*MODIFY*/
};
extern ports_t ports ;
/*PORT*/

/*START SE constant declarations*/  
struct senso_t{
  int nums;
  int ids[6]; //srs/portin
  char type[12];
  char model[12];
};
struct sen_t {
  int numtypes;
  int numsens;
  senso_t stype[${numtypes}];
};
extern const sen_t SE;
/*END SE constant declarations*/ 

/*START srs data structure declarations*/ 
struct se_t {//sensors
    int sr;
    int reading;
};
struct cs_t {//controlled sensors
  int sr;
  int reading;
  bool onoff;
  int hi;
  int lo;
};
struct cr_t {//controlled relay
  int sr;
  int tsec;
  bool onoff;
};
struct rel_t {//timers
    int sr;
    bool onoff;
};
struct di_t {//diff control
    int sa;
    int sb; 
    int don;
    int doff;
    int maxa;
    int maxb;
    bool onoff;
};
struct srs_t {
  int numsr;
  int numse;
  se_t se[${numse}];/*MODIFY*/
  int numcs;
  cs_t cs[${numcs}];/*MODIFY*/
  int numcr;
  cr_t cr[${numcr}];/*MODIFY*/
  int numrel;
  rel_t rel[${numrel}];/*MODIFY*/
  int numdi;
  di_t di[${numdi}];/*MODIFY*/
};
extern srs_t srs;
/*END srs data structure declarations*/  

/*START xdata structure declarations*/
struct xda_t {
  char xdev[17];
  srs_t xrs;
};
struct xdata_t {
  int numdevs;
  xda_t xda[${numxdevs}];
};
extern xdata_t xdata;
/*END xdata structure declarations*/


/*START prg data structure declarations*/  
struct prg_t{
  int sr;
  AlarmID_t aid;
  int ev;
  int numdata;
  int prg[11][4];//max 11 events [hr,min,max,min]  
  int hms;
};
struct prgs_t{
  int numprgs;
  prg_t prg[${numprgs}];/*MODIFY*/
};
extern prgs_t prgs;
/*END prg data structure declarations*/  

/*flags*/
struct flags_t{
  int cONNectd;
  int hayWIFI;
  int hayMQTT;
  bool aUTOMA;
  bool fORCErESET;  
  int cREMENT;
  int HAStIMR; //11100(28) 4,8, and 16 have timers not temp
  int IStIMERoN;//11100 assume some time left, timers with tleft>0 
  int HAYpROG;// = senrels with events>1
  int HAYsTATEcNG; //11111(31 force report) some state change int or ext
  int CKaLARM; //11111 assume alarm is set at start
  int ISrELAYoN;// = summary of relay states  
  int tIMElEFT[10];// =[0,0,56,0,0] timeleft in timrs
};
extern flags_t f;

struct iscsidx_t {
  int srtype;
  int idx;
};

extern char itopic[40];
extern char ipayload[256];
extern char idev[17];
extern unsigned int ipaylen;
extern bool NEW_MAIL;

#endif
`

cfgh.write(configha)

/*app intiState --------------------------------------------------------*/
const MKinist=()=>{
    let ini = 
`/*  
temp_out: darr:[]
tstat: {pro:[[0,0,65,63]], timeleft:0, darr:[reading,onoff,66,64]},
tv: {pro:[[0,0,hi,lo]], darr:[reading,onoff,hi,lo]},
pond: {pro:[[0,0,0],[19,15,1]], timeleft:0, darr:[0,0,0]},
//solar: darr:[sra_rdg, srb_rdg, difon, difoff,maxa, maxb onoff]
*/
const initialState = {
  //pro must start at 0,0
`
  const keys = Object.keys(cfgdata)
  keys.map((k)=>{
    const fdata = cfgdata[k].filter((d)=>{
      return typeof d.inapp === 'undefined' || d.inapp == true
    })
    fdata.map((e,i)=>{
      switch(e.type){
        case "se":{
          ini += `  ${e.label}: { darr: [${e.reading}] },\n`
          break; 
        }
        case "cs":{
          let prg=''
          if(e.hayprg){
            prg= `, pro: [[0, 0, ${e.hi}, ${e.lo}]]`
          }
          ini += `  ${e.label}: { darr: [${e.reading}, ${e.onoff}, ${e.hi}, ${e.lo}]${prg} },\n`
          break;
        }
        case "rel":{
          let prg=''
          let timr=''
          if(e.hayprg){
            prg= `, pro: [[0, 0, ${e.onoff}]]`
          }
          if(e.haytimr){
            timr = `, timeleft: 0`
          }
          ini += `  ${e.label}: { darr: [${e.onoff}]${prg}${timr}},\n`
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
  }) 
  ini= ini.slice(0,-2)+`\n}\n\n`
  ini +=  `const appid = "${appid}" \n\n`
  ini += `export {initialState, appid}`
  return ini 
}
console.log('MKinist(): ', MKinist());
appd.write(MKinist())
/*app intiState --------------------------------------------------------*/

/*create an empty appid.md  --------------------*/
const md = `
# ${appid}


## ${devid}
`
if(!fs.existsSync(`${appid}.md`)){
  const inst = fs.createWriteStream(`${appid}.md`);
  inst.write(md)
  inst.end()
}
/*create an empty appid.md ----------------------------------- */

sql.end()
appd.end()
cfgc.end()
cfgh.end()

