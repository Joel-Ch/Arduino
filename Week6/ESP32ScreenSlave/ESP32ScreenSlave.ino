#include <Wire.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include <string>

TFT_eSPI tft = TFT_eSPI();  // Invoke custom library

String colour;
int Colour;

void setup() {
  Wire.begin(5);                // join I2C bus with address #5
  Wire.onRequest(requestEvent); // register event
  Wire.onReceive(receiveEvent); // register event
  Serial.begin(9600);
  Serial.println("Running!");
  tft.init();
  tft.fillScreen(TFT_BLACK);
  colour="black";
  Serial.println("HI");
}

void loop() {
  if (colour == "black")
    Colour = TFT_BLACK;
  else if (colour== "red")
    Colour = TFT_RED;
  else if (colour == "green")
    Colour = TFT_GREEN;
  else if (colour == "blue")
    Colour = TFT_BLUE;
  else if (colour == "yellow")
    Colour = TFT_YELLOW;
  tft.fillScreen(Colour);
}

void receiveEvent(int howMany)
{
  int count=0;
  colour="";
  Serial.println("Recieve!");

  while(Wire.available())
  {
    char c = Wire.read();    // receive a byte as character
    //Create Int from the Byte Array
    colour+=c;
    count++;
  }
  Serial.println(colour);
}

void requestEvent(void)
{

}
