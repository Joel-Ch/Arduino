#include <Arduino.h>
#include <Wire.h>
#include <HCSR04.h>
#include <MPU6500_WE.h>

#define MPU6500_ADDR 0x68
#define TRIGGER_PIN 32
#define ECHO_PIN 33

MPU6500_WE myMPU6500 = MPU6500_WE(MPU6500_ADDR);

void CalibrateGyro()
{
    Serial.println("Position you MPU6500 flat and don't move it - calibrating...");
    delay(1000);
    myMPU6500.autoOffsets();
    myMPU6500.enableGyrDLPF();
    myMPU6500.setGyrDLPF(MPU6500_DLPF_6);
    myMPU6500.setSampleRateDivider(5);
    myMPU6500.setGyrRange(MPU6500_GYRO_RANGE_250);

    Serial.println("Done!");
}
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

float MeasureAngleX()
{
    xyzFloat gyr = myMPU6500.getGyrValues();

    Serial.println("Gyroscope data in degrees/s: ");
    Serial.print(gyr.x);

    return gyr.x;
}

float MeasureAngleY()
{
    xyzFloat gyr = myMPU6500.getGyrValues();

    Serial.println("Gyroscope data in degrees/s: ");
    Serial.print(gyr.y);

    return gyr.y;
}

float MeasureAngleZ()
{
    xyzFloat gyr = myMPU6500.getGyrValues();

    Serial.println("Gyroscope data in degrees/s: ");
    Serial.print(gyr.z);

    return gyr.z;
}