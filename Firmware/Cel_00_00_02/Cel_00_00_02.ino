#include <WiFi101.h>
#include <Wire.h>
#include <Adafruit_SHT31.h>

#include "SensorSettings.h"
#include "NetworkSettings.h"

unsigned long cycleCounter = 0;
int resetPin = 7;

unsigned long systemStarting = millis();
const unsigned long SYSTEMRESTARTINTERVAL = 6048000000; //one week //86400000; //one day 604800000; //one week

String receivedLine = "";
String receivedPost = "";

void setup() 
{
  //Serial.begin(9600);
  receivedLine.reserve(LINELENGTH);
  receivedPost.reserve(LINELENGTH);
  getSearchStr.reserve(getSearchStr.length());
  postSearchStr.reserve(postSearchStr.length());
  httpSearchStr.reserve(httpSearchStr.length());
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  pinMode(resetPin, OUTPUT);
  digitalWrite(resetPin, HIGH);
  blinkLittleLedBlink(100, 10);

  if (WiFi.status() == WL_NO_SHIELD)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    while (true);
  }

  WiFi.config(ipAddress_);

  for(int d = 0; d < MAXIMUMNUMBEROFADCS; d++)
  {
    if(sensorName_[adcSensor_[d]] == "Dry Contact")
    {
      pinMode(adcPinout_[d], INPUT_PULLUP);
    }
    else
    {
      pinMode(adcPinout_[d], INPUT);
    }
    if(actuationEnable_[d])
    {
      pinMode(actuationPinout_[d], OUTPUT);
      digitalWrite(actuationPinout_[d], LOW);
    }
  }
  sht31.begin(0x44);
  sht31.heater(false);

  // attempt to connect to WiFi network:
  connectWifi();
  WiFi.macAddress(mac);

  server.begin();


  blinkLittleLedBlink(500, 5);
  
}

void loop() 
{
  alarm_watcher();
  isItConnectWifi(); //restarts if not connected
  html_interface();
  shouldIRestart();
}


void connectWifi()
{
  while (WiFi.status() != WL_CONNECTED) 
  {
    alarm_watcher();
    status = WiFi.begin(SSIDName, SSIDPass);
    // wait 10 seconds for connection:
    blinkLittleLedBlink(250, 20);
  }
  delay(1000);
}

void isItConnectWifi()
{
  if(WiFi.status() != WL_CONNECTED)
  {
    digitalWrite(resetPin, LOW); //hard resets Arduino
  }
}


void blinkLittleLedBlink(int deltaT, int rounds)
{
  for(int k = 0; k < rounds; k++)
  {
    delay(deltaT);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(deltaT);
    digitalWrite(LED_BUILTIN, LOW);
  }
}

boolean shouldIRestart()
{
  if(millis() - systemStarting >= SYSTEMRESTARTINTERVAL)
  {
    digitalWrite(resetPin, LOW); //hard resets Arduino
    return true;
  }
  return false;
}
