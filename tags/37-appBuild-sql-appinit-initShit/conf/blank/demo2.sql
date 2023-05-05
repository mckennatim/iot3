
REPLACE INTO `devs` (`devid`, `owner`, `devpwd`, `locid`, `description`) VALUES ( 
  'CYURD128', 
  'tim@sitebuilt.net',
  'geniot',
  '12ParleyVale',
  'demo of long board with relay, DHT11 and DSB1820'
);

REPLACE INTO `app_loc` (`appid`, `locid`, `devs`, `zones`) VALUES (
  'demo2',
  '12ParleyVale',
  '{
  "CYURD128": [
    {
      "sr": 0,
      "label": "temp2"
    },
    {
      "sr": 1,
      "label": "temp"
    },
    {
      "sr": 2,
      "label": "hum"
    },
    {
      "sr": 3,
      "label": "timr1"
    },
    {
      "sr": 4,
      "label": "btn1"
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
  {"id": "temp2", "name": "demo temp sensor", "img": "temp2.png" },
  {"id": "temp", "name": "temp side of DHT11", "img": "temp.png" },
  {"id": "hum", "name": "temp side of DHT11", "img": "hum.png" },
  {"id": "timr1", "name": "a relay controlled by timer", "img": "timr1.png" },
  {"id": "btn1", "name": "a relay controlled by button", "img": "btn1.png" },
  {"id": "temp_out", "name": "outside temperature", "img": "temp_out.png" }
]'
);

REPLACE INTO `app_loc_user` (`appid`, `userid`, `locid`, `role`,`auth`) VALUES (
  'demo2',
  'tim@sitebuilt.net',
  '12ParleyVale',
  'admin',
  1
); 
  
REPLACE INTO `app_loc_user` (`appid`, `userid`, `locid`, `role`,`auth`) VALUES (
  'demo2',
  'mckenna.tim@gmail.com',
  '12ParleyVale',
  'user',
  1
); 
  
/*
These are SQL commands utlity commands
*/

SELECT * FROM `devs` ORDER BY `id` DESC LIMIT 3;
SELECT * FROM `app_loc_user` ORDER BY `id` DESC LIMIT 3;
SELECT * FROM `app_loc` ORDER BY `id` DESC LIMIT 3;

