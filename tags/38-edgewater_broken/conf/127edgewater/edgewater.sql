

REPLACE INTO `devs` (`devid`, `owner`, `devpwd`, `locid`, `description`) VALUES ( 
  'CYURD127', 
  'tim@sitebuilt.net',
  'geniot',
  '12ParleyVale',
  'use toggle button to start sequentially timed relays controlling irrigation'
);

REPLACE INTO `app_loc` (`appid`, `locid`, `devs`, `zones`) VALUES (
  'edgewater',
  '12ParleyVale',
  '
{
  "CYURD127": [
    {"sr": 0, "label": "zone1"},
    {"sr": 1, "label": "zone2"},
    {"sr": 2, "label": "zone3"},
    {"sr": 3, "label": "zone4"},
    {"sr": 4, "label": "zone5"} 
  ],
},
{
  "CYURD006": [
    {"sr": 0, "label": "temp_out"},
    {"sr": 1, "label": "temp_attic"} 
  ]
},
{
  "CYURD018": [
    {"sr": 0, "label": "gh_hum"} 
  ]
} 
', 
  '
[
    {"id": zone1, "label": "irrigation zone 1", "img": "zone1.png"},
    {"id": zone2, "label": "irrigation zone 2", "img": "zone2.png"},
    {"id": zone3, "label": "irrigation zone 3", "img": "zone3.png"},
    {"id": zone4, "label": "irrigation zone 4", "img": "zone4.png"},
    {"id": zone5, "label": "special zone", "img": "zone5.png"},
    {"id": temp_out, "label": "outside temperature", "img": "temp_out.png"},
    {"id": temp_attic, "label": "attic temperature", "img": "temp_attic.png"},
    {"id": gh_hum, "label": "greenhouse humidity", "img": "gh_hum.png"}
]
'
);

REPLACE INTO `app_loc_user` (`appid`, `userid`, `locid`, `role`,`auth`) VALUES (
  'edgewater',
  'tim@sitebuilt.net',
  '12ParleyVale',
  'admin',
  1
); 

REPLACE INTO `app_loc_user` (`appid`, `userid`, `locid`, `role`,`auth`) VALUES (
  'edgewater',
  'mckenna.tim@gmail.com',
  '12ParleyVale',
  'user',
  1
); 
    
REPLACE INTO `app_loc_user` (`appid`, `userid`, `locid`, `role`,`auth`) VALUES (
  'edgewater',
  'noahmckenna@gmail.com',
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

