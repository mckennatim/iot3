import{ devs, zones, initialState, devinfo, apploc} from './doorStrike.js'
import fs from 'fs'


console.log('devs: ', devs);

console.log('devs,zones: ', devs.CYURD127[2].port,zones[1]);

console.log('Object.keys(devs): ', Object.keys(devs)[0]);

// const dd = devs.map((d)=>{
//   console.log('d: ', d);
//})

// const sqdevd = "INSERT INTO `devs` (`devid`, devpwd, description, bizid, server,specs,owner)
// SELECT `devid`, devpwd, description, bizid, server, specs, owner
// FROM `devices`;'

const sq = `SELECT * FROM \`devs\` ORDER BY \`devs\`$a{devi}\` DESC;`
const de =`
DELETE FROM devs WHERE devs.devid = '${devinfo.devid}';
`

console.log(de);


fs.appendFile('./doorStrike.sql', de, function (err) {
  if (err) throw err;
  console.log('Saved!');
});

const MKports = ()=>{
  let ports = `/*ports */ \n ports_t ports{ \n`
  devs.CYURD127.map((d)=>{

    ports += `  {${d.port.toUpperCase()}, ${d.io.toUpperCase()}}, \\\\ sr:${d.sr} ${d.label} \n`;
  })
  ports += `}`
  return ports
}

const al =`
INSERT INTO \`app_loc\` (\`appid\`, \`locid\`, \`devs\`, \`zones\`) VALUES (
  '${apploc.appid}',
  '${apploc.locid}',
  '${JSON.stringify(devs)}',
  '${JSON.stringify(zones)}'
  );
  `

console.log('al: ', al);  
fs.appendFile('./doorStrike.sql', al, function (err) {
  if (err) throw err;
  console.log('Saved!');
});

const insdev = `
INSERT INTO \`devs\` (\`devid\`, \`owner\`, \`devpwd\`, \`locid\`, \`description\`) VALUES ( 
'${devinfo.devid}', 
'${devinfo.owner}',
'${devinfo.pwd}',
'${apploc.locid}',
'${apploc.descr}',
);
`

console.log('insdev: ', insdev);  
fs.appendFile('./doorStrike.sql', insdev, function (err) {
  if (err) throw err;
  console.log('Saved!');
});

console.log(MKports());