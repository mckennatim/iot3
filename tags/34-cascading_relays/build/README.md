# Building a new single 8266 device project
:dog2: means ask Tim

The basic steps are...

in the `my/build` directory

1. [choose a circuit board](#choose_a_circuit_board) from `my/boards`
1. open appdata.js from `my/build`
2. [modify devinfo and apploc](3modify_devinfo_and_apploc) then `save`
4. run `node createApp.js` 
4. switch to your project config directory `cd ../conf/{devid_appid}`
4. [add your configuration data](#add_your_configuration_data)
    1. add sensor relay controllers you need `srs`
    4. add input devices that will communicate with controllers `inputs`
    4. add external data sources that your project will draw on `xdata`(if any)
4. run appBuild.js `node appBuild.js`
5. if `{devid}/CONFIG.cpp` and `{devid}/CONFI.h` look good, run `node copyConfig.js`
6. see if it compiles
7. see if it uploads
8. see if it runs

## choose a circuit board
* 
## modify devinfo and apploc

In particular...
* choose a devid next in line of deployed devices CYURD128, CYRURD128... :dog2:
* choose an appid
* indicate which board you will use
* list the owner and a user
* include the password :dog2:
* add location

## add your configuration data


https://myprojects.geoapify.com/api/se4O2gkXFwbN9zE8K0No/keys

https://api.geoapify.com/v1/geocode/search?text=38%20Upper%20Montagu%20Street%2C%20Westminster%20W1H%201LJ%2C%20United%20Kingdom&apiKey=d683f58bb9f348a5945a5e7af5d9f2ab

{"type":"FeatureCollection","features":[{"type":"Feature","properties":{"datasource":{"sourcename":"openstreetmap","attribution":"© OpenStreetMap contributors","license":"Open Database License","url":"https://www.openstreetmap.org/copyright"},"country":"United Kingdom","country_code":"gb","state":"England","county":"Greater London","city":"London","postcode":"W1H 1LJ","suburb":"Marylebone","street":"Upper Montagu Street","housenumber":"38","lon":-0.16030636023550826,"lat":51.52016005,"state_code":"ENG","formatted":"38 Upper Montagu Street, London, W1H 1LJ, United Kingdom","address_line1":"38 Upper Montagu Street","address_line2":"London, W1H 1LJ, United Kingdom","category":"building.residential","timezone":{"name":"Europe/London","offset_STD":"+00:00","offset_STD_seconds":0,"offset_DST":"+01:00","offset_DST_seconds":3600,"abbreviation_STD":"GMT","abbreviation_DST":"BST"},"result_type":"building","rank":{"importance":0.41000999999999993,"popularity":8.988490181891963,"confidence":0.95,"confidence_city_level":1,"confidence_street_level":1,"match_type":"full_match"},"place_id":"51dcb14637eb84c4bf59c6b7c19a94c24940f00102f901370cef1100000000c00203"},"geometry":{"type":"Point","coordinates":[-0.16030636023550826,51.52016005]},"bbox":[-0.160394,51.5201061,-0.1602251,51.5202273]}],"query":{"text":"38 Upper Montagu Street, Westminster W1H 1LJ, United Kingdom","parsed":{"housenumber":"38","street":"upper montagu street","postcode":"w1h 1lj","district":"westminster","country":"united kingdom","expected_type":"building"}}}