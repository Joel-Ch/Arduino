#include <Wire.h> // include Wire library
#include <Arduino.h>
String message;
void setup()
{
    Wire.begin(0x08);             // join i2c bus with address 8
    Wire.onReceive(receiveEvent); // create a receive event
    Serial.begin(9600);           // start serial to visualise data
                                  // received via serial monitor in the IDE
}
void loop()
{
}
void receiveEvent(int bytes)
{
    message = "";
    while (Wire.available())
    {                         // loop whilst bus is busy
        char c = Wire.read(); // receive data byte by byte
        message += c;         // form complete string
    }
    Serial.println(message); // write string to serial monitor
    delay(500);
    Serial.print("Number of bytes used:");
    Serial.println(bytes);
}