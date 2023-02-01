#include <Wire.h>

#define I2C_ARDUINO_ADDR 4


void SetArduino(int rightSpeed, int leftSpeed, int servoAngle)
{
    Wire.beginTransmission(I2C_ARDUINO_ADDR);
    uint8_t leftLow = leftSpeed & 0xff;
    uint8_t leftHigh = (leftSpeed >> 8);
    Wire.write(leftHigh);
    Wire.write(leftLow);
    uint8_t rightLow = rightSpeed & 0xff;
    uint8_t rightHigh = (rightSpeed >> 8);
    Wire.write(rightHigh);
    Wire.write(rightLow);
    uint8_t angleLow = servoAngle & 0xff;
    uint8_t angleHigh = (servoAngle >> 8);
    Wire.write(angleHigh);
    Wire.write(angleLow);
    Wire.endTransmission();
    //Serial.print("Arduino is set to: (RLA) ");
    //Serial.print(rightSpeed);
    //Serial.print(leftSpeed);
    //Serial.println(servoAngle);

    return;
}

void ReadEncoders(int *rightPos, int *leftPos)
{
    Wire.requestFrom(I2C_ARDUINO_ADDR, 2);
    while (Wire.available())
    {

        *rightPos = Wire.read();
        *leftPos = Wire.read(); 
    }
    //Serial.print("reading encoders");
}