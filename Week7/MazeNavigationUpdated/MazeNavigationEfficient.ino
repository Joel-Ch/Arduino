//turning left 35 x and y (ie drives 35cm forwards and 35cm left while turning left)
//turning right 53 x and y (ditto but 53cm)
/*
trial maze actual measurements:
F 220  ->  170
R
F 130 -> 50
L
F 60 -> -20
R
F X 


*/
#include <Keypad.h>
#include "RunMotors.h"
#include "ReturnAngleTurned.h"
#include "ReturnDistanceTravelled.h"
#include <string>
#include <LiquidCrystal.h>
#include <cctype>

#define inputLength 30
// #define TURNONSPOT

byte rs = 19, en = 23, d4 = 18, d5 = 17, d6 = 16, d7 = 15;
char alpha;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

byte smiley[8] = {
    0b00000,
    0b00000,
    0b01010,
    0b00000,
    0b00000,
    0b10001,
    0b01110,
    0b00000};

byte frownie[8] = {
    0b00000,
    0b00000,
    0b01010,
    0b00000,
    0b00000,
    0b00000,
    0b01110,
    0b10001};

byte heart[8] = {
    0b00000,
    0b01010,
    0b11111,
    0b11111,
    0b11111,
    0b01110,
    0b00100,
    0b00000};

byte armsDown[8] = {
    0b00100,
    0b01010,
    0b00100,
    0b00100,
    0b01110,
    0b10101,
    0b00100,
    0b01010};

byte armsUp[8] = {
    0b00100,
    0b01010,
    0b00100,
    0b10101,
    0b01110,
    0b00100,
    0b00100,
    0b01010};

const byte ROWS = 4; // four rows
const byte COLS = 3; // four columns
// define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'7', '8', '9'},
    {'*', '0', '#'}};
byte rowPins[ROWS] = {2, 0, 4, 5}; // connect to the row pinouts of the keypad
byte colPins[COLS] = {14, 27, 26}; // connect to the column pinouts of the keypad

// initialize an instance of class NewKeypad
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

void setup()
{
  Serial.begin(9600);
  Wire.begin();
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.createChar(0, heart);
  lcd.createChar(1, smiley);
  lcd.createChar(2, frownie);
  lcd.createChar(3, armsDown);
  lcd.createChar(4, armsUp);
}

void loop()
{
  int i = 0;
  String message = "";
  int distance = 0;
  // // 2 - Forward, 4 - Left, 8 - Backwards, 6 - Right
  // // . Go button/confirm, #
  // // . 1, 3, 7, 9 and 0 are novelty buttons.

  // // --------------------------------------------------------------- Directions ----------------------------------------------------------------------------------------
  lcd.setCursor(0, 0);
  // lcd.clear();

  while (alpha != '#')
  {
    lcd.clear();
    lcd.print("   Welcome!");
    delay(3000);
    lcd.clear();
    lcd.print("   Press # to");
    lcd.setCursor(0, 1);
    lcd.print(" Continue!");
    lcd.setCursor(11, 1);
    lcd.write(byte(0));
    lcd.write(byte(3));
    delay(2000);
    lcd.setCursor(12, 1);
    lcd.write(byte(4));

    alpha = customKeypad.waitForKey();
  }
  lcd.clear();
  lcd.print("2|4|6|8 Movement");
  lcd.setCursor(0, 1);
  lcd.print("Y:# N:*");

  i = 0;
  char enteredValue = customKeypad.waitForKey();

  while (enteredValue != '#')
  { // until we choose to continue we will assign movement to the arrray elements
    lcd.clear();
    lcd.setCursor(2, 0);
    lcd.print("{Directions}");
    lcd.setCursor(0, 1);
    if (enteredValue == '2')
    {
      message += "F";
      lcd.print(message);
      Serial.print("F: ");

      char input='0';
      do 
      {
        input = customKeypad.waitForKey();
        if (input == '*')
        {
          if (isdigit(message.charAt(message.length() - 1)))
            message.remove(message.length() - 1, 1);
            Serial.print("\b");
        }
        else //if (input != '#')
        message += input;
        Serial.print(input);
        lcd.clear();
        lcd.setCursor(2, 0);
        lcd.print("{Directions}");
        lcd.setCursor(0, 1);
        lcd.print(message);
      } while (input != '#');
      message.remove(message.length() - 1, 1);
      Serial.println("");
    }
    else if (enteredValue == '4')
    {//left
      message += "L";
      Serial.println("L");
    }
    else if (enteredValue == '6')
    {//right
      message += "R";
      Serial.println("R");
    }
    else if (enteredValue == '8')
    {//backwards
      message += "B";
      Serial.println("B");
    }
    else if (enteredValue == '*')
    {//delete
      while (isdigit(message.charAt(message.length() - 1)))
      {//if the last character is a digit, remove it
        message.remove(message.length()-1,1);
      }
      
      message.remove(message.length()-1,1);//remove the last character
      Serial.println("Deleted");
    }
    lcd.setCursor(2, 0);
    lcd.print("{Directions}");
    lcd.setCursor(0,1);
    lcd.print(message);
    // Serial.print("i = "); // Using for troubleshooting
    // Serial.print(i);
    // Serial.print("\t");
    // Serial.print("Value in Array = ");
    // Serial.println(instructions[i]);

    enteredValue = customKeypad.waitForKey();
  }
  message+='#';

  Serial.println(message);

  //inputs are in! time for execution
  // for (i = 0; i < inputLength; i++)
  // {
  //   Serial.print("Position ");
  //   Serial.print(i);
  //   Serial.print(" : ");
  //   Serial.println(instructions[i]);
  // }

  lcd.clear();
  lcd.print("     Happy?");
  lcd.setCursor(0, 1);
  lcd.print(" # - ");
  lcd.setCursor(6, 1);
  lcd.write(byte(1));
  lcd.print(" |  * - ");
  lcd.setCursor(15, 1);
  lcd.write(byte(2));

  char confirm = customKeypad.waitForKey();
  while (confirm != '#' && confirm != '*')
  {
    confirm = customKeypad.waitForKey();//ensure input is either # or *
  }
  
  i = 0;
  if (confirm == '#')
  {
    Serial.println("Confirmed!");
    lcd.clear();
    lcd.print(" Don't Touch!");
    while (message[i] != '#')
    {
      lcd.clear();
      lcd.print(" Don't Touch!");
      Serial.print(i + 1);
      Serial.print(" : ");
      Serial.println(message[i]);
      switch (message[i])
      {
      case 'F'://forwards
        lcd.setCursor(4, 1);
        lcd.print("F"); // print instruction on screen
        lcd.setCursor(6, 1);
        Serial.print("Forwards?");
        distance = 0;
        ++i;
        while (isdigit(message.charAt(i)))
        {
          distance = distance*10 + (int)(message[i]-'0');
          ++i;
        }
        --i;
        lcd.print(distance);
        DriveDistance(distance,1);
        break;
      case 'L'://left
        lcd.setCursor(4, 1);
        lcd.print("L");
        #ifdef TURNONSPOT
        DriveDistance(35, 0);
        #endif
        TurnAngle(0);
        #ifdef TURNONSPOT
        DriveDistance(35, 0);
        #endif
        break;
      case 'R'://right
        lcd.setCursor(4, 1);
        lcd.print("R");
        #ifdef TURNONSPOT
        DriveDistance(53,0);
        #endif
        TurnAngle();
        #ifdef TURNONSPOT
        DriveDistance(53,0);
        #endif
        break;
      case 'B':
        lcd.setCursor(4, 1);
        lcd.print("B");
        Serial.print("Backwards?");
        DriveDistance(20, 0);
        Serial.print("Backwar");
        // RunMotors(-130, -130, 92);
        // delay(370); // goes back 10cm by default
        // StopMotors();
        break;
      
      default:
        Serial.print("Error!");
        break;
      }
      i++;
    }
    StopMotors();//trialing only stopping at the end not between steps
    lcd.clear();
    lcd.print("  Task Complete!");
    lcd.setCursor(8, 1);
    lcd.write(byte(0));//heart
    while(Wire.available()>0)
      Wire.read();
    StopMotors();
    delay(3000);
  }

  lcd.clear();
  lcd.print("   Resetting...");
  lcd.setCursor(7, 1);//cool little animation:
  lcd.print("|");
  delay(200);
  lcd.setCursor(7, 1);
  lcd.print('/');
  delay(200);
  lcd.setCursor(7, 1);
  lcd.print('-');
  delay(200);
  lcd.setCursor(7, 1);
  lcd.print('\\');
  delay(200);
  lcd.setCursor(7, 1);
  lcd.print('|');
  delay(200);
  lcd.setCursor(7, 1);
  lcd.print('/');
  delay(200);
  lcd.setCursor(7, 1);
  lcd.print('-');
  delay(200);
  lcd.setCursor(7, 1);
  lcd.print('\\');
  delay(200);
  lcd.setCursor(7, 1);
  lcd.print('|');
  //lcd.print("  ¯|_(ツ)_/¯");
}
