#include <Wire.h>
#include "ArduinoComms.h"
#include <Arduino.h>

#define TRIGGER_PIN 32
#define ECHO_PIN 33
#define APin 34
#define BPin 35
#define CPin 4
#define DPin 0
#define EPin 2
#define FPin 15

#define kp 40
#define ki 1
#define kd 1
#define kw 1/10

#define baseSpeed 100

float inA, inB, inC, inD, inE, inF, linePos,error, oldError, u, tightCorner;

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
}


void loop ()
{
    delay(500);  
    // read inputs
    inA = (analogRead(APin) - 4095) * (9 - 1) / (1946 - 4095) + 1;
    inB = (analogRead(BPin) - 4095) * (9 - 1) / (3677 - 4095) + 1;
    inC = (analogRead(CPin) - 4095) * (9 - 1) / (3645 - 4095) + 1;
    inD = 1+ 0*((analogRead(DPin) - 4095) * (9 - 1) / (4094 - 4095) + 1);
    inE = (analogRead(EPin) - 4095) * (9 - 1) / (3755 - 4095) + 1;
    inF = (analogRead(FPin) - 4095) * (9 - 1) / (2318 - 4095) + 1;

    //inA = map(analogRead(APin),4095,1946,1,9);// max = 2100
    //inB = map(analogRead(BPin),4095,3677,1,9);// max = 270
    //inC = map(analogRead(CPin),4095,3645,1,9); // max = 450
    //inD = 4+ 0* map(analogRead(DPin),4095,4094,1,9); // max = 1? - needs fixing
    //inE = map(analogRead(EPin),4095,3755,1,9); // max = 340 - needs fixing
    //inF = map(analogRead(FPin),4095,2318,1,9);// max = 1720
    Serial.print("Inputs: ");
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
      SetArduino(-100,-100,90);
    }
    else
    {

      linePos = (inA * 1 + inB * 2 + inC * 3 + inD * 4 + inE * 5 + inF * 6) / (inA + inB + inC + inD + inE + inF);
      Serial.print("linePos: ");
      Serial.print(linePos);    
      // find error for finding u
      error = 3.5 - linePos;
      Serial.print("error: ");
      Serial.print(error);

      if (error <= 1 || error >= -1)
      {
          tightCorner = 1;
      }
      else if (error <= 2 || error >= -2)
      {
          tightCorner = 0.8;
      }
      else if (error <= 3 || error >= -3)
      {
          tightCorner = 0.7;
      }
      else if (error <= 4 || error >= -4)
      {
          tightCorner = 0.5;
      }


      // Sum 1-n of e
      //sumError += error;

      // PID equation
      u = kp * error; //+ ki*sumError;// + kd*(error-oldError);

      // en-1
      oldError = error;

      // now we have u
      Serial.print("U: ");
      Serial.println(u);


      // now send instructions to the wheels
      SetArduino((baseSpeed + (kw * u))*tightCorner, (baseSpeed - (kw * u))*tightCorner, 90 + u);
      Serial.print("RLA: ");
      Serial.print((baseSpeed + (kw * u))*tightCorner);
      Serial.print(" ");
      Serial.print((baseSpeed - (kw * u))*tightCorner);
      Serial.print(" ");
      Serial.print(90+u);
    }
}