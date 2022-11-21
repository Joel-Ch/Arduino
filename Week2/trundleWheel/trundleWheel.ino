/* Digital Trundle Wheel
   takes input from a 6 cycle (4 positions per cycle) quadrature encoder
*/

// include encoder library
#include <Encoder.h>
#include <Arduino.h>

// change these two numbers (if needed) to enable pins connected to encoder
Encoder myEnc(2, 11); //enable pins with interrupt capability
Encoder myEnc2(3, 12); //enable pins with interrupt capability

long oldPosition  = -999, oldPosition2 = -999;
float distance, distance2;

void setup() 
{
  Serial.begin(9600); 
  Serial.println("Digital Trundle Wheel Challenge: ");
}

void loop() 
{
  long newPosition = myEnc.read();
  long newPosition2 = myEnc2.read();
  
  // check if encoder has moved
  if (newPosition != oldPosition) 
  {
    oldPosition = newPosition;
    
  // edit the code below to calculate the distance moved, +1 increment = (diameter*pi)/encoder count per revolution
    distance = newPosition * 1.0;
  // ***
    
    // output distance to the serial monitor                    
    Serial.print("Distance(m): ");
    Serial.println(distance);
  }

  if (newPosition2 != oldPosition2)
  {
    oldPosition2 = newPosition2;

    // edit the code below to calculate the distance moved, +1 increment = (diameter*pi)/encoder count per revolution
    distance2 = newPosition2 * 1.0;
    // ***

    // output distance to the serial monitor
    Serial.print("Distance2(m): ");
    Serial.println(distance2);
  }
}
