#line 1 "C:\\Users\\poely\\Documents\\Code\\Arduino\\Week3\\FinalEndOfWeekChallenge\\FinalEndOfWeekChallenge.ino"
#include <Arduino.h>
#include "ArduinoComms.h"
#include <MPU6050_tockn.h>

MPU6050 mpu6050(Wire);

#define TRIGGER_PIN 32
#define ECHO_PIN 33

// NOTE: The MPU6500 doesn't have a compass, so they only way to find the absolute angle it to do some weird maths involving integration of the angle moved with respect to gravity or something.
// That would be fine

// Global variables!!!!
int rightEncoderPos, leftEncoderPos, leftSpeed, rightSpeed, servoAngle = 90, distance, angleOffset, angle;

float CalculateDistance()
{
    digitalWrite(TRIGGER_PIN, LOW);
    delayMicroseconds(2);
    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(TRIGGER_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIGGER_PIN, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    long duration = pulseIn(ECHO_PIN, HIGH);
    // Calculate the distance
    float distance = duration * 0.034 / 2;
    Serial.print(distance);
    delay(500);

    return distance;
}

void EndingA()
{
    Serial.print("ending A");
    rightSpeed = leftSpeed = -100;
    SetArduino(rightSpeed, leftSpeed, servoAngle); // drive backward
    distance = 200;
    while (distance >= 45)               // wait until 10cm away
    {
        Serial.print("waiting for an object to be close ");
        delay(100);
        distance = CalculateDistance();
        Serial.println(distance);
    }
    rightSpeed = leftSpeed = 0;
    SetArduino(rightSpeed, leftSpeed, servoAngle); // then stop
    Serial.print("finishedA!");
    return;
}

void EndingB()
{
    Serial.print("ending B");

    int EncoderCheck = 1;
    while (EncoderCheck)//check encoders were read correctly
    {
        EncoderCheck = ReadEncoders(&rightEncoderPos, &leftEncoderPos);
    }
    int leftEncoderInit = rightEncoderPos;
    int rightEncoderInit = leftEncoderPos;
    Serial.print("right Encoder:");
    Serial.println(rightEncoderPos);
    Serial.println("left Encoder:");
    Serial.println(leftEncoderPos);

    rightSpeed = leftSpeed = -100;//drive backwards
    SetArduino(rightSpeed, leftSpeed, servoAngle);

    while (1)
    {
        EncoderCheck = 1;
        while (EncoderCheck)
        {
            EncoderCheck = ReadEncoders(&rightEncoderPos, &leftEncoderPos);
        }
        Serial.print("Left: ");
        Serial.println(leftEncoderPos);
        Serial.print("Right: ");
        Serial.println(rightEncoderPos);

        // please check this!
        float DIAMETER = 5;
        // maths:
        // 16 counts per rotation
        // diameter = x (cm)
        // circumference = pi*x = c (cm)
        // 30/c = total rotations = t
        // 16*t = total counts
        // overall:
        int totalCounts = (16 * (10 / (3.14159265 * (DIAMETER))));//find total number of counts 30cm is

        if (rightEncoderInit - rightEncoderPos >= totalCounts && leftEncoderInit - leftEncoderPos >= totalCounts)
        {
            rightSpeed = leftSpeed = 0; // then stop moving
            SetArduino(rightSpeed, leftSpeed, servoAngle);
            Serial.print("finishedB!");
            return;
        }
        delay(500);
    }

}

void setup()
{
    Wire.begin();
    Serial.begin(9600);     // Starts the serial communication
    pinMode(TRIGGER_PIN, OUTPUT); // Sets the trigPin as an Output
    pinMode(ECHO_PIN, INPUT);  // Sets the echoPin as an Input
    mpu6050.begin();
    mpu6050.calcGyroOffsets(true);
}

void loop()
{
    rightSpeed = leftSpeed = 255;
    SetArduino(rightSpeed, leftSpeed, servoAngle);//drive forward
    delay(1000);// for 1 second
    rightSpeed = leftSpeed = 0;
    SetArduino(rightSpeed, leftSpeed, servoAngle);//then stop

    // turn 180 degrees
    SetArduino(200,255,110);
    mpu6050.update();
    while(angle >= -180)
    {
      mpu6050.update();
      angle = mpu6050.getAngleZ();
      Serial.println(angle);
    }
    SetArduino(0,0,90);
    // finish turning
    rightSpeed = leftSpeed = -100;
    SetArduino(rightSpeed, leftSpeed, servoAngle);//drive backward
    distance = 200;
    while (distance >= 45)                        // wait until 10cm away
    {
        Serial.print("waiting for an object to be close ");
        delay(100);
        distance = CalculateDistance();
        Serial.println(distance);
    }
    rightSpeed = leftSpeed = 0;
    SetArduino(rightSpeed, leftSpeed, servoAngle); // then stop

    delay(5000);

    // Turn 90 degrees clockwise
    SetArduino(200,255,110);
    mpu6050.update();
    while(angle >= -270)
    {
      mpu6050.update();
      angle = mpu6050.getAngleZ();
      Serial.println(angle);
    }
    SetArduino(0,0,90);
    // Stop Turning

    // THEN:
    EndingA();

    // OR:
    // EndingB();

    delay(10000);

    return;
}
