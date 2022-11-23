// #include <Wire.h> // include Wire library
// #include <Arduino.h>
// #include "ConvertToText.h"
// #include <string.h>
// String message;
// void setup()
// {
//     Wire.begin(0x08);             // join i2c bus with address 8
//     Wire.onReceive(receiveEvent); // create a receive event
//     Serial.begin(9600);           // start serial to visualise data
//                                   // received via serial monitor in the IDE
// }
// void loop()
// {

// }
// void receiveEvent(int bytes)
// {
//     Serial.print("Number of bytes recieved:");
//     Serial.println(bytes);
//     message = "";
//     while (Wire.available())
//     {                         // loop whilst bus is busy
//         char c = Wire.read(); // receive data byte by byte
//         message += c;         // form complete string
//     }

//     int messageLength = message.length();
//     char messageString[messageLength+1];
//     message.toCharArray(messageString, messageLength + 1);

//     Serial.println(messageString); // print morse code
    
//     delay(500);
    
//     char output[200] = {0};
//     convertToText(messageString, output); // run conversion function
//     Serial.println(output); // print output in text
// }