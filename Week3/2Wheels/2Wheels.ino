#include <Arduino.h>
#include "ArduinoComms.h"
#include <math.h>
#include <Wire.h>

const int MPU_addr = 0x68;
int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;

int minVal = 265;
int maxVal = 402;

double x;
double y;
double z;

int speed;

void setup()
{
    Wire.begin();
    Wire.beginTransmission(MPU_addr);
    Wire.write(0x6B);
    Wire.write(0);
    Wire.endTransmission(true);
    Serial.begin(9600);
}

void loop()
{
    Wire.beginTransmission(MPU_addr);
    Wire.write(0x3B);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU_addr, 14, true);
    AcX = Wire.read() << 8 | Wire.read();
    AcY = Wire.read() << 8 | Wire.read();
    AcZ = Wire.read() << 8 | Wire.read();
    int xAng = map(AcX, minVal, maxVal, -90, 90);
    int yAng = map(AcY, minVal, maxVal, -90, 90);
    int zAng = map(AcZ, minVal, maxVal, -90, 90);

    x = RAD_TO_DEG * (atan2(-yAng, -zAng) + PI);
    y = RAD_TO_DEG * (atan2(-xAng, -zAng) + PI);
    z = RAD_TO_DEG * (atan2(-yAng, -xAng) + PI);

    Serial.print("AngleX= ");
    Serial.println(x);
    Serial.print("AngleY= ");
    Serial.println(y);
    Serial.print("AngleZ= ");
    Serial.println(z);

    Serial.println("-----------------------------------------");

    int xint = (int)x;
    Serial.print("  ");
    Serial.print(xint);

    if (xint < 82)
    {
        speed = -pow(5, (82-xint));
        Serial.print("Forwards");
    }

    else if (xint > 82)
    {
        speed = (pow(5, (xint-82)));
        Serial.print("Backwards");
    }
    SetArduino(speed, speed, 90);

    delay(400);
}