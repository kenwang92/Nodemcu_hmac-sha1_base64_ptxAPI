# Nodemcu_hmac-sha1_base64_ptxAPI_OLED
1.Apply an account on [PTX](https://ptx.transportdata.tw/PTX/)  
2.Get `APP ID`&`APP KEY` from email authentication  
3.Put your `APP ID`&`APP KEY` in code and setup WiFi `ssid` `passwd`  
4.Download some libraries as the below!
  * `hmac-sha1` https://github.com/Cathedrow/Cryptosuite  
  * `OLED` https://github.com/ThingPulse/esp8266-oled-ssd1306
    * You have to creat your own fonts library & add `#include <OLED fonts library>` in code
       * More information click [here](https://github.com/ThingPulse/esp8266-oled-ssd1306)
       * `OLED FONTS Create` http://oleddisplay.squix.ch/#/home

5.Give me a star if you like it thanks!  
***
Some Code from :  
* https://forum.arduino.cc/index.php?topic=342528.0  
- https://github.com/jalmeroth/ESP8266-OAUTH2/blob/master/ESP8266-OAUTH2/ESP8266-OAUTH2.ino  
* https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266WiFi/examples/HTTPSRequest/HTTPSRequest.ino 
- https://github.com/adafruit/Adafruit-Tweet-Receipt/blob/master/Gutenbird.ino   
資料介接「交通部PTX平臺」&平臺標章![PTX LOGO](https://github.com/kenwang92/Nodemcu_hmac-sha1_base64_ptxAPI/blob/master/PTX_LOGO.png)
