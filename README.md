# RFIDtagboardlogger

Final project to Microprocessor's System II and Design Laboratory course at Electronics faculty.
Simple logger of developement boards rented by students on laboratories.

## Used modules

Project uses STM32F401RE Nucleo board, NodeMCU v0.9, LCD HD44780 16x2 with I2C converter and RC522 RFID module.
Moreover, it uses Firebase.

## Setting up
### Hardware
Connect everyting following [schematic](https://i.imgur.com/4nxXy50.png)

### Software
1. Program Nucleo board using source files. During developement we used Keil software.
2. Download necessary Arduino libs and install them:
[Firebase-Arduino](https://github.com/Chriton/Firebase-Arduino), [ArduinoJson](https://github.com/bblanchon/ArduinoJson), [LiquidCrystal_I2C](https://github.com/johnrickman/LiquidCrystal_I2C), [ESP8266](http://arduino.esp8266.com/stable/package_esp8266com_index.json)
3. Configure NodeMCU by changing following lines of code:
```
#define FIREBASE_HOST "XXX.firebaseio.com"
#define FIREBASE_AUTH "XXX"
#define WIFI_SSID "XXX"
#define WIFI_PASSWORD "XXX"
```
After that, program using Arduino IDE.

## Functions
- Reading UID of RFID cards using RC522 module
- Debug through UART
- RTC
- Connection to Firebase through Wi-Fi

## Autors
- Mateusz Kapala (https://github.com/matikap2/)
- Jan Gąsienica-Józkowy (https://github.com/Andropogon/)

## License
Fell free to use.

