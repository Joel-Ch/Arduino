#include <Wire.h>
#include "ArduinoComms.h"
#include <Arduino.h>
#include <SPI.h>
#include <TFT_eSPI.h> // Hardware-specific library

#define TRIGGER_PIN 32
#define ECHO_PIN 33
#define APin 34
#define BPin 35
#define CPin 4
#define DPin 0
#define EPin 2
#define FPin 15

#define kp 35
#define ki 1
#define kd 1
#define kw 0.4

TFT_eSPI tft = TFT_eSPI();  // Invoke custom library

#define baseSpeed 150

float inA, inB, inC, inD, inE, inF, linePos,error, oldError,sumError, u, tightCorner, turnTracker = 0;

void setup()
{
    Wire.begin();
    Serial.begin(9600);     // Starts the serial communication
    pinMode(TRIGGER_PIN, OUTPUT); // Sets the trigPin as an Output
    pinMode(ECHO_PIN, INPUT);  // Sets the echoPin as an Input
    pinMode(APin, INPUT);
    pinMode(BPin, INPUT);
    pinMode(CPin, INPUT);
    pinMode(DPin, INPUT);
    pinMode(EPin, INPUT);
    pinMode(FPin, INPUT);
    tft.init();
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
}

void loop ()
{
    //tft.fillRect(55,0,50,220,TFT_BLACK);
    tft.setCursor(0,0,2);
    // read inputs
    inA = (analogRead(APin) - 4095) * (9 - 1) / (1000 - 4095) + 1;
    inB = (analogRead(BPin) - 4095) * (9 - 1) / (1700 - 4095) + 1;
    inC = (analogRead(CPin) - 4095) * (9 - 1) / (2000 - 4095) + 1;
    inD = (analogRead(DPin) - 4095) * (9 - 1) / (2100 - 4095) + 1;
    inE = (analogRead(EPin) - 4095) * (9 - 1) / (2090 - 4095) + 1;
    inF = (analogRead(FPin) - 4095) * (9 - 1) / (1700 - 4095) + 1;

    //inA = map(analogRead(APin),4095,1946,1,9);// max = 2100
    //inB = map(analogRead(BPin),4095,3677,1,9);// max = 270
    //inC = map(analogRead(CPin),4095,3645,1,9); // max = 450
    //inD = 4+ 0* map(analogRead(DPin),4095,4094,1,9); // max = 1? - needs fixing
    //inE = map(analogRead(EPin),4095,3755,1,9); // max = 340 - needs fixing
    //inF = map(analogRead(FPin),4095,2318,1,9);// max = 1720
    Serial.print("Inputs: ");
    tft.print("A:        ");
    tft.println(inA);
    tft.print("B:        ");
    tft.println(inB);
    tft.print("C:        ");
    tft.println(inC);
    tft.print("D:        ");
    tft.println(inD);
    tft.print("E:        ");
    tft.println(inE);
    tft.print("F:        ");
    tft.println(inF);
    Serial.print(inA);
    Serial.print(" ");
    Serial.print(inB);
    Serial.print(" ");
    Serial.print(inC);
    Serial.print(" ");
    Serial.print(inD);
    Serial.print(" ");
    Serial.print(inE);
    Serial.print(" ");
    Serial.println(inF);

    // do processing here

    if(inA == 1 && inB == 1 && inC == 1 && inD == 1 && inE == 1 && inF == 1)
    {
      SetArduino(-100,-100,90+turnTracker);
      delay(200);
    }
    else
    {

      linePos = (inA * 1 + inB * 2 + inC * 3 + inD * 4 + inE * 5 + inF * 6) / (inA + inB + inC + inD + inE + inF);
      Serial.print("linePos: ");
      Serial.print(linePos);
      tft.print("LinePos:  ");
      tft.println(linePos);
      // find error for finding u
      error = 3.5 - linePos;
      Serial.print(" error: ");
      Serial.print(error);
      tft.print("Error:    ");
      tft.println(error);

      if (error >= -0.4 && error <= 0.4)
      {
        tightCorner = 1;
      }
      else if (error >= -0.6 && error <= 0.6)
      {
        tightCorner = 0.95;
      }
      else if (error >= -0.8 && error <= 0.8)
      {
        tightCorner = 0.9;
      }
      else if (error >= -1 && error <= 1)
      {
        tightCorner = 0.85;
      }
      else if (error >= -1.2 && error <= 1.2)
      {
        tightCorner = 0.8;
      }
      Serial.print(" corner: ");
      Serial.print(tightCorner);
      tft.print("Corner:   ");
      tft.println(tightCorner);

      if (error >= 0)
      {
        turnTracker = -20;
      }
      else if (error < 0)
      {
        turnTracker = 20;
      }

      // Sum 1-n of e
      sumError += error;

      // PID equation
      u = kp * error + ki*sumError + kd*(error-oldError);

      // en-1
      oldError = error;

      // now we have u
      Serial.print("U: ");
      Serial.println(u);
      tft.print("U:        ");
      tft.println(u);


      // now send instructions to the wheels
      SetArduino((baseSpeed - (kw * u))*tightCorner, (baseSpeed + (kw * u))*tightCorner, 90 + u);
      Serial.print("RLA: ");
      Serial.print((baseSpeed - (kw * u)));
      Serial.print(" ");
      Serial.print((baseSpeed + (kw * u)));
      Serial.print(" ");
      Serial.print(90+u);
      tft.print("R:        ");
      tft.println((baseSpeed - (kw * u)));
      tft.print("L:        ");
      tft.println((baseSpeed + (kw * u)));
      tft.print("A:        ");
      tft.print(90+u);
    }
}