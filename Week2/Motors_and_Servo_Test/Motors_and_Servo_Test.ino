#include <Servo.h>    //include the servo library
#include <Arduino.h>
#define servoPin 4
Servo myservo;        // create servo object to control a servo
float steeringAngle;  // variable to store the servo position

#define enA 5   //EnableA command line - should be a PWM pin
#define enB 6   //EnableB command line - should be a PWM pin

//name the motor control pins - replace the CHANGEME with your pin number, digital pins do not need the 'D' prefix whereas analogue pins need the 'A' prefix
#define INa A0  //Channel A direction 
#define INb A1  //Channel A direction 
#define INc A2  //Channel B direction 
#define INd A3  //Channel B direction 

byte speedSetting = 0;  //initial speed = 0


void setup() {
  // put your setup code here, to run once:

  myservo.attach(servoPin);  //attach our servo object to pin D4
  //the Servo library takes care of defining the PinMode declaration (libraries/Servo/src/avr/Servo.cpp line 240)

  //configure the motor control pins as outputs
  pinMode(INa, OUTPUT);
  pinMode(INb, OUTPUT);
  pinMode(INc, OUTPUT);
  pinMode(INd, OUTPUT);
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);

  //initialise serial communication
  Serial.begin(9600);
  Serial.println("Arduino Nano is Running"); //sanity check

  speedSetting = 255;
  motors(speedSetting, speedSetting); //make a call to the "motors" function and provide it with a value for each of the 2 motors - can be different for each motor - using same value here for expedience
  Serial.print("Motor Speeds: ");
  Serial.println(speedSetting);
  myservo.write(38);
}


void loop() {
  int speedLeft = 255;
  int speedRight = 255;
  int slowSpeedRight = 200;
  int slowSpeedLeft = 200;
  moveSteering();
  delay(500);
  // goForwards(speedLeft, speedRight);
  // delay(100);
  // delay(3000);
  // stopMotors(speedLeft, speedRight);
  // delay(500);
  // goBackwards(speedLeft, speedRight);
  // delay(3000);
  // stopMotors(speedLeft, speedRight);
  // delay(500);
  // goClockwise(speedLeft, slowSpeedRight);
  // delay(3000);
  // stopMotors(speedLeft, speedRight);
  // delay(500);
  // goAntiClockwise(slowSpeedLeft, speedRight);
  // delay(3000);
  // stopMotors(speedLeft, speedRight);
  // delay(500);
  // goBackAntiClockwise(speedLeft, slowSpeedRight);
  // delay(3000);
  // stopMotors(speedLeft, speedRight);
  // delay(500);
  // goBackClockwise(slowSpeedLeft, speedRight);
  // delay(3000);
  // stopMotors(speedLeft, speedRight);
  // delay(500);
  // goClockwise(speedLeft, slowSpeedRight);
  // delay(3900);
  // goAntiClockwise(slowSpeedLeft, speedRight);
  // delay(3900);
  // AnalogDriveB(speedLeft, speedRight);
  // delay(500);
  // stopMotors(speedLeft, speedRight);
}

void motors(int leftSpeed, int rightSpeed) {
  //set individual motor speed
  //direction is set separately

  analogWrite(enA, leftSpeed);
  analogWrite(enB, rightSpeed);
}

void moveSteering() {
  //you may need to change the maximum and minimum servo angle to have the largest steering motion
  int maxAngle = 130;
  int minAngle = 50;
  myservo.write(0);
  for (steeringAngle = minAngle; steeringAngle <= maxAngle; steeringAngle += 1) { //goes from minAngle to maxAngle (degrees)
    //in steps of 1 degree
    myservo.write(steeringAngle);   //tell servo to go to position in variable 'steeringAngle'
    delay(15);                      //waits 15ms for the servo to reach the position
    Serial.print("Turning\n");
  }
  for (steeringAngle = maxAngle; steeringAngle >= minAngle; steeringAngle -= 1) { // goes from maxAngle to minAngle (degrees)
    myservo.write(steeringAngle);   //tell servo to go to position in variable 'steeringAngle'
    delay(15);                      //waits 15 ms for the servo to reach the position
    Serial.print("Turning Back\n");
  }
  delay(20);
}


//for each of the below function, two of the 'IN' variables must be HIGH, and two LOW in order to move the wheels - use a trial and error approach to determine the correct combination for your EEEBot
void goForwards(int left, int right) {
  motors(left, right);
  myservo.write(38);
  digitalWrite(INa, LOW);
  digitalWrite(INb, HIGH);
  digitalWrite(INc, HIGH);
  digitalWrite(INd, LOW);
}

void goBackwards(int left, int right) {
  motors(left, right);
  myservo.write(38);
  digitalWrite(INa, HIGH);
  digitalWrite(INb, LOW);
  digitalWrite(INc, LOW);
  digitalWrite(INd, HIGH);
}

void goClockwise(int left, int right) {
  motors(left, right);
  myservo.write(63);
  digitalWrite(INa, LOW);
  digitalWrite(INb, HIGH);
  digitalWrite(INc, HIGH);
  digitalWrite(INd, LOW);
}

void goBackClockwise(int left, int right) {
  motors(left, right);
  myservo.write(63);
  digitalWrite(INa, HIGH);
  digitalWrite(INb, LOW);
  digitalWrite(INc, LOW);
  digitalWrite(INd, HIGH);
}

void goAntiClockwise(int left, int right) {
  motors(left, right);
  myservo.write(13);
  digitalWrite(INa, LOW);
  digitalWrite(INb, HIGH);
  digitalWrite(INc, HIGH);
  digitalWrite(INd, LOW);
}

void goBackAntiClockwise(int left, int right) {
  motors(left, right);
  myservo.write(13);
  digitalWrite(INa, HIGH);
  digitalWrite(INb, LOW);
  digitalWrite(INc, LOW);
  digitalWrite(INd, HIGH);
}



void stopMotors(int left, int right) {
  motors(left, right);
  myservo.write(38);
  digitalWrite(INa, LOW);
  digitalWrite(INb, LOW);
  digitalWrite(INc, LOW);
  digitalWrite(INd, LOW);
}

void AnalogDrive(int left, int right) {
  motors(left, right);
  int i, max = 255;
  for (size_t i = 0; i < max; i++)
  {
    analogWrite(enA, i);
    analogWrite(enA, i);
  }
  for (size_t i = 255; i > 0; i--)
  {
    analogWrite(enA, i);
    analogWrite(enA, i);
  }
}

void AnalogDriveB(int left, int right) {
  myservo.write(38);
  int i, max = 2000, time;
  for (size_t i = 0; i < max; i++)
  {
    time = (i/20);
    digitalWrite(INb, HIGH); 
    digitalWrite(INc, HIGH); 
    delayMicroseconds(1000*(time));   // waits for t1 uS (high time)
    digitalWrite(INb, LOW);
    digitalWrite(INc, LOW);
    delayMicroseconds(1000*(100-time));   // waits for t2 uS (low time
  }
  
}
