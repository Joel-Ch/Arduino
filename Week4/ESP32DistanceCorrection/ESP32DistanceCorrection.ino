#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include "ArduinoComms.h"
#include <Arduino.h>
#include <SPI.h>
#include <TFT_eSPI.h> // Hardware-specific library

TFT_eSPI tft = TFT_eSPI(); // Invoke custom library

const char *ssid = "C11Chip";
const char *password = "00000000";

// Add your MQTT Broker IP address, example:
// const char* mqtt_server = "192.168.1.144";
const char *mqtt_server = "192.168.137.166";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];

float selection = 20, distance = 0;

int speed = 0;

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
  tft.setCursor(0, 0, 2);
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

  if (String(topic) == "Joel/distanceSelect")
  {
    selection = messageTemp.toFloat();
  }
}

void reconnect()
{
  // Loop until we're reconnected
  while (!client.connected())
  {
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(0, 0, 2);
    tft.print("Reconnecting ");
    tft.fillCircle(80, 160, 30, TFT_RED);
    tft.fillCircle(80, 173, 15, TFT_BLACK);
    tft.fillCircle(80, 176, 14, TFT_RED);
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
      client.subscribe("Joel/distanceSelect");
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
    distance = CalculateDistance(); // this takes time to work so to avoid flashing must be before the screen resets
    
    speed = map(distance, 0, (selection*2),255,-255);
    SetArduino(speed, speed, 90);
    
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(0,0,2);
    tft.print("Distance: ");
    tft.println(distance);
    tft.print("Selection: ");
    tft.println(selection);
    tft.print("Speed: ");
    tft.println(speed);


    if(distance < (selection + 10) || distance > (selection - 10))
    {
      tft.fillCircle(80, 160, 30, TFT_GREEN);
      tft.fillCircle(80, 165, 15, TFT_BLACK);
      tft.fillCircle(80, 160, 14, TFT_GREEN);
      tft.fillCircle(70, 150, 5, TFT_BLACK);
      tft.fillCircle(90, 150, 5, TFT_BLACK);
    }
    else if(distance < (selection + 30) || distance > (selection - 30))
    {
      tft.fillCircle(80, 160, 30, TFT_YELLOW);
      tft.drawRect(60,150,40,5, TFT_BLACK);
      tft.fillCircle(70, 150, 5, TFT_BLACK);
      tft.fillCircle(90, 150, 5, TFT_BLACK);
    }
    else
    {
      tft.fillCircle(80, 160, 30, TFT_RED);
      tft.fillCircle(80, 173, 15, TFT_BLACK);
      tft.fillCircle(80, 176, 14, TFT_RED);
      tft.fillCircle(70, 150, 5, TFT_BLACK);
      tft.fillCircle(90, 150, 5, TFT_BLACK);
    }

  }
}