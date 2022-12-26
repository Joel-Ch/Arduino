#include <Wire.h>

#define TRIGGER_PIN 32
#define ECHO_PIN 33

#define I2C_ARDUINO_ADDR 4

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
    delay(500);

    return distance;
}


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
    Serial.println("Arduino is set to ");
    Serial.println(rightSpeed);
    Serial.println(leftSpeed);
    Serial.println(servoAngle);

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
    Serial.print("reading encoders");
}