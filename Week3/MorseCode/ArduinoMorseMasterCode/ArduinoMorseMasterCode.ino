#include <Wire.h> // include Wire library
#include <arduino.h>
void setup()
{
    Wire.begin(); // join i2c bus (address optional for master)
    Serial.begin(9600);
}
void loop()
{
    Wire.beginTransmission(0x08); // transmit to slave device address 8
    Wire.write("...././.-../.-../---"); // send five bytes, one for each character
    Wire.endTransmission();       // end transmission
    Serial.print("sent message!");
    delay(1000);
}