#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include "ArduinoComms.h"
#include <MPU6050_tockn.h>
#include <Arduino.h>


#define TRIGGER_PIN 32
#define ECHO_PIN 33

#define DIAMETER 5

MPU6050 mpu6050(Wire);

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
int value = 0;

float distance = 0;

int rightSpeed = 0, leftSpeed = 0, servoAngle = 90, rightEncoder, leftEncoder;

void setup()
{
    Serial.begin(115200);
    // default settings
    // (you can also pass in a Wire library object like &Wire2)
    setup_wifi();
    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);
    mpu6050.begin();
    mpu6050.calcGyroOffsets(true);
    pinMode(TRIGGER_PIN, OUTPUT); // Sets the trigPin as an Output
    pinMode(ECHO_PIN, INPUT);     // Sets the echoPin as an Input
    Wire.begin();
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
    SetArduino(rightSpeed, leftSpeed, servoAngle);
}

void reconnect()
{
    // Loop until we're reconnected
    while (!client.connected())
    {
        Serial.print("Attempting MQTT connection...");
        // Attempt to connect
        if (client.connect("JoelEEEBotClient"))
        {
            Serial.println("connected");
            // Subscribe
            client.subscribe("Joel/driving");

            client.subscribe("Joel/steering");
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

    long now = millis();
    if (now - lastMsg > 50)
    {
        lastMsg = now;

        mpu6050.update();
        distance = CalculateDistance();

        // Convert the value to a char array
        char distString[8];
        dtostrf(distance, 1, 2, distString);
        Serial.print("  Distance: ");
        Serial.println(distString);
        client.publish("Joel/distance", distString);

        float angleX = mpu6050.getAngleX();

        // Convert the value to a char array
        char XString[8];
        dtostrf(angleX, 1, 2, XString);
        Serial.print("  X: ");
        Serial.print(XString);
        client.publish("Joel/X", XString);

        float angleY = mpu6050.getAngleY();

        // Convert the value to a char array
        char YString[8];
        dtostrf(angleY, 1, 2, YString);
        Serial.print("  Y: ");
        Serial.print(YString);
        client.publish("Joel/Y", YString);

        float angleZ = mpu6050.getAngleZ();

        // Convert the value to a char array
        char ZString[8];
        dtostrf(angleZ, 1, 2, ZString);
        Serial.print("  Z: ");
        Serial.println(ZString);
        client.publish("Joel/Z", ZString);

        ReadEncoders(&rightEncoder, &leftEncoder);

        // Convert the value to a char array
        char RString[8];
        dtostrf(rightEncoder, 1, 2, RString);
        Serial.print("  Right: ");
        Serial.println(RString);
        client.publish("Joel/rightEncoder", RString);

        int rightDistance = rightEncoder*(16/(PI*DIAMETER));

        // Convert the value to a char array
        char RDString[8];
        dtostrf(rightDistance, 1, 2, RDString);
        Serial.print("  Right Distance: ");
        Serial.println(RDString);
        client.publish("Joel/rightDistance", RDString);

        // Convert the value to a char array
        char LString[8];
        dtostrf(leftEncoder, 1, 2, LString);
        Serial.print("  Left: ");
        Serial.println(LString);
        client.publish("Joel/leftEncoder", LString);
        
        int leftDistance = leftEncoder*(16/(PI*DIAMETER));

        // Convert the value to a char array
        char LDString[8];
        dtostrf(leftDistance, 1, 2, LDString);
        Serial.print("  Left Distance: ");
        Serial.println(LDString);
        client.publish("Joel/leftDistance", LDString);

        float Temp = mpu6050.getTemp();

        // Convert the value to a char array
        char tempString[8];
        dtostrf(Temp, 1, 2, tempString);
        Serial.print("  Temperature: ");
        Serial.println(tempString);
        client.publish("Joel/temperature", tempString);
    }
}