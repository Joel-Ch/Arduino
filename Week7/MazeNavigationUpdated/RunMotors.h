#include <Wire.h>
#define I2C_SLAVE_ADDR 0x04 // 4 in hexadecimal

void RunMotors(int x, int y, int z)
{
  Serial.print(x);
  Serial.print(" ");
  Serial.print(y);
  Serial.print(" ");
  Serial.println(z);

  while(Wire.available()>0)
    Wire.read();

  Wire.begin();
  Wire.beginTransmission(I2C_SLAVE_ADDR);

  Wire.write((byte)((x & 0x0000FF00) >> 8));    // first byte of x, containing bits 16 to 9
  Wire.write((byte)(x & 0x000000FF));           // second byte of x, containing the 8 LSB - bits 8 to 1
  Wire.write((byte)((y & 0x0000FF00) >> 8));    // first byte of y, containing bits 16 to 9
  Wire.write((byte)(y & 0x000000FF));           // second byte of y, containing the 8 LSB - bits 8 to 1
  Wire.write((byte)((z & 0x0000FF00) >> 8));    // first byte of z, containing bits 16 to 9
  Wire.write((byte)(z & 0x000000FF));           // second byte of z, containing the 8 LSB - bits 8 to 1

  Wire.endTransmission();   // stop transmitting
  return;
}

void StopMotors()
{
  RunMotors(0, 0, 92);
  return;
}

void ReadEncoders(int *x, int *y, int *oldX, int *oldY)
{
  int left, right;
  while(Wire.available()>0)
    Wire.read();

  Wire.requestFrom(I2C_SLAVE_ADDR, 5); // request 5 bytes from slave device #4 (4 data bytes + 1 response byte)

  while (Wire.available() > 1) // read the data bytes
  {
    right= Wire.read() << 8; // receive high byte (overwrites previous reading)
    right |= Wire.read();     // receive low byte as lower 8 bits
    left = Wire.read() << 8; // receive high byte (overwrites previous reading)
    left |= Wire.read();     // receive low byte as lower 8 bits
  }
  int response = Wire.read(); // read the response byte
  if (response == 0x01)       // check if the response byte indicates that the data was received correctly
  {
    Serial.println("Data received correctly");
    *x = right;
    *y = left;
    *oldX = *x;
    *oldY = *y;
  }
  else
  {
    Serial.println("Data not received correctly");
    *x = *oldX;
    *y = *oldY;
  }
  Serial.print("Encoders: ");
  Serial.print(*x);
  Serial.print(" ");
  Serial.print(*y);
  return;
}