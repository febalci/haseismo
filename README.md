# HA Seismo - Earthquake Detection Sensor
Seismic Sensor with MQTT for use with Home Assistant


This project is an online Seismic Senor based on MPU6050 accelerometer. It sends xy and z gravity changes during an earthquake and send it to MQTT server.

## Hardware
### Required Components:
- Nodemcu
- MPU 6050 accelerometer
- Openlog (optional)
- DS3231 RTC
- Red, Yellow, Green LEDs
- Resistors for LEDs

### Fritzing:
![alt text](images/Seismo%20MPU6050.jpg?raw=true)

## Software
The code is written on platformio. 
### Required Libraries:
	- electroniccats/MPU6050@^0.2.1
	- NTPClient=https://github.com/arduino-libraries/NTPClient.git
	- ayushsharma82/AsyncElegantOTA @ ^2.2.5
	- khoih-prog/ESPAsync_WiFiManager@^1.6.0
	- bblanchon/ArduinoJson@^6.17.3
	- marvinroger/AsyncMqttClient@^0.8.2
	- makuna/RTC@^2.3.5

## Notes:
- Web Page: Use %% instead of % in css styles. Otherwise placeholder template fails in asyncweb. 
or you can use #define TEMPLATE_PLACEHOLDER '~' in build parameters and can use \~PLACEHOLDER\~ instead of %PLACEHOLDER%
- Openlog:
	1. Uppercase / Lowercase letters both work, but prefer lowercase filenames.
	2. When creating new file with append, it still creates LOG0000xx.TXT with 0 bytes. So delete this with rm first.
	3. For command mode send 3 x escape (26) and put a delay later. Delay is required after every command.
	4. append ends command mode.
	5. TXO creates problems during Serial firmware upload, so only used RXO on Serial1 on D4 for Openlog.
