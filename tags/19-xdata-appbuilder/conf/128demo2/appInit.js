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
  temp: { darr: [44, 0, 69, 67] },
  hum: { darr: [24, 0, 68, 60] },
  timr1: { darr: [0], pro: [[0, 0, 0]] },
  ledBlue: { darr: [0] },
  lr: { darr: [44, 0, 69, 67], pro: [[0, 0, 69, 67]] },
  mb: { darr: [33, 0, 70, 40], pro: [[0, 0, 70, 40]] },
  temp_out: { darr: [55] },
  ledBlue: { darr: [0] },
  temp_out: { darr: [55] },
  ledBlue: { darr: [0] }
}

const appid = "demo2" 

export {initialState, appid}