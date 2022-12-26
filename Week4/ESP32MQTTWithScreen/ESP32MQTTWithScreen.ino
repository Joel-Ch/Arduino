#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include "ArduinoComms.h"
#include <MPU6050_tockn.h>
#include <SPI.h>
#include <TFT_eSPI.h> // Hardware-specific library

#define TRIGGER_PIN 32
#define ECHO_PIN 33

#define CENTREX 145

#define DIAMETER 5

MPU6050 mpu6050(Wire);

TFT_eSPI tft = TFT_eSPI();  // Invoke custom library

float CalculateDistance()
{
    digitalWrite(TRIGGER_PIN, LOW);
    delayMicroseconds(2);
    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(TRIGGER_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIGGER_PIN, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    long duration = pulseIn(ECHO_PIN, HIGH);
    // Calculate the distance
    float distance = duration * 0.034 / 2;
    delay(500);

    return distance;
}

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

float distance = 0, willTemp = 0;
float oldDistance = 0, oldWillTemp = 0;

int rightDistance, leftDistance;
float temp;

int rightSpeed = 0, leftSpeed = 0, servoAngle = 90, rightEncoder, leftEncoder, angleX = 0, angleY = 0, angleZ = 0;
int First = 0;

int colour;

int CENTREY = 60;

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

    tft.fillScreen(TFT_BLACK);

    // mpu6050.update();
    // angleX = ((int)mpu6050.getAngleX())%360;
    // angleY = ((int)mpu6050.getAngleY())%360;
    // angleZ = ((int)mpu6050.getAngleZ())%360;
}

void setup_wifi()
{
    delay(10);
    // We start by connecting to a WiFi network
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

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
    else if (String(topic) == "WTempMPU")
    {
      willTemp = messageTemp.toFloat();
    }
    SetArduino(rightSpeed, leftSpeed, servoAngle);
}

void reconnect()
{
    // Loop until we're reconnected
    while (!client.connected())
    {
        client.publish("Joel/connected", "disconnected");

        Serial.print("Attempting MQTT connection...");
        // Attempt to connect
        if (client.connect("JoelEEEBotClient", "", "", "Joel/connected", 0, false, "offline"))
        {
            Serial.println("connected");

            client.publish("Joel/connected", "online");
            // Subscribe
            client.subscribe("Joel/driving");

            client.subscribe("Joel/steering");

            client.subscribe("WTempMPU");
        }
        else
        {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            // Wait 5 seconds before retrying
            delay(5000);
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

        if(First = 1)
        {
          tft.setCursor(0,0,2);
          tft.setTextColor(TFT_WHITE);
          tft.print("Distance: ");
          tft.setTextColor(TFT_BLACK);
          tft.println(oldDistance);
          tft.setTextColor(TFT_WHITE);
          tft.print("X: ");
          tft.setTextColor(TFT_BLACK);
          tft.println(angleX);
          tft.setTextColor(TFT_WHITE);
          tft.print("Y: ");
          tft.setTextColor(TFT_BLACK);
          tft.println(angleY);
          tft.setTextColor(TFT_WHITE);
          tft.print("Z: ");
          tft.setTextColor(TFT_BLACK);
          tft.println(angleZ);
          tft.setTextColor(TFT_WHITE);
          tft.print("Right Enc: ");
          tft.setTextColor(TFT_BLACK);
          tft.println(rightEncoder);
          tft.setTextColor(TFT_WHITE);
          tft.print("Dist: ");
          tft.setTextColor(TFT_BLACK);
          tft.println(rightDistance);
          tft.setTextColor(TFT_WHITE);
          tft.print("Left Enc: ");
          tft.setTextColor(TFT_BLACK);
          tft.println(leftEncoder);
          tft.setTextColor(TFT_WHITE);
          tft.print("Dist: ");
          tft.setTextColor(TFT_BLACK);
          tft.println(leftDistance);
          tft.setTextColor(TFT_WHITE);
          tft.print("Temperature: ");
          tft.setTextColor(TFT_BLACK);
          tft.println(temp);
          tft.setTextColor(TFT_WHITE);
          tft.print("Will's Temperature: ");
          tft.setTextColor(TFT_BLACK);
          tft.println(oldWillTemp);
        }
        First = 1;
        mpu6050.update();

        tft.setTextColor(TFT_WHITE);
        tft.setCursor(0,0,2);

        // Convert the value to a char array
        char distString[8];
        dtostrf(distance, 1, 2, distString);
        Serial.print("  Distance: ");
        Serial.println(distString);
        client.publish("Joel/distance", distString);
        tft.print("Distance: ");
        tft.println(distance);

        angleX = ((int)mpu6050.getAngleX())%360;

        // Convert the value to a char array
        char XString[8];
        itoa(angleX, XString, 10);
        Serial.print("  X: ");
        Serial.print(XString);
        client.publish("Joel/X", XString);
        tft.print("X: ");
        tft.println(angleX);

        angleY = ((int)mpu6050.getAngleY())%360;

        // Convert the value to a char array
        char YString[8];
        itoa(angleY, YString, 10);
        Serial.print("  Y: ");
        Serial.print(YString);
        client.publish("Joel/Y", YString);
        tft.print("Y: ");
        tft.println(angleY);

        angleZ = ((int)mpu6050.getAngleZ())%360;

        // Convert the value to a char array
        char ZString[8];
        itoa(angleZ, ZString, 10);
        Serial.print("  Z: ");
        Serial.println(ZString);
        client.publish("Joel/Z", ZString);
        tft.print("Z: ");
        tft.println(angleZ);

        ReadEncoders(&rightEncoder, &leftEncoder);

        // Convert the value to a char array
        char RString[8];
        dtostrf(rightEncoder, 1, 2, RString);
        Serial.print("  Right: ");
        Serial.println(RString);
        client.publish("Joel/rightEncoder", RString);
        tft.print("Right Enc: ");
        tft.println(rightEncoder);

        rightDistance = rightEncoder * (16 / (PI * DIAMETER));

        // Convert the value to a char array
        char RDString[8];
        dtostrf(rightDistance, 1, 2, RDString);
        Serial.print("  Right Distance: ");
        Serial.println(RDString);
        client.publish("Joel/rightDistance", RDString);
        tft.print("Dist: ");
        tft.println(rightDistance);

        // Convert the value to a char array
        char LString[8];
        dtostrf(leftEncoder, 1, 2, LString);
        Serial.print("  Left: ");
        Serial.println(LString);
        client.publish("Joel/leftEncoder", LString);
        tft.print("Left Enc: ");
        tft.println(leftEncoder);

        leftDistance = leftEncoder * (16 / (PI * DIAMETER));

        // Convert the value to a char array
        char LDString[8];
        dtostrf(leftDistance, 1, 2, LDString);
        Serial.print("  Left Distance: ");
        Serial.println(LDString);
        client.publish("Joel/leftDistance", LDString);
        tft.print("Dist: ");
        tft.println(leftDistance);

        temp = mpu6050.getTemp();

        // Convert the value to a char array
        char tempString[8];
        dtostrf(temp, 1, 2, tempString);
        Serial.print("  Temperature: ");
        Serial.println(tempString);
        client.publish("Joel/temperature", tempString);
        tft.print("Temperature: ");
        tft.println(temp);

        tft.print("Will's Temperature: ");
        tft.println(willTemp);

        if(distance > 90)
          colour = TFT_GREEN;
        else if(distance < 20)
          colour = TFT_RED;
        else
          colour = TFT_YELLOW;

        tft.fillRect(CENTREX-30,CENTREY-30,CENTREX+30,CENTREY+30,TFT_BLACK);
        
        CENTREY = map(distance, 0, 500, 30,120);

           //          X        Y        R        Colour
        tft.fillCircle(CENTREX, CENTREY,       30, colour);
        tft.fillCircle(CENTREX, CENTREY + 5,   15, TFT_BLACK);
        tft.fillCircle(CENTREX, CENTREY,       14, colour);
        tft.fillCircle(CENTREX-10, CENTREY-10, 5, TFT_BLACK);
        tft.fillCircle(CENTREX+10, CENTREY-10, 5, TFT_BLACK);

        oldDistance = distance;
        oldWillTemp = willTemp;

    }
}