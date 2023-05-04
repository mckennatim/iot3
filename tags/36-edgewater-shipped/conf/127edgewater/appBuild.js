import{ cfgdata, devinfo, apploc, senstypes, ctrltypes, topics} from './appdata.js'
import fs from 'fs'
import {hhead, hfoot} from '../../build/constCONFIG.h.js'
import {chead, cfoot} from '../../build/constCONFIG.cpp.js'
import {D0,D1,D2,D3,D4,D5,D6,D7,D8,RX,TX,SCL,SCLK,MISO,MOSI,CS}from '../../build/constants.js'


const {devid, owner,pwd, mqtt_server,mqtt_port} = devinfo
const {appid, locid, user, descr, board} = apploc
const ctrtypk = Object.keys(ctrltypes)

const devstr =
`
/*dev device variables*/  
char devid[9]="${devid}";
char owner[254]="${owner}";
char pwd[24]="${pwd}";
char mqtt_server[60]="${mqtt_server}";
char mqtt_port[6]="${mqtt_port}";
`

const {srs,inputs, xdata} = cfgdata

const NUMSR = srs.length;
const NUMINP = inputs.length
const NUMTYP = ctrtypk.length

const xdevs = Object.keys(xdata);
const NUMXDEV = xdevs.length
const SENSTYPS = senstypes.length


let NUMXD =0
xdevs.map(x=>{
  NUMXD +=xdata[x].length
})
console.log('NUMXD: ', NUMXD);



const prgs = srs.filter((s)=>s.prg && s.init[3]==-1 && s.init[2]==-1).map((r)=>{
  if (r.prg){
    return r.prg
  }
})
const NUMPRGS = prgs.length

console.log('NUMSR, NUMINP,NUMXD,prgs: ', NUMSR, NUMINP,NUMXD,prgs);
console.log('NUMPRGS: ', NUMPRGS);



const reca = srs.map((s)=>s.rec==true ?1:0).reverse()
const dOrEC = '0b'+reca.join('')
const prga = srs.map((s)=>s.prg ?1:0).reverse()
const HAYpROG= '0b'+prga.join('')
const tima = srs.map((s)=>s.timr ?1:0).reverse()
const HAYtIMR= '0b'+tima.join('')

console.log('dOrEC: ', dOrEC);
console.log('HAYpROG: ', HAYpROG);
console.log('HAYtIMR: ', HAYtIMR);

console.log('Object.keys(D2)[0]: ', Object.keys({D2})[0], D2);

console.log('Object.keys(D5)[0]: ', Object.keys({D5})[0], D5);


const mkSrsStr = ()=>{
  let srstr = 
`
srs_t srs[NUMSR] ={//{sr, port, typidx, data[]}   
`
  srs.map((s)=>{
    const tidx = ctrtypk.findIndex(c=>c == s.ctype)
    let inistr = `{`
    s.init.map(i=>inistr+=`${i}, `)
    inistr=inistr.slice(0,-1)+'}';
    srstr += `  {${s.sr}, ${s.out}, ${tidx}, ${inistr}}, //${s.ctype} ${s.label} \n`
  })
  srstr+=`};\n`
  return srstr
}

console.log(mkSrsStr());

const mkCmdStr=()=>{
  let cmdstr =
`
const cmd_t cmds[NUMSR] ={//{sr,nda,data{})
`  
  srs.map((s)=>{
    const tidx = ctrtypk.findIndex(c=>c == s.ctype)
    const cmd = ctrltypes[ctrtypk[tidx]].cmd;
    let inistr = `{`
    cmd.map(i=>inistr+=`${i}, `)
    inistr=inistr.slice(0,-2)+'}';
    cmdstr+= `  {${s.sr}, ${cmd.length}, ${inistr}}, //${s.ctype} ${s.label}\n`
  })
  cmdstr+=`};\n`
  return cmdstr
}

console.log(mkCmdStr());

const mkPrgStr = ()=>{
  let prgstr = 
`
prgs_t prgs[NUMPRGS]={//{sr, dax, aid, ev, prg[[h,m,set]], hms} 
`
  srs.filter((p)=>p.prg && p.init[3]==-1 && p.init[2]==-1).map((s)=>{
    const ctrtyps = Object.keys(ctrltypes)
    const tidx = ctrtyps.findIndex(c=>c == s.ctype)
    const dax = ctrltypes[ctrtyps[tidx]].prg[0];
    let estr = `{`
    s.prg.map((r, i)=>{
      estr += `{`
      r.map((q,j)=>estr+=`${s.prg[i][j]},`)
      estr=estr.slice(0,-1)+`},`
    })
    estr=estr.slice(0,-1)+`}`
    prgstr += ` {${s.sr}, ${dax}, 255, ${s.prg.length}, ${estr}},\n`
  })
  prgstr += `};\n`
  return prgstr
}

console.log(mkPrgStr());

const mkSentstr =()=>{
  let xdstr = 
`
const char sensors[SENSTYPS][MAXSSTR] = 
{`
  senstypes.map(x=>xdstr+=`"${x}", `)
  // xdstr = `${xdstr.slice(0,-2)}`
  xdstr += `};\n`
  return xdstr
}
console.log(mkSentstr());

const mkxdstr =()=>{
  let xdstr = 
`
const char xdevtpc[NUMXDEV][MAXDSTR] = 
{
`
  xdevs.map(x=>xdstr+=`"${x}/srstate", `)
  // xdstr = `${xdstr.slice(0,-2)}`
  xdstr += `};\n`
  return xdstr
}
console.log(mkxdstr());

let MAXDT =0 
const mkXdastr=()=>{
  let xdastr = 
`
const xdata_t xdata[NUMXD] = 
{
`
  xdevs.map((x,i)=>{
    // xdastr +=`{`
    xdata[x].map((y)=>{
      if(y.dt.length>MAXDT) MAXDT=y.dt.length
      xdastr += `  {${i}, ${y.sr}, ${y.dt.length},//dev,sr,nd \n    { //xdidx, {numtargs {{sr, tdidx}}}\n`
      y.dt.map((z)=>{
        xdastr += `      {${z.xdidx}, {${z.targets.length}, {`
        z.targets.map(a=>{
          xdastr +=`{`
          a.map(b=>{
            xdastr += `${b}, `
          })
          xdastr =xdastr.slice(0,-2)+`}}}}, \n`
        })
        // xdastr +=` },\n`
      })
      xdastr +=`    },\n  },\n`
    })
    // xdastr+=`};\n`
  })
  xdastr+=`};\n`
  return xdastr
}
console.log(mkXdastr());

const MkInpstr=()=>{
  let inpstr = 
`
inp_t inp[NUMINP] = {
`

  inputs.map((y)=>{
    let pstr = `{${y.ports.map(p=>`${p},`)}}`;
    inpstr += `  {${pstr}, "${y.itype}", ${y.targets.length}, \n    { `

    y.targets.map((z)=>{
      let tstr =`{`
      z.map(t=>{
        tstr+=`{${t[0]}, ${t[1]}}, `
      })
      tstr+=`}`
      inpstr+=`{${z.length}, ${tstr}},\n      `
    })
    inpstr=inpstr.slice(0,-7)+` },`
    let astr=``
    if(y.actions){y.actions.map(a=>{
      astr+=`${a},`
    })}
    inpstr+=`\n    {${astr}}, },\n`
  })
  inpstr+=`};\n`
  return inpstr
}

console.log(MkInpstr());
const flgstr = 
`
/*flags extern data structure*/
flags_t f {
  ${HAYtIMR},//HAYtIMR 1100000 64+32=96
  ${HAYpROG},//HAYpROG 11000010 =128+64+4=198
  ${dOrEC},//dOrEC`

const hvar =
`
/*project constants for ${devid+appid}*/
#define NUMXDEV ${NUMXDEV} //# of x devices
#define NUMXD ${NUMXD} //# of xdata records
#define NUMSR ${NUMSR} //number of sr's 
#define NUMINP ${NUMINP}
#define NUMPRGS ${NUMPRGS}
#define MAXDT ${MAXDT}
#define SENSTYPS ${SENSTYPS}
#define NUMTYP ${NUMTYP}

`

const mkTdsStr =()=>{
  let tdstr =
`
const tds_t tds[NUMTYP] ={
`
  ctrtypk.map(c=>{
    const nd = ctrltypes[c].lab.length
    let dstr = `{`
    ctrltypes[c].lab.map(l =>{
      dstr += `"${l}", `
    })
    dstr += `}`
    tdstr += `  {"${c}", ${nd}, ${dstr}},\n`
    
  })
  tdstr += `};\n`
  return tdstr
}  

console.log('mkTdsStr(): ', mkTdsStr());


const mkIniSh=()=>{
  let istr = 
`
void initShit(){
`  
  cfgdata.srs.map((c,i)=>{
    istr += `  pinMode(srs[${i}].port, OUTPUT);\n`
  })
  cfgdata.inputs.map((p,y)=>{
    istr += `  pinMode(inp[${y}].port[0], INPUT);\n`
  })
  istr += `}\n`
  return istr
}

console.log('mkIniSh(): ', mkIniSh());

let hstr = `${hhead} ${hvar} ${hfoot}`
let cstr = `${chead} ${devstr} ${mkIniSh()} ${mkSrsStr()} ${mkCmdStr()}  ${mkPrgStr()} ${MkInpstr()} ${mkxdstr()} ${mkXdastr()} ${mkSentstr()} ${mkTdsStr()} ${flgstr} ${cfoot}`

console.log(hstr);


const mkCONF=()=>{
  const hconf = fs.createWriteStream(`${devid}/CONFIG.h`)
  hconf.write(hstr)
  hconf.end()
  const cconf = fs.createWriteStream(`${devid}/CONFIG.cpp`)
  cconf.write(cstr)
  cconf.end()
}

mkCONF()

const zfilt = srs.filter(f=>{
  if (f.prg && f.init[2]==-1) {
    return true
  } else {
    return false
  }
})
    
console.log('zfilt: ', zfilt);  


