# Smart-PID-Incubator

This is a complete arduino code of my Smart PID controlled Incubator Version -1.0
Here you can cotroll the brightness of a dc bulb with PID controlling method to stabilize the
required temperature & humidity as well. For remote monitoring i have used BLYNK IOT platform.


Here i have used the following components and tools--
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
 
As this is the version-1.0, there is a limitations in the current arduino code is you must need to connect with wifi, other wise the whole system will not work. You can't even see the temperature and humidity . So be carefull and you i will upload the upadated code in the next version and. Moreover i will add power mosfet instead of L298 driver so that we can handle more current. 
