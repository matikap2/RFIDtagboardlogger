#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include "stdint.h"
#include <LiquidCrystal_I2C.h>

// DATABASE & WIFI CONFIG
#define FIREBASE_HOST "XXX.firebaseio.com"
#define FIREBASE_AUTH "XXX"
#define WIFI_SSID "XXX"
#define WIFI_PASSWORD "XXX"

//UART1 TO STM32F4
//GPIO14 (D5) - RX
//GPIO12 (D6) - TX
SoftwareSerial UART(14, 12, false, 256);

//LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

//VARIABLES TO SEND
String plytka_UID;
String legitka_UID;
String czas;
uint8_t temp;
char buf[50];

void setup() {
  lcd.begin(16,2);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(3, 0);
  lcd.print("RFID BOARD");
  lcd.setCursor(5, 1);
  lcd.print("LOGGER");
  
  Serial.begin(9600);
  
  UART.begin(9600);
  Serial.println("Software serial started");
  delay(3000);
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("Connecting to");
  lcd.setCursor(6, 1);
  lcd.print("WiFi");
  // connect to wifi.
  WiFi.disconnect();
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("Connected:");
  lcd.setCursor(0, 1);
  lcd.print(WiFi.localIP());
  
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  delay(3000);
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  //Firebase.begin(FIREBASE_HOST);
}

void loop() {
  legitka_UID = "";
  plytka_UID = "";
  czas = "0";
  /*********************************************************************/
  UART.write("1");              //State machine on STM32, read legitka_UID
  Serial.println("Waiting for ELS");
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Show me your ELS");
  lcd.setCursor(3, 1);
  lcd.print("Please :D");
 
  while(!UART.available()){delay(1000);}
  delay(1000);
  while(UART.available()>0){
    temp = UART.read();                //Wait fot legitka_UID (convert from ascii code to hex)
    sprintf(buf,"%c", temp);
    legitka_UID += buf;              
  }
  Serial.println(legitka_UID);
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("Your ELS UID");
  lcd.setCursor(3, 1);
  lcd.print(legitka_UID);
  
  Serial.println("ELS is ready");
  delay(3000);
  /*********************************************************************/
  UART.write("2");              //State machine on STM32, read plytka_UID
  Serial.println("Waiting for plytka");
   
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("Show me board");
  lcd.setCursor(3, 1);
  lcd.print("Please :D");
  
  while(!UART.available()){delay(1000);}
  delay(1000);
  while(UART.available()>0){
    temp = UART.read();                 //Wait fot plytka_UID (convert from ascii code to hex)
    sprintf(buf,"%c", temp);
    plytka_UID += buf;  
  }
  Serial.println(plytka_UID);
    
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("Your board UID");
  lcd.setCursor(3, 1);
  lcd.print(plytka_UID);
  
  Serial.println("Plytka is ready");
  delay(3000);
  /*********************************************************************/
  UART.write("3");               //State machine on STM32, read czas
  Serial.println("Waiting for czas");
   
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Waiting for time");
  
  while(!UART.available()){delay(1000);}
  delay(1000);
  while(UART.available()>0){
    temp = UART.read();                 //Wait fot czas
    sprintf(buf,"%c", temp);
    czas += buf;   
  }
  Serial.println(czas);
    
  lcd.clear();
  lcd.setCursor(6, 0);
  lcd.print("Time:");
  lcd.setCursor(0, 1);
  lcd.print(czas);
  
  Serial.println("Czas is ready");
  delay(3000);
/*********************************************************************/
  UART.write("4");              //RESET state machine on STM32
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("Thank you!");

  
  delay(2000);
  Serial.println("Let's roll!");
  
    //Make JsonObject
     StaticJsonBuffer<200> jsonBuffer;
     JsonObject& root = jsonBuffer.createObject();
     root["studentId"] = legitka_UID;
     root["boardId"] = plytka_UID;
     root["timestamp"] = czas;
        
  // Send JsonObject of legitka_UID and plytka_UID to FIREBASE
  String name = Firebase.push("Data", root);
  // handle error
  if (Firebase.failed()) {
      lcd.setCursor(0, 1);
      lcd.print("ERR! Reset...");
      Serial.print("pushing /logs failed:");
      delay(2000);
      ESP.restart();
      return;
  }
  lcd.setCursor(3, 1);
  lcd.print("Log pushed!");
  Serial.print("pushed: /logs/");
  Serial.println(name);
  delay(2000);
}
