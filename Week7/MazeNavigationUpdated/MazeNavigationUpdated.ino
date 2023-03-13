
#include <Keypad.h>
#include "RunMotors.h"
#include "ReturnAngleTurned.h"
#include <string>
#include <LiquidCrystal.h>

char alpha;
const int rs = 19, en = 23, d4 = 18, d5 = 17, d6 = 16, d7 = 15;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
int instructions[30], i = 0;
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
  // // Other features wanted:
  // // . Undo, *
  // // . Go button/confirm, 5
  // // . Reset whole thing , #
  // // . 1, 3, 7, 9 and 0 are novelty buttons.

  // // --------------------------------------------------------------- Directions ----------------------------------------------------------------------------------------
  lcd.setCursor(0, 0);
  // lcd.clear();

  do
  {
    lcd.clear();
    lcd.print("   Welcome!");
    delay(3000);
    lcd.clear();
    lcd.print("   Press 5 to");
    lcd.setCursor(0, 1);
    lcd.print(" Continue!");
    lcd.setCursor(11, 1);
    lcd.write(byte(0));
    lcd.write(byte(3));
    delay(2000);
    lcd.setCursor(12, 1);
    lcd.write(byte(4));

    alpha = customKeypad.waitForKey();
  } while (alpha != '5');
  // store all of the inputs into an array, confirm with the user and then execute the commands one by one.

  // Prompt for input and give them the choice to exit selection and begin movement
  lcd.clear();
  lcd.print("2|4|6|8 Movement");
  lcd.setCursor(0, 1);
  lcd.print("[5 To Continue]"); // while loop is exited through pressing 5 and now we are given movement key

  i = 0;
  char enteredValue = customKeypad.waitForKey();

  while (enteredValue != '5')
  { // until we choose to continue we will assign movement to the arrray elements
    lcd.clear();
    lcd.setCursor(2, 0);
    lcd.print("{Directions}");
    lcd.setCursor(0, 1);
    Serial.print("i = "); // Using for troubleshooting
    Serial.print(i);
    Serial.print("\t");
    Serial.print("Value in Array = "); // Using for troubleshooting
    Serial.println((char)instructions[i]);
    if (enteredValue == '2')
    {
      message = message + "F";
      instructions[i] = 2;
      i++;
    }
    else if (enteredValue == '4')
    {
      message = message + "L";
      instructions[i] = 4;
      i++;
    }
    else if (enteredValue == '6')
    {
      message = message + "R";
      instructions[i] = 6;
      i++;
    }
    else if (enteredValue == '8')
    {
      message = message + "B";
      instructions[i] = 8;
      i++;
    }
    lcd.print(message);

    enteredValue = customKeypad.waitForKey();
  }
  Serial.print("Value in position 1: ");
  Serial.println(instructions[0]);
  Serial.print("Value in position 2: ");
  Serial.println(instructions[1]);
  Serial.print("Value in position 3: ");
  Serial.println(instructions[2]);
  Serial.print("Value in position 4: ");
  Serial.println(instructions[3]);

  lcd.clear();
  lcd.print("     Happy?");
  lcd.setCursor(0, 1);
  lcd.print(" 5 - ");
  lcd.setCursor(6, 1);
  lcd.write(byte(1));
  lcd.print(" |  * - ");
  lcd.setCursor(15, 1);
  lcd.write(byte(2));

  char confirm = customKeypad.waitForKey();

  if (confirm == '*')
  {
    lcd.clear();
    lcd.print("   Resetting...");
    lcd.setCursor(0, 1);
    lcd.print("  ¯|_(ツ)_/¯");
    delay(3000);

    return;
  }
  else if (confirm == '5')
  {
    lcd.clear();
    lcd.print(" Don't Touch!");
    lcd.setCursor(0, 1);
    lcd.print("(ง'̀-'́)ง");
    for (i = 0; i <= 30; i++)
    {

      if (instructions[i] == '2')
      {                           // Forwards
        RunMotors(-130, 130, 85); // Assuming 90 degrees is centered for the vehicle.
        delay(370);
        StopMotors();
      }

      else if (instructions[i] == '4')
      { // Left
        RunMotors(-120, 140, 60);
        ReturnAngleTurned(165);
      }

      else if (instructions[i] == '6')
      { // Right
        RunMotors(-140, 120, 120);
        ReturnAngleTurned(165);
      }

      else if (instructions[i] == '8')
      {
        RunMotors(130, -130, 85); // Assuming 90 degrees is centered for the vehicle.
        delay(370);
        StopMotors();
      }
    }
  }

  lcd.clear();
  lcd.print("  Task Complete!");
  lcd.setCursor(8, 1);
  lcd.write(byte(0));
  delay(4000);

  for (i = 0; i < 30; i++)
  {
    instructions[i] = 0;
  }
}
