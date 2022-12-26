#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include "ArduinoComms.h"
#include <MPU6050_tockn.h>
#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

#define TRIGGER_PIN 32
#define ECHO_PIN 33

#define DIAMETER 5

MPU6050 mpu6050(Wire);

#define _cs 17   // goes to TFT CS
#define _dc 16   // goes to TFT DC
#define _mosi 23 // goes to TFT MOSI
#define _sclk 18 // goes to TFT SCK/CLK
#define _rst 5   // goes to TFT RESET
#define _miso    // Not connected
//       3.3V     // Goes to TFT LED
//       5v       // Goes to TFT Vcc
//       Gnd      // Goes to TFT Gnd

// MATHS:
// 1 rotation = 16 encoder counts = PI*Diameter
// hence 1 encoder count = PI*Diameter/16
// encoder*(16/PI) = 1 Diameter
// encoder*(16/(PI*Diameter(cm))) = Distance(cm)

// Replace the next variables with your SSID/Password combination
const char *ssid = "C11Chip";
const char *password = "00000000";

// Add your MQTT Broker IP address, example:
// const char* mqtt_server = "192.168.1.144";
const char *mqtt_server = "192.168.137.166";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];

float distance = 0, SeanDist = 0;

int rightSpeed = 0, leftSpeed = 0, servoAngle = 90, rightEncoder, leftEncoder;

int colour;

void setup()
{
    tft.init();
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);

    Serial.begin(115200);
    // default settings
    // (you can also pass in a Wire library object like &Wire2)
    setup_wifi();
    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);

    mpu6050.begin();
    mpu6050.calcGyroOffsets(true);
    Wire.begin();

    pinMode(TRIGGER_PIN, OUTPUT); // Sets the trigPin as an Output
    pinMode(ECHO_PIN, INPUT);     // Sets the echoPin as an Input
}

void setup_wifi()
{
    delay(10);
    // We start by connecting to a WiFi network
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(0,0,2);
    tft.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    tft.print(WiFi.localIP());
}

void callback(char *topic, byte *message, unsigned int length)
{
    Serial.print("Message arrived on topic: ");
    Serial.print(topic);
    Serial.print(". Message: ");
    String messageTemp;

    for (int i = 0; i < length; i++)
    {
        Serial.print((char)message[i]);
        messageTemp += (char)message[i];
    }
    Serial.println();

    if (String(topic) == "Joel/driving")
    {
        Serial.print("Changing output to ");
        if (messageTemp == "on")
        {
            Serial.println("on");
            rightSpeed = leftSpeed = 255;
        }
        else if (messageTemp == "off")
        {
            Serial.println("off");
            rightSpeed = leftSpeed = 0;
        }
    }
    else if (String(topic) == "Joel/steering")
    {
        Serial.print("Now steering ");
        if (messageTemp == "left")
        {
            Serial.println("left");
            servoAngle = 70;
        }
        else if (messageTemp == "right")
        {
            Serial.println("right");
            servoAngle = 110;
        }
        else if (messageTemp == "straight")
        {
            Serial.println("straight");
            servoAngle = 90;
        }
    }
    else if (String(topic) == "Joel/seanDist")
    {
      SeanDist = messageTemp.toFloat();
    }
    SetArduino(rightSpeed, leftSpeed, servoAngle);
}

