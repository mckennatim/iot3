
REPLACE INTO `devs` (`devid`, `owner`, `devpwd`, `locid`, `description`) VALUES ( 
  'CYURD127', 
  'tim@sitebuilt.net',
  'geniot',
  '255ChestnutAve',
  'door strike app'
);

REPLACE INTO `app_loc` (`appid`, `locid`, `devs`, `zones`) VALUES (
  'doorStrike',
  '255ChestnutAve',
  '{
  "CYURD127": [
    {
      "sr": 0,
      "label": "contact"
    },
    {
      "sr": 1,
      "label": "strike"
    },
    {
      "sr": 2,
      "label": "ledRed"
    },
    {
      "sr": 3,
      "label": "ledGreen"
    },
    {
      "sr": 4,
      "label": "ledBlue"
    },
    {
      "sr": 5,
      "label": "lr"
    },
    {
      "sr": 7,
      "label": "mb"
    },
    {
      "sr": 6,
      "label": "music"
    }
  ],
  "CYURD006": [
    {
      "sr": 0,
      "label": "temp_out"
    }
  ]
}', 
  '
[
  {"id": "contact", "name": "a short description", "img": "contact.png" },
  {"id": "strike", "name": "a short description", "img": "strike.png" },
  {"id": "ledRed", "name": "a short descrition", "img": "ledRed.png" },
  {"id": "ledGreen", "name": "a short descrition", "img": "ledGreen.png" },
  {"id": "ledBlue", "name": "a short descrition", "img": "ledBlue.png" },
  {"id": "lr", "name": "a short descrition", "img": "lr.png" },
  {"id": "mb", "name": "a short descrition", "img": "mb.png" },
  {"id": "music", "name": "a third sensor from 1-wire", "img": "music.png" },
  {"id": "temp_out", "name": "outside temperature", "img": "temp_out.png" }
]'
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

