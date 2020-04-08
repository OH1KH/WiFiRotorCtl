# WiFiRotorCtl
This folder holds rotor controller that has static values, but can be configured by sending all parameters via tcp.
WiFi network connection is hard coded and it also defines a hard coded accesspoint so you can connect to it either via your defined WiFi, or rotor control's own accesspoint.

It happened that I have several ESP12e modeules that false reports flash size as 2M where it should report 4M  (cheap Chinese...)
With those ones eeprom or SPIFFS saving does not work and also network manager is unstabile.
So this way I can use them.
