#include <Encoder.h>

// either change your wire connections to the two encoders or change the pin numbers in the code
// to change whether the count increments or decrements when turning in a particular direction
Encoder enc1(2, 11); // create an encoder object for encoder 1
Encoder enc2(3, 12); // create an encoder object for encoder 2
long oldPos_enc1 = -999;
long oldPos_enc2 = -999;

#define enA 5 // EnableA command line - should be a PWM pin
#define enB 6 // EnableB command line - should be a PWM pin

void setup()
{
    // set the encoder pins as inputs
    pinMode(enA, OUTPUT);
    pinMode(enB, OUTPUT);

    // initialise serial communication
    Serial.begin(9600);
    Serial.println("Arduino Nano is Running"); // sanity check
}

void loop()
{
    // update the two encoder values
    long enc1_count = enc1.read();
    long enc2_count = enc2.read();
    if (enc1_count != oldPos_enc1)
    {
        oldPos_enc1 = enc1_count;
    }
    if (enc2_count != oldPos_enc2)
    {
        oldPos_enc2 = enc2_count;
    }
    Serial.print(oldPos_enc1);
    Serial.print(" ");
    Serial.println(oldPos_enc2);
}