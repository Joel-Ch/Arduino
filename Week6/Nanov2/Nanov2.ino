/*This Code communicates via I2C on address 4
It is designed to recieve a string with the format "angle/rightspeed/leftspeed"
The string is then split into it's separate numbers which are sent the the servo/motors*/

#include <Servo.h>
#include <Encoder.h>
#include <Wire.h>

#define servoPin 4

Servo servo;          // create servo object to control a servo
float steeringAngle;  // variable to store the servo position

// either change your wire connections to the two encoders or change the pin numbers in the code
// to change whether the count increments or decrements when turning in a particular direction
Encoder enc1(2, 11);  // create an encoder object for encoder 1
Encoder enc2(3, 12);  // create an encoder object for encoder 2
long oldPos_enc1 = -999;
long oldPos_enc2 = -999;

#define enA 5  // EnableA command line - should be a PWM pin
#define enB 6  // EnableB command line - should be a PWM pin

// name the motor control pins - replace the ** with your pin number, digital pins do not need the 'D' prefix whereas analogue pins need the 'A' prefix
#define INb A0  // Channel A direction
#define INa A1  // Channel A direction
#define INc A2  // Channel B direction
#define INd A3  // Channel B direction

void setup() {

  Wire.begin(4);
  Wire.onReceive(receiveEvent); // register event
  //Wire.onRequest(requestEvent); // register event

  servo.attach(servoPin);  // attach our servo object to pin D4
  // the Servo library takes care of defining the PinMode declaration (libraries/Servo/src/avr/Servo.cpp line 240)

  // configure the motor control pins as outputs
  pinMode(INa, OUTPUT);
  pinMode(INb, OUTPUT);
  pinMode(INc, OUTPUT);
  pinMode(INd, OUTPUT);
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);

  Serial.begin(9600);// initialise serial communication
  Serial.println("Arduino Nano is Running");  // sanity check
}


void loop() {
  // update the two encoder values
  long enc1_count = enc1.read();
  long enc2_count = enc2.read();
  if (enc1_count != oldPos_enc1) {
    oldPos_enc1 = enc1_count;
  }
  if (enc2_count != oldPos_enc2) {
    oldPos_enc2 = enc2_count;
  }
}

// this function executes when data is requested from the master device
/*void requestEvent(void) {
  Wire.write(oldPos_enc1);
  Wire.write(oldPos_enc2);
}*/

// this function executes whenever data is received from the master device
void receiveEvent(int howMany) {
  //Set Up Vars
  int Result[3] = {};
  char data[13] = {};
  int i = 0;

  while (Wire.available()) {
    char c = Wire.read();  // receive a byte as character
    Serial.print(c);
    Serial.print("|");
    data[i]=c;//add character to string
    i++;
  }
  i=0;
  char *number = strtok(data, "/");//split string by slashes
  while (number != NULL)
  {
    Result[i] = atoi(number);//convert each number to an integer, then store them in Result[]
    i++;
    number = strtok(NULL, "/");    
  }
  
  // verify that the correct values are received via the serial monitor
  Serial.print("Angle: ");
  Serial.print(Result[0]);
  Serial.print(" Right Motor: ");
  Serial.print(Result[1]);
  Serial.print(" Left Motor: ");
  Serial.println(Result[2]);

  setSteeringAngle(Result[0]);//set steering angle and motor speeds
  runMotors(Result[2], Result[1]);
}


// function to clear the I2C buffer
/*void emptyBuffer(void) {
  Serial.println("Error: I2C Byte Size Mismatch");  // i.e. an incorrect number of bytes has been received
  while (Wire.available()) {
    Wire.read();
  }
}*/


// function to set the steering angle
void setSteeringAngle(int servoAngle) {
  servoAngle = constrain(servoAngle, 50, 130);  // prevents the servo being set to 'impossible' angles
  servo.write(servoAngle);
}


// function to run the motors
void runMotors(int leftMotor_speed, int rightMotor_speed) {
  // limit the speed value between -255 and 255 as the PWM value can only be between 0 and 255 - the negative is handled below
  leftMotor_speed = constrain(leftMotor_speed, -255, 255);
  rightMotor_speed = constrain(rightMotor_speed, -255, 255);

  // vary the motor speeds - use the absolute value to remove the negative
  analogWrite(enA, abs(leftMotor_speed));
  analogWrite(enB, abs(rightMotor_speed));

  // if the speed value is negative, run the motor backwards
  if (leftMotor_speed < 0) {
    digitalWrite(INa, LOW);
    digitalWrite(INb, HIGH);
  }
  // else, run the motor forwards
  else {
    digitalWrite(INa, HIGH);
    digitalWrite(INb, LOW);
  }

  // if the speed value is negative, run the motor backwards
  if (rightMotor_speed < 0) {
    digitalWrite(INc, LOW);
    digitalWrite(INd, HIGH);
  }
  // else run the motor forwards
  else {
    digitalWrite(INc, HIGH);
    digitalWrite(INd, LOW);
  }
}
