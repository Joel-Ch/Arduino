#include <Wire.h>
#include "ArduinoComms.h"
#include <MPU6050_tockn.h>
#include <Arduino.h>

#define TRIGGER_PIN 32
#define ECHO_PIN 33
#define APin 34
#define BPin 35
#define CPin 4
#define DPin 0
#define EPin 2
#define FPin 15
// distance between infrared sensors
#define INFDIST 1

unsigned int inA, inB, inC, inD, inE, inF, linePos, angle, rightSpeed, leftSpeed;

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
    // read inputs 

    inA = 4096 - analogRead(APin);// max = 2100
    inB = 4096 - analogRead(BPin);// max = 270
    inC = 4096 - analogRead(CPin); // max = 450
    inD = 4096 - analogRead(DPin); // max = 1?
    inE = 4096 - analogRead(EPin); // max = 340
    inF = 4096 - analogRead(FPin);// max = 1720
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

    linePos = (inA * 1 * INFDIST + inB * 2 * INFDIST + inC * 3 * INFDIST + inD * 4 * INFDIST + inE * 5 * INFDIST + inF * 6 * INFDIST) / (inA + inB + inC + inD + inE + inF);
    
    // now we have the position of the line 
    Serial.print("LinePos: ");
    Serial.println(linePos);

    // now send instructions to the wheels
    angle = map(linePos, 1, 6, 140, 40);
    rightSpeed = 0;//map(linePos, 1, 6, 120, 100);
    leftSpeed = 0;//map(linePos, 1, 6, 100, 120);
    SetArduino(rightSpeed, leftSpeed, angle);
    delay(500);
}