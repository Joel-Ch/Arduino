#define TRIGGER_PIN 32
#define ECHO_PIN 33
#define SOUND_SPEED 0.034

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
    float distance = duration * SOUND_SPEED / 2;
    Serial.print(distance);
    delay(500);

    return distance;
}

void setup()
{
    Serial.begin(115200);     // Starts the serial communication
    pinMode(TRIGGER_PIN, OUTPUT); // Sets the trigPin as an Output
    pinMode(ECHO_PIN, INPUT);  // Sets the echoPin as an Input
}

void loop()
{

    

    // Prints the distance in the Serial Monitor
    Serial.print("Distance (cm): ");
    Serial.println(CalculateDistance());
    delay(1000);
}
