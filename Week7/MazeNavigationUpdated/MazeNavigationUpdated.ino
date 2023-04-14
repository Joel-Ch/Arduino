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

// #define TURNONSPOT
#define inputLength 30

char alpha;
byte rs = 19, en = 23, d4 = 18, d5 = 17, d6 = 16, d7 = 15;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
int instructions[inputLength], i = 0;
String distance[inputLength];//array of strings
String message;

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
      message = message + "F";
      instructions[i] = 2;
      do 
      {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Enter Distance:");
        lcd.setCursor(0, 1);
        lcd.print(distance[i]);
        input = customKeypad.waitForKey();
        if (input == '*')
        {
          if (distance[i].length() != 0)
            distance[i].remove(distance[i].length()-1,1);//if the string has >= 1 item in it, remove the last item
        }
        else
          distance[i] += input;
      } while (input != '#');
      distance[i].remove(distance[i].length()-1,1);//remove the # from the string
      message += distance[i];//add the distance to the message
      lcd.clear();
      Serial.print(distance[i]);//print the distance to the serial monitor
      i++;
    }
    else if (enteredValue == '4')
    {//left
      message = message + "L";
      instructions[i] = 4;
      i++;
    }
    else if (enteredValue == '6')
    {//right
      message = message + "R";
      instructions[i] = 6;
      i++;
    }
    else if (enteredValue == '8')
    {//backwards
      message = message + "B";
      instructions[i] = 8;
      i++;
    }
    else if (enteredValue == '*')
    {//delete
      i--;
      while (isdigit(message.charAt(message.length()-1)))
      {//if the last character is a digit, remove it
        message.remove(message.length()-1,1);
      }
      
      message.remove(message.length()-1,1);//remove the last character
      instructions[i] = 0;//reset the instruction
      distance[i] = "";
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
  
  if (confirm == '#')
  {
    Serial.println("Confirmed!");
    lcd.clear();
    lcd.print(" Don't Touch!");
    for (i = 0; i <= inputLength; i++)
    {
      StopMotors();
      lcd.clear();
      lcd.print(" Don't Touch!");
      lcd.setCursor(0,1);
      lcd.print(i+1);//print the current instruction number(starting at 1 not 0)
      switch (instructions[i])
      {
      case 2://forwards
        lcd.setCursor(4, 1);
        lcd.print("F"); // print instruction on screen
        lcd.setCursor(6, 1);
        lcd.print(distance[i]);
        RunMotors(130, 130, 92);
        DriveDistance(distance[i].toInt());

        // for(int j = 0; j < distance[i].toInt(); j++)
        // {
        //   Serial.println(j);
        //   delay(30);//takes xxms to move 1cm
        // }
        break;
      case 4://left
        lcd.setCursor(4, 1);
        lcd.print("L");
        #ifdef TURNONSPOT
        RunMotors(-130, -130, 92);
        DriveDistance(35);
        #endif
        RunMotors(120, 140, 62);
        ReturnAngleTurned(165);
        #ifdef TURNONSPOT
        RunMotors(-130, -130, 92);
        DriveDistance(35);
        #endif
        break;
      case 6://right
        lcd.setCursor(4, 1);
        lcd.print("R");
        #ifdef TURNONSPOT
        RunMotors(-130, -130, 92);
        DriveDistance(53);
        #endif
        RunMotors(140, 120, 122);
        ReturnAngleTurned(178);
        #ifdef TURNONSPOT
        RunMotors(-130, -130, 92);
        DriveDistance(53);
        #endif
        break;
      case 8:
        lcd.setCursor(4, 1);
        lcd.print("B");
        RunMotors(-130, -130, 92);
        delay(370); // goes back 10cm by default
        StopMotors();
        break;
      
      default:
        Serial.print("Error!");
        break;
      }
    }
    lcd.clear();
    lcd.print("  Task Complete!");
    lcd.setCursor(8, 1);
    lcd.write(byte(0));//heart
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

  for (i = 0; i < inputLength; i++)
  {
    // Serial.print(i);
    // Serial.print(" : ");
    // Serial.print(instructions[i]);
    // Serial.print(" : ");
    // Serial.println(distance[i]);//print out inputs for debugging, then clear them
    instructions[i] = 0;
    distance[i] = "";
  }
  message = "";
  
}
