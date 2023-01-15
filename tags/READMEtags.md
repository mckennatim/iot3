# tags

## corrections
### 04-WIFI_MQTTconn-v2
In order to work it needs this in loop()

    if(f.cONNectd){
        client.loop();
    }