#include <Wire.h>
#include <Arduino.h>
#define I2C_ARDUINO_ADR 4
long encoderLeft;
long encoderRight;

void setup()
{
    Wire.begin();
    Serial.begin(9600);
    Serial.println("Running");
}

void SetArduinoSpeed (int leftSpeed, int rightSpeed)
{
    Wire.beginTransmission(I2C_ARDUINO_ADR);
    uint8_t leftLow = leftSpeed & 0xff;
    uint8_t leftHigh = (leftSpeed >> 8);
    Wire.write(leftLow);
    Wire.write(leftHigh);
    uint8_t rightLow = rightSpeed & 0xff;
    uint8_t rightHigh = (rightSpeed >> 8);
    Wire.write(rightLow);
    Wire.write(rightHigh);
    Wire.endTransmission();
    Serial.println("Arduino speeds set");
}

void servoOutput(int servoAngle)
{
    uint8_t servoLow = servoAngle & 0xff;
    uint8_t servoHigh = (servoAngle >> 8);
    Wire.write(servoLow);
    Wire.write(servoHigh);
}

void MoveSteering()
{
    int maxAngle = 90;
    int minAngle = 0;
    float steeringAngle;
    servoOutput(0);
    for (steeringAngle = minAngle; steeringAngle <= maxAngle; steeringAngle += 1)
    { // goes from minAngle to maxAngle (degrees)
        // in steps of 1 degree
        servoOutput(steeringAngle); // tell servo to go to position in variable 'steeringAngle'
        delay(15);                    // waits 15ms for the servo to reach the position
        Serial.print("Turning\n");
    }
    for (steeringAngle = maxAngle; steeringAngle >= minAngle; steeringAngle -= 1)
    {                                 // goes from maxAngle to minAngle (degrees)
        servoOutput(steeringAngle); // tell servo to go to position in variable 'steeringAngle'
        delay(15);                    // waits 15 ms for the servo to reach the position
        Serial.print("Turning Back\n");
    }
    delay(20);
}

// void ReadEncoders()
// {
//     Wire.requestFrom(I2C_ARDUINO_ADR, 4);

//     while (Wire.available())
//     {
//         encoderLeft = Wire.read();
//         encoderRight = Wire.read();
//     }
// }

void loop()
{
    // ReadEncoders();
    // Serial.print("Left: ");
    // Serial.println(encoderLeft);
    // Serial.print("Right: ");
    // Serial.println(encoderRight);

    SetArduinoSpeed(255, 255);
    delay(1000);
    
}