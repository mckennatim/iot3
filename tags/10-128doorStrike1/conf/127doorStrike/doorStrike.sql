
SELECT * FROM `devs` ORDER BY `devs`.`devid` DESC;

DELETE FROM devs WHERE `devs`.`id` = 9

    INSERT INTO `devs` (`id`, `devid`, `owner`, `devpwd`, `locid`, `description`) VALUES (NULL, 'CYURD127', 
    'tim@sitebuilt.net',
    'geniot',
    '255ChestnutAve',
    'doorStrike with strike RGB LEDs and contacts. Part of entry system project that has doorbells, camera and aprtment UI.'
    );


    INSERT INTO `app_loc_user` (`appid`, `userid`, `locid`, `role`,`auth`) VALUES (
    'doorStrike',
    'tim@sitebuilt.net',
    '255ChestnutAve',
    'admin',
    1
    );    

    INSERT INTO `app_loc_user` (`appid`, `userid`, `locid`, `role`,`auth`) VALUES (
    'doorStrike',
    'mckenna.tim@gmail.com',
    '255ChestnutAve',
    'user',
    1
    );    

    SELECT * FROM `app_loc_user` ORDER BY `id` DESC;
    SELECT * FROM `app_loc` ORDER BY `id` DESC;
 
 
INSERT INTO `app_loc` (`appid`, `locid`, `devs`, `zones`) VALUES (
  'doorStrike',
  '255ChestnutAve',
  '{"CYURD127":[{"sr":0,"label":"contact","type":"se","io":"input","port":"D4"},{"sr":1,"label":"redLED","type":"rel","io":"output","port":"D8"},{"sr":2,"label":"greenLED","type":"rel","io":"output","port":"D7"},{"sr":3,"label":"blueLED","type":"rel","io":"output","port":"D6"},{"sr":4,"label":"strike","type":"rel","io":"output","port":"D1"}]}',
  '[{"id":"contact","name":"NC magnetic door contact","img":"contact.jpg"},{"id":"redLED","name":"closed & locked: redLED","img":"redLED.jpg"},{"id":"greenLED","name":"unlocked: greenLED","img":"greenLED.jpg"},{"id":"blueLED","name":"open: blueLED","img":"blueLED.jpg"},{"id":"strike","name":"Door Strike","img":"strike.jpg"}]'
  );
 

DELETE FROM devs WHERE devs.devid = 'CYURD127';

DELETE FROM devs WHERE devs.devid = 'CYURD127';

INSERT INTO `app_loc` (`appid`, `locid`, `devs`, `zones`) VALUES (
  'doorStrike',
  '255ChestnutAve',
  '{"CYURD127":[{"sr":0,"label":"contact","type":"se","io":"input","port":"D4"},{"sr":1,"label":"redLED","type":"rel","io":"output","port":"D8"},{"sr":2,"label":"greenLED","type":"rel","io":"output","port":"D7"},{"sr":3,"label":"blueLED","type":"rel","io":"output","port":"D6"},{"sr":4,"label":"strike","type":"rel","io":"output","port":"D1"}]}',
  '[{"id":"contact","name":"NC magnetic door contact","img":"contact.jpg"},{"id":"redLED","name":"closed & locked: redLED","img":"redLED.jpg"},{"id":"greenLED","name":"unlocked: greenLED","img":"greenLED.jpg"},{"id":"blueLED","name":"open: blueLED","img":"blueLED.jpg"},{"id":"strike","name":"Door Strike","img":"strike.jpg"}]'
  );
  
INSERT INTO `app_loc` (`appid`, `locid`, `devs`, `zones`) VALUES (
  'doorStrike',
  '255ChestnutAve',
  '{"CYURD127":[{"sr":0,"label":"contact","type":"se","io":"input","port":"D4"},{"sr":1,"label":"redLED","type":"rel","io":"output","port":"D8"},{"sr":2,"label":"greenLED","type":"rel","io":"output","port":"D7"},{"sr":3,"label":"blueLED","type":"rel","io":"output","port":"D6"},{"sr":4,"label":"strike","type":"rel","io":"output","port":"D1"}]}',
  '[{"id":"contact","name":"NC magnetic door contact","img":"contact.jpg"},{"id":"redLED","name":"closed & locked: redLED","img":"redLED.jpg"},{"id":"greenLED","name":"unlocked: greenLED","img":"greenLED.jpg"},{"id":"blueLED","name":"open: blueLED","img":"blueLED.jpg"},{"id":"strike","name":"Door Strike","img":"strike.jpg"}]'
  );
  
INSERT INTO `devs` (`devid`, `owner`, `devpwd`, `locid`, `description`) VALUES ( 
'CYURD127', 
'tim@sitebuilt.net',
'geniot',
'255ChestnutAve',
'doorStrike with strike RGB LEDs and contacts. Part of entry system project that has doorbells, camera and aprtment UI.',
);

INSERT INTO `devs` (`devid`, `owner`, `devpwd`, `locid`, `description`) VALUES ( 
'CYURD127', 
'tim@sitebuilt.net',
'geniot',
'255ChestnutAve',
'doorStrike with strike RGB LEDs and contacts. Part of entry system project that has doorbells, camera and aprtment UI.',
);
