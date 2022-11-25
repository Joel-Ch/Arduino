#include <Arduino.h>
#include <HCSR04.h>

// #define triggerPin 35
// #define echoPin 34

// void setup()
// {
//     Serial.begin(9600);
//     HCSR04.begin(triggerPin, echoPin);
// }

// void loop()
// {
//     double *distances = HCSR04.measureDistanceCm();

//     Serial.print("1: ");
//     Serial.print(*distances);
//     Serial.println(" cm");

//     Serial.println("---");
//     delay(250);
// }

const int trigPin = 32;
const int echoPin = 33;
long duration;
float distanceCm;
// if you don't know your display address, run an I2C scanner sketch
// define sound speed in cm/uS
#define SOUND_SPEED 0.034

void setup()
{
    Serial.begin(115200);     // Starts the serial communication
    pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
    pinMode(echoPin, INPUT);  // Sets the echoPin as an Input
}

void loop()
{

    // Clears the trigPin
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration = pulseIn(echoPin, HIGH);
    // Calculate the distance
    distanceCm = duration * SOUND_SPEED / 2;

    // Prints the distance in the Serial Monitor
    Serial.print("Distance (cm): ");
    Serial.println(distanceCm);
    delay(1000);
}