#include <Wire.h> // include Wire library
#include <arduino.h>
#include "ConvertToText.h"
String message;
void setup()
{
    Wire.begin(); // join i2c bus (address optional for master)
    Serial.begin(9600);
}

void requestMessage1()
{
    message = "";
    Wire.beginTransmission(0x08); // transmit to device #8
    Wire.write("message1");       // sends six bytes
    Wire.endTransmission();       // stop transmitting
    Wire.requestFrom(0x08, 32);
    while (Wire.available())
    {                         // loop whilst bus is busy
        char c = Wire.read(); // receive data byte by byte
        if (c == '#')
            break;    // check for end of message
        message += c; // build complete message
    }
    return;
}

void requestMessage2()
{
    message = "";
    Wire.beginTransmission(0x08); // transmit to device #8
    Wire.write("message2");       // sends six bytes
    Wire.endTransmission();       // stop transmitting
    Wire.requestFrom(0x08, 32);
    while (Wire.available())
    {                         // loop whilst bus is busy
        char c = Wire.read(); // receive data byte by byte
        if (c == '#')
            break;    // check for end of message
        message += c; // build complete message
    }
    return;
}

void decodeData()
{
    int messageLength = message.length();
    char messageString[messageLength+1];
    message.toCharArray(messageString, messageLength + 1);

    Serial.println(messageString); // print morse code
    char output[200] = {0};
    convertToText(messageString, output); // run conversion function
    Serial.println(output);               // print output in text
    return;
}

void loop()
{
    requestMessage1();
    decodeData();
    delay(500);
    requestMessage2();
    decodeData();
    delay(500);
}