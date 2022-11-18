#include <Encoder.h>
#include <Servo.h> //include the servo library
#include <Arduino.h>
#define servoPin 4
Servo steering;
#define enA 5
#define enB 6

#define INa A0
#define INb A1
#define INc A2
#define INd A3

Encoder encoderLeft(2, 11);
Encoder encoderRight(3, 12);
// Change these for whichever pins is necessary

void motors(int leftSpeed, int rightSpeed)
{
    // set individual motor speed
    // direction is set separately

    analogWrite(enA, leftSpeed);
    analogWrite(enB, rightSpeed);
}

void setup()
{
    steering.attach(servoPin); // attach our servo object to pin D4
    // the Servo library takes care of defining the PinMode declaration (libraries/Servo/src/avr/Servo.cpp line 240)

    // configure the motor control pins as outputs
    pinMode(INa, OUTPUT);
    pinMode(INb, OUTPUT);
    pinMode(INc, OUTPUT);
    pinMode(INd, OUTPUT);
    pinMode(enA, OUTPUT);
    pinMode(enB, OUTPUT);

    // initialise serial communication
    Serial.begin(9600);
    Serial.println("Arduino Nano is Running"); // sanity check

    motors(255, 255);

    steering.write(38); // set the servo to be central
}

void loop()
{

    float DIAMETER = 0.2;
    // maths:
    // 16 counts per rotation
    // diameter = x (m)
    // circumference = pi*x = c (m)
    // 10/c = total rotations = t
    // 16*t = total counts
    // overall:

    // total number of counts in 10m
    int totalCounts = (16 * (10 / (3.14159265 * (DIAMETER))));

    // encoder input
    long countsLeft = encoderLeft.read();
    long countsRight = encoderRight.read();

    while (countsLeft < totalCounts && countsRight < totalCounts)
    {
        digitalWrite(INa, LOW);
        digitalWrite(INb, HIGH);
        digitalWrite(INc, HIGH);
        digitalWrite(INd, LOW);
        Serial.println(countsLeft);
        Serial.println(countsRight);


        if (countsLeft == countsRight)
        {
            motors(255, 255);
            steering.write(38);
        }
        
        else if (countsLeft < countsRight)
        {
            motors(255, 200);
            steering.write(28);
        }

        else if (countsRight < countsLeft)
        {
            motors(200, 255);
            steering.write(48);
        }
    }

    digitalWrite(INa, LOW);
    digitalWrite(INb, LOW);
    digitalWrite(INc, LOW);
    digitalWrite(INd, LOW);
    delay(10000);

    return;
}
