
REPLACE INTO `devs` (`devid`, `owner`, `devpwd`, `locid`, `description`) VALUES ( 
  'CYURD128', 
  'tim@sitebuilt.net',
  'geniot',
  '255ChestnutAve',
  ''
);

REPLACE INTO `app_loc` (`appid`, `locid`, `devs`, `zones`) VALUES (
  'doorStrike',
  '255ChestnutAve',
  '{"CYURD128":[{"sr":0,"label":"contact","type":"se","io":"input","port":"D5"},{"sr":1,"label":"ledRed","type":"rel","io":"output","port":"D6"},{"sr":2,"label":"ledGreen","type":"rel","io":"output","port":"D7"},{"sr":3,"label":"ledBlue","type":"rel","io":"output","port":"D8"},{"sr":4,"label":"strike","type":"rel","io":"output","port":"D1"}]}',
  '[{"id":"contact","name":"NC magnetic door contact","img":"contact.jpg"},{"id":"ledRed","name":"ledRed - door is locked","img":"ledRed.jpg"},{"id":"ledGreen","name":"ledGreen - door is unlocked","img":"ledGreen.jpg"},{"id":"ledBlue","name":"ledBlue - door is open","img":"ledBlue.jpg"},{"id":"strike","name":"NC magnetic door strike","img":"strike.jpg"}]'
);

REPLACE INTO `app_loc_user` (`appid`, `userid`, `locid`, `role`,`auth`) VALUES (
  'doorStrike',
  'tim@sitebuilt.net',
  '255ChestnutAve',
  'admin',
  1
); 
  
REPLACE INTO `app_loc_user` (`appid`, `userid`, `locid`, `role`,`auth`) VALUES (
  'doorStrike',
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



