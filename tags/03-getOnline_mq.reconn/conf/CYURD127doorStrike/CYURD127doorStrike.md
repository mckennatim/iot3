# CYURD127doorStrike

## description
### components
[Door strike](https://www.amazon.com/dp/B071HBR584?psc=1&ref=ppx_yo2ov_dt_b_product_details) for 255 Chestnut Ave front door. Strike is operated through a 12 DC relay. Other components include an [ACDC switching power supply w dual 5V 1A, 12V 8A output](https://www.ebay.com/itm/383719573513), a [NC magnetic contact door](https://www.amazon.com/dp/B075Q4NJBB?psc=1&ref=ppx_yo2ov_dt_b_product_details) and a [4pin multicolor LED](https://www.amazon.com/gp/product/B077XGF3YR/ref=ppx_yo_dt_b_asin_title_o00_s00?ie=UTF8&psc=1)

### operation
When the door is closed, with contacts closed and the strike is locked (not powered, fail secure) the LED is red.  When the button is pushed on the app the strike is released and the LED goes green. Once the door is opened the LED turns blue, which signals the app to turn unpush the button and lock the stike. Once the door closes and the contacts connect, the led turnd red.

### configuration
The device has: 

- 1 input: the magnetic door contact
  - D2 - sr0 contact
- 4 outputs: the strike and the red, green and blue LED. \
  - D6 - sr1 redLED
  - D7 - sr2 greenLED
  - D8 - sr3 blueLED
  - D1 - sr4 strike

### from an existing code standpoint the operation can be described:

When you push the button on the app
- some command is sent to the device that opens the door like from 
  - E:\fs\www\hooks3\iot\cascada2\src\components\Control.jsx\changeTo() publishes:
    - topic: CYURD127/cmd, payload: {"id":4, "sra":[1]}
- the device turns on the greenLED    
- ideally the device listens for a change of state in the magnetic contact then... 
  - turns the light to blueLED 
  - turns off the strike
    - topic: CYURD127/cmd, payload: {"id":4, "sra":[0]}
  - sends the state of the LED and the strike back to the app  

Automatically
- once the door closes and
  - the magnetic contact is connected and
  - the stike is off
- change the LED to red and send its state to the app

In summary
- This application reacts to:
  - pressing the open button on the app
    - opens the strike
    - turns on the greenLED
  - a change in the magnetic latch
    - if 0 -> 1: turn on the redLED and send state
    - if 1 -> 0: turn on the blueLED, turn off the strike and send state

### So what code can it use and what code does it need?

The strike app will reuse the mqtt messaging formats of existing code. It will use the authentication code. It will use publish srstate and read cmd.
It will listen for changes in the mag contact. 

### How to approach the project?

- get some previous code to flash to the device
- assign new ports and srs and see if it will flash
- pare down the code to only do what the app needs

