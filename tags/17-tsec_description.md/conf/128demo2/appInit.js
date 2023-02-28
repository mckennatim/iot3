/*  
temp_out: darr:[]
tstat: {pro:[[0,0,65,63]], timeleft:0, darr:[reading,onoff,66,64]},
tv: {pro:[[0,0,hi,lo]], darr:[reading,onoff,hi,lo]},
pond: {pro:[[0,0,0],[19,15,1]], timeleft:0, darr:[0,0,0]},
//solar: darr:[sra_rdg, srb_rdg, difon, difoff,maxa, maxb onoff]
*/
const initialState = {
  //pro must start at 0,0
  temp2: { darr: [45] },
  temp: { darr: [44, 0, 69, 67], pro: [[0, 0, 69, 67]], timeleft: 0 },
  hum: { darr: [24, 0, 90, 60] },
  timr1: { darr: [0], pro: [[0, 0, 0]], timeleft: 0 },
  temp_out: { darr: [55] }
}

const appid = "demo2" 

export {initialState, appid}