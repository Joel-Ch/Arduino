#include <Wire.h>
#include "ArduinoComms.h"

void setup()
{
  Wire.begin();
  Serial.begin(9600);     // Starts the serial communication
}

void loop()
{
  SetArduino(0,0,90);
  delay(1000);
  SetArduino(0,0,80);
  delay(1000);
  SetArduino(0,0,100);
  delay(1000);
  SetArduino(0,0,70);
  delay(1000);
  SetArduino(0,0,110);
  delay(1000);
  SetArduino(0,0,60);
  delay(1000);
  SetArduino(0,0,120);
  delay(1000);
  SetArduino(0,0,50);
  delay(1000);
  SetArduino(0,0,130);
  delay(1000);
  SetArduino(0,0,40);
  delay(1000);
  SetArduino(0,0,140);
  delay(1000);
  SetArduino(0,0,30);
  delay(1000);
  SetArduino(0,0,150);
  delay(1000);
  SetArduino(0,0,20);
  delay(1000);
  SetArduino(0,0,160);
  delay(1000);
  SetArduino(0,0,10);
  delay(1000);
  SetArduino(0,0,170);
  delay(1000);
  SetArduino(0,0,0);
  delay(1000);
  SetArduino(0,0,180);
  delay(1000);
}