
REPLACE INTO `devs` (`devid`, `owner`, `devpwd`, `locid`, `description`) VALUES ( 
  'CYURD128', 
  'tim@sitebuilt.net',
  'geniot',
  '255ChestnutAve',
  ''
);

REPLACE INTO `app_loc` (`appid`, `locid`, `devs`, `zones`) VALUES (
  'doorStrike0',
  '255ChestnutAve',
  '{"CYURD128":[{"sr":0,"label":"contact","type":"se","senses":"onoff","model":"NCcontact","in":"D5","reading":1,"rec":1},{"sr":1,"label":"strike","type":"rel","out":"D1","onoff":0,"hayprg":1,"rec":1},{"sr":2,"label":"ledRed","type":"rel","out":"D8","onoff":1,"rec":0},{"sr":3,"label":"ledGreen","type":"rel","out":"D7","onoff":0,"rec":0},{"sr":4,"label":"ledBlue","type":"rel","out":"D6","onoff":0,"rec":0},{"sr":5,"label":"lr","type":"cs","senses":"temp","model":"ds18b20","in":"D0","out":"D4","reading":44,"onoff":0,"hi":69,"lo":67,"hayprg":1,"rec":1},{"sr":6,"label":"mb","type":"cs","senses":"temp","model":"ds18b20","in":"D0","out":"D1","reading":33,"onoff":0,"hi":70,"lo":40,"hayprg":1,"rec":1}]}',
  '[{"id":"contact","name":"NC door contact","img":"contact.jpg"},{"id":"strike","name":"NC magnetic door strike","img":"strike.jpg"},{"id":"ledRed","name":"door ledRed","img":"ledRed.jpg"},{"id":"ledGreen","name":"door ledGreen","img":"ledGreen.jpg"},{"id":"ledBlue","name":"door ledBlue","img":"ledBlue.jpg"}]'
);

REPLACE INTO `app_loc_user` (`appid`, `userid`, `locid`, `role`,`auth`) VALUES (
  'doorStrike0',
  'tim@sitebuilt.net',
  '255ChestnutAve',
  'admin',
  1
); 
  
REPLACE INTO `app_loc_user` (`appid`, `userid`, `locid`, `role`,`auth`) VALUES (
  'doorStrike0',
  'mckenna.tim@gmail.com',
  '255ChestnutAve',
  'user',
  1
); 
  
/*
These are SQL commands utlity commands
*/

SELECT * FROM `devs` ORDER BY `id` DESC;
SELECT * FROM `app_loc_user` ORDER BY `id` DESC;
SELECT * FROM `app_loc` ORDER BY `id` DESC;

