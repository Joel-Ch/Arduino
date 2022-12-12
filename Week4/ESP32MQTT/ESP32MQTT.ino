#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <Arduino.h>
#include "ArduinoComms.h"
#include <MPU6050_tockn.h>

#define TRIGGER_PIN 32
#define ECHO_PIN 33

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
    Serial.print(distance);
    delay(500);

    return distance;
}

// Replace the next variables with your SSID/Password combination
const char *ssid = "REPLACE_WITH_YOUR_SSID";
const char *password = "REPLACE_WITH_YOUR_PASSWORD";

// Add your MQTT Broker IP address, example:
// const char* mqtt_server = "192.168.1.144";
const char *mqtt_server = "YOUR_MQTT_BROKER_IP_ADDRESS";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

float distance = 0;

int rightSpeed = 0, leftSpeed = 0, servoAngle = 90;

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

    // Feel free to add more if statements to control more GPIOs with MQTT

    // If a message is received on the topic esp32/output, you check if the message is either "on" or "off".
    // Changes the output state according to the message
    if (String(topic) == "esp32/output")
    {
        Serial.print("Changing output to ");
        if (messageTemp == "forward")
        {
            Serial.println("forward");
            rightSpeed = leftSpeed = 255;
        }
        else if (messageTemp == "off")
        {
            Serial.println("off");
            rightSpeed = leftSpeed = 0;
        }
        else if (messageTemp == "right")
        {
            Serial.println("right");
            servoAngle = 120;
        }
        else if (messageTemp == "left")
        {
            Serial.println("left");
            servoAngle = 60;
        }
        else if (messageTemp == "straight")
        {
            Serial.println("straight");
            servoAngle = 90;
        }
        else if (messageTemp == "backward")
        {
            Serial.println("backward");
            rightSpeed = leftSpeed = -255;
        }
        SetArduino(rightSpeed, leftSpeed, servoAngle);
    }
}

void reconnect()
{
    // Loop until we're reconnected
    while (!client.connected())
    {
        Serial.print("Attempting MQTT connection...");
        // Attempt to connect
        if (client.connect("ESP8266Client"))
        {
            Serial.println("connected");
            // Subscribe
            client.subscribe("esp32/output");
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
    if (now - lastMsg > 5000)
    {
        lastMsg = now;

        distance = CalculateDistance();
        mpu6050.update();

        // Convert the value to a char array
        char distString[8];
        dtostrf(distance, 1, 2, distString);
        Serial.print("Distance: ");
        Serial.println(distString);
        client.publish("esp32/distance", distString);

        float angleX = mpu6050.getAngleX();

        // Convert the value to a char array
        char XString[8];
        dtostrf(angleX, 1, 2, XString);
        Serial.print("X: ");
        Serial.print(XString);
        client.publish("esp32/X", XString);

        float angleY = mpu6050.getAngleY();

        // Convert the value to a char array
        char YString[8];
        dtostrf(angleY, 1, 2, YString);
        Serial.print("  Y: ");
        Serial.print(YString);
        client.publish("esp32/Y", YString);

        float angleZ = mpu6050.getAngleZ();

        // Convert the value to a char array
        char ZString[8];
        dtostrf(angleZ, 1, 2, ZString);
        Serial.print("  Z: ");
        Serial.print(ZString);
        client.publish("esp32/Z", ZString);
    }
}