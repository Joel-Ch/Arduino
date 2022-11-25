#include <Arduino.h>
#include "ArduinoComms.h"
#include "ReadSensors.h"
#include <MPU6500_WE.h>

#define MPU6500_ADDR 0x68
#define TRIGGER_PIN 32
#define ECHO_PIN 33

// NOTE: The MPU6500 doesn't have a compass, so they only way to find the absolute angle it to do some weird maths involving integration of the angle moved with respect to gravity or something.
// That would be fine

// Global variables!!!!
int rightEncoderPos, leftEncoderPos, leftSpeed, rightSpeed, servoAngle = 90, distance;

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
    HCSR04.begin(TRIGGER_PIN, ECHO_PIN);
    Wire.begin();
    Serial.begin(9600);     // Starts the serial communication
    pinMode(TRIGGER_PIN, OUTPUT); // Sets the trigPin as an Output
    pinMode(ECHO_PIN, INPUT);  // Sets the echoPin as an Input
    if (!myMPU6500.init())
    {
        Serial.println("MPU6500 does not respond");
    }
    else
    {
        Serial.println("MPU6500 is connected");
    }
    CalibrateGyro();
}

void loop()
{
    rightSpeed = leftSpeed = 255;
    SetArduino(rightSpeed, leftSpeed, servoAngle);//drive forward
    delay(1000);// for 1 second
    rightSpeed = leftSpeed = 0;
    SetArduino(rightSpeed, leftSpeed, servoAngle);//then stop

    // turn 180 degrees
    SetArduino(150, 200, 110);
    delay(1000);
    SetArduino(-200, -150, 70);
    delay(1000);
    SetArduino(150, 200, 110);
    delay(1000);
    SetArduino(-200, -150, 70);
    delay(1000);
    SetArduino(0, 0, 90);

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
    SetArduino(150, 200, 110);
    delay(2000);
    SetArduino(0, 0, 90);
    // This is the only way to do it unless you have an MPU 9500 or a magnetometer

    // THEN:
    EndingA();

    // OR:
    // EndingB();

    delay(10000);

    return;
}