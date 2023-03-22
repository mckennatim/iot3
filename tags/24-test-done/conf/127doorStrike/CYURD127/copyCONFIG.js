
import fs from 'fs'

const dest= '../../../src'

fs.copyFile('CONFIG.h', `${dest}/CONFIG.h`,(err)=>{
  if (err){
    console.log('err: ', err);
  }
  console.log('CONFIG.h is copied to my/src ');
})

fs.copyFile('CONFIG.cpp', `${dest}/CONFIG.cpp`,(err)=>{
  if (err){
    console.log('err: ', err);
  }
  console.log('CONFIG.cpp is copied to my/src ');
})

