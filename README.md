# IR_Remote_ESP32_ESP8266

This repository contains codes for ESP32 and ESP8266 to allow these controllers serve as IR remotes.

Version hidden under System_IRDA folder is dedicated to ESP8266 chip with use of SDcard Reader via SPI.
Those folder includes http,css,js files and also C/C++ files for Arduino IDE, but can be used in platfromIO instead

Unfortunetly due to low available RAM on ESP8266 HTTPS requests cause corruption of HTTP headers.
Propably caused by big headroom of HTTPS certificates and symetric RSA key needed to store in IRAM.
For now I dont see porpouse to fix problem with this, instead I recommend to switch unsecure way of connection via HTTP
Feel free to download this repository and modify it on your own.

Under folder Parameters there is version written for ESP32 using HTTPS as secure server. 
This version was made to finnish my engineer degree, so feel free to download It and test it






