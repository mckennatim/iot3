# project build system 
The project build system creates a custom project based on fs/iot3/my base project. It sets up a new `conf/`, from which the specifics of the custom project can be specified. 

Building a project from `my/conf/projectid/ ` copies the CONFIG.cpp and CONFIG.h files into the my/src directory. It also sends information to the server's database so the project can be identified and authorized by the server. 

Each project has an associated react app. That app uses the information in a `initState.js` file in the conf directory. That file gest sopied to the react app.

Now you go to `my/src/main.cpp` and tweak it until the it compiles and does what it is supposed to do. From the `conf/appid/devid` directory you can run 'getMain.js` to have a copy of the tweaked main.cpp file associtaed with each project and device.

## my/appCreate.js
 
- `node my/appCreate.js` creates a new application based upon values chosen 
  - const devid = "CYURD128" /*higher number is most recent project*/
  - const appid = "demo2" /*MODIFY*/
  - const locid = "12ParleyVale" /*MODIFY*/
  - const user = "mckenna.tim@gmail.com" /*MODIFY*/
  - const board = "long.pdf" /* one of the board options from `my/boards/` */
 
Running `node my/appCreate.js` creates a new directory `conf/128appid`, copies `appbuild.js` to it and  creates an `appdata.js` file in it. It also creates an `conf/128/appid/CYURD128` directory and creates a file called `copyCONFIG.js` in it.  

## building my/conf/projectId/

`appdate.js` is a template for a project.  Here, `devinfo` and `apploc` info are specified. Each device in a project has  a list of sensors and relays`sr`'s that it contains. Depending the `sr` type, it can contain other info. 

From the cutomized `appdata.js` the necessary device and project configuration files and database modifications are generated.

Each `sr` has a label and a description that the web app uses to keep track of and modify the state of the system. The web app uses information contained in `initState` as its base configuration. 

## trying the configuration in my/src and running the device from a web app.

The device CONFIG.cpp and CONFIG.h are then copied by running `node copyCONF.js` for a device. Ideally it should compile and run and be flashable to a blank esp8266 device. 

Using a copy of `initState.js` placed in a wsl:/home/tim/www/react/v18/iot/src/ directory, a prototype demo of the device state and operation by the web app can be created and the device function can be verified.

## saving a project configuration and sendig it tp GITHUB

Once a minimally viable project MVP is built, `getCONF.js` can be run from conf/projectId/devid to have a current best copy of the project's main.cpp, CONFIG.cpp and CONFIG.h files in conf/projectId/devid.

At this point and all along the way, it is wise to send everything to git repositories. 






