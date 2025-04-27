
import fs from 'fs'

const src= '../../../src'

fs.copyFile(`${src}/CONFIG.h`, `CONFIG.h`,(err)=>{
  if (err){
    console.log('err: ', err);
  }
  console.log('CONFIG.h is copied from my/src ');
})

fs.copyFile(`${src}/CONFIG.cpp`, `CONFIG.cpp`,(err)=>{
  if (err){
    console.log('err: ', err);
  }
  console.log('CONFIG.cpp is copied from my/src ');
})

fs.copyFile(`${src}/main.cpp`, `main.cpp`,(err)=>{
  if (err){
    console.log('err: ', err);
  }
  console.log('main.cpp is copied from my/src ');
})

