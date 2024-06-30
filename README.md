Smart PID Incubator - Version 1.0

This repository contains the complete Arduino code for my Smart PID-controlled Incubator.

This project allows you to control the brightness of a DC bulb using a PID control method to maintain a stable temperature and humidity level. Additionally, it supports remote monitoring through the BLYNK IoT platform. You can also configure the system to store real-time temperature and humidity readings in a Google Sheet.

Components and Tools:

1) ESP-32 (The Micro Controller unit)
2) DHT-22
3) L298(Using Mosfet can be more cost efficient)
4) DC Bulb as heating element (here i used motor bike headlight)
5) 0.96 OLED display
6) Double channel relay module.
7) Cooling fan
8) Humidifire
9) Arduino IDE.
10) BLYNK IOT app for remote monitoring and notfication alert.

 
Limitations of Version 1.0:

You must need to connect with wifi, other wise the whole system will not work. You can't even see the temperature and humidity . So be carefull and i will upload the upadated code in the next version. Moreover i will add power mosfet instead of L298 driver so that we can handle more current. 