void reconnect()
{
    // Loop until we're reconnected
    while (!client.connected())
    {
        tft.fillScreen(TFT_BLACK);
        tft.setCursor(0,0,2);
        tft.print("Reconnecting ");
        tft.fillCircle(80, 160, 30, TFT_YELLOW);
        tft.fillCircle(80, 173, 15, TFT_BLACK);
        tft.fillCircle(80, 176, 14, TFT_YELLOW);
        tft.fillCircle(70, 150, 5, TFT_BLACK);
        tft.fillCircle(90, 150, 5, TFT_BLACK);
        tft.print(".");

        Serial.print("Attempting MQTT connection...");
        // Attempt to connect
        if (client.connect("JoelEEEBotClient"))
        {
            Serial.println("connected");
            tft.print("Connected!");
            // Subscribe
            client.subscribe("Joel/driving");

            client.subscribe("Joel/steering");

            client.subscribe("Joel/seanDist");
        }
        else
        {
            tft.print(".");
            Serial.print("failed, rc=");
            Serial.print(client.state());
            tft.println(".");
            Serial.println(" try again in 5 seconds");
            // Wait 5 seconds before retrying
            delay(5000);
            tft.print("Retrying");
        }
    }
}
void loop()
{
    if (!client.connected())
    {
        reconnect();
    }
    client.loop();
    {
        distance = CalculateDistance();//this takes time to work so to avoid flashing must be before the screen resets

        tft.fillScreen(TFT_BLACK);
        tft.setCursor(0,0,2);

        mpu6050.update();

        // Convert the value to a char array
        char distString[8];
        dtostrf(distance, 1, 2, distString);
        Serial.print("  Distance: ");
        Serial.println(distString);
        client.publish("Joel/distance", distString);
        tft.print("Distance: ");
        tft.println(distance);

        float angleX = mpu6050.getAngleX();
        while (angleX >= 360)
        {
            angleX -= 360;
        }
        while (angleX <= 0)
        {
            angleX += 360;
        }

        // Convert the value to a char array
        char XString[8];
        dtostrf(angleX, 1, 2, XString);
        Serial.print("  X: ");
        Serial.print(XString);
        client.publish("Joel/X", XString);
        tft.print("X: ");
        tft.print((int)angleX);

        float angleY = mpu6050.getAngleY();
        while (angleY >= 360)
        {
            angleY -= 360;
        }
        while (angleY <= 0)
        {
            angleY += 360;
        }

        // Convert the value to a char array
        char YString[8];
        dtostrf(angleY, 1, 2, YString);
        Serial.print("  Y: ");
        Serial.print(YString);
        client.publish("Joel/Y", YString);
        tft.print(" Y: ");
        tft.print((int)angleY);

        float angleZ = mpu6050.getAngleZ();
        while (angleZ >= 360)
        {
            angleZ -= 360;
        }
        while (angleZ <= 0)
        {
            angleX += 360;
        }

        // Convert the value to a char array
        char ZString[8];
        dtostrf(angleZ, 1, 2, ZString);
        Serial.print("  Z: ");
        Serial.println(ZString);
        client.publish("Joel/Z", ZString);
        tft.print(" Z: ");
        tft.println((int)angleZ);

        ReadEncoders(&rightEncoder, &leftEncoder);

        // Convert the value to a char array
        char RString[8];
        dtostrf(rightEncoder, 1, 2, RString);
        Serial.print("  Right: ");
        Serial.println(RString);
        client.publish("Joel/rightEncoder", RString);
        tft.print("Right Enc: ");
        tft.print(rightEncoder);

        int rightDistance = rightEncoder * (16 / (PI * DIAMETER));

        // Convert the value to a char array
        char RDString[8];
        dtostrf(rightDistance, 1, 2, RDString);
        Serial.print("  Right Distance: ");
        Serial.println(RDString);
        client.publish("Joel/rightDistance", RDString);
        tft.print(" Dist: ");
        tft.println(rightDistance);

        // Convert the value to a char array
        char LString[8];
        dtostrf(leftEncoder, 1, 2, LString);
        Serial.print("  Left: ");
        Serial.println(LString);
        client.publish("Joel/leftEncoder", LString);
        tft.print("Left Enc: ");
        tft.print(leftEncoder);

        int leftDistance = leftEncoder * (16 / (PI * DIAMETER));

        // Convert the value to a char array
        char LDString[8];
        dtostrf(leftDistance, 1, 2, LDString);
        Serial.print("  Left Distance: ");
        Serial.println(LDString);
        client.publish("Joel/leftDistance", LDString);
        tft.print(" Dist: ");
        tft.println(leftDistance);

        float Temp = mpu6050.getTemp();

        // Convert the value to a char array
        char tempString[8];
        dtostrf(Temp, 1, 2, tempString);
        Serial.print("  Temperature: ");
        Serial.println(tempString);
        client.publish("Joel/temperature", tempString);
        tft.print("Temperature: ");
        tft.println(Temp);

        tft.print("Sean's Distance: ");
        tft.println(SeanDist);

        if(distance > 90)
          colour = TFT_GREEN;
        else if(distance < 20)
          colour = TFT_RED;
        else
          colour = TFT_YELLOW;

      //                X   Y   R     Colour
        tft.fillCircle(80, 160, 30, colour);
        tft.fillCircle(80, 165, 15, TFT_BLACK);
        tft.fillCircle(80, 160, 14, colour);
        tft.fillCircle(70, 150, 5, TFT_BLACK);
        tft.fillCircle(90, 150, 5, TFT_BLACK);
    }
}