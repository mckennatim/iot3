
REPLACE INTO `devs` (`devid`, `owner`, `devpwd`, `locid`, `description`) VALUES ( 
  'CYURD128', 
  'tim@sitebuilt.net',
  'geniot',
  '255ChestnutAve',
  ''
);

REPLACE INTO `app_loc_user` (`appid`, `userid`, `locid`, `role`,`auth`) VALUES (
  'doorStrike1',
  'tim@sitebuilt.net',
  '255ChestnutAve',
  'admin',
  1
); 
  
REPLACE INTO `app_loc_user` (`appid`, `userid`, `locid`, `role`,`auth`) VALUES (
  'doorStrike1',
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

