#include <BluetoothSerial.h>
#include "ArduinoComms.h"
#include <Arduino.h>

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#define TRIGGER_PIN 32
#define ECHO_PIN 33

BluetoothSerial SerialBT;
int rightSpeed = 0, leftSpeed = 0, servoAngle = 90;
char input;

void setup()
{
    Serial.begin(9600);
    SerialBT.begin("ESP32"); // Bluetooth device name
    Wire.begin();
    Serial.println("The device started, now you can pair it with bluetooth!");
}

void loop()
{
    if (SerialBT.available())
    {
        input = SerialBT.read();
        Serial.print("INPUT: ");
        Serial.println(input);
        Serial.println("---");
        if (input == '1')
        {
            rightSpeed = leftSpeed = 0;
        }
        else if (input == '2')
        {
            rightSpeed = leftSpeed = 255;
        }
        else if (input == '3')
        {
            rightSpeed = leftSpeed = -255;
        }
        else if (input == '4')
        {
            servoAngle = 110;
        }
        else if (input == '5')
        {
            servoAngle = 70;
        }
        else if (input == '6')
        {
            servoAngle = 90;
        }
        else
        {
            Serial.println("please enter one of straight, left, right, backwards, forwards or stop");
        }
    }
    delay(1000);
    
}