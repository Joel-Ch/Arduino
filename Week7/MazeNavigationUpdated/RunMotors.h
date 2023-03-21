#include <Wire.h>
#define I2C_SLAVE_ADDR 0x04 // 4 in hexadecimal

void RunMotors(int x, int y, int z)
{
  Wire.begin();
  Wire.beginTransmission(I2C_SLAVE_ADDR);

  Wire.write((byte)((x & 0x0000FF00) >> 8));    // first byte of x, containing bits 16 to 9
  Wire.write((byte)(x & 0x000000FF));           // second byte of x, containing the 8 LSB - bits 8 to 1
  Wire.write((byte)((y & 0x0000FF00) >> 8));    // first byte of y, containing bits 16 to 9
  Wire.write((byte)(y & 0x000000FF));           // second byte of y, containing the 8 LSB - bits 8 to 1
  Wire.write((byte)((z & 0x0000FF00) >> 8));    // first byte of z, containing bits 16 to 9
  Wire.write((byte)(z & 0x000000FF));           // second byte of z, containing the 8 LSB - bits 8 to 1


  Wire.endTransmission();   // stop transmitting
}

void StopMotors()
{
  RunMotors(0, 0, 92);
}

void ReadEncoders(int *x, int *y)
{
  Wire.requestFrom(I2C_SLAVE_ADDR, 4);    // request 4 bytes from slave device #4

  if (Wire.available() == 4)              // if four bytes were received
  {
    while (Wire.available())
    {
      *x = Wire.read() << 8; // receive high byte (overwrites previous reading)
      *x |= Wire.read();     // receive low byte as lower 8 bits
      *y = Wire.read() << 8; // receive high byte (overwrites previous reading)
      *y |= Wire.read();     // receive low byte as lower 8 bits
    }
  }
}