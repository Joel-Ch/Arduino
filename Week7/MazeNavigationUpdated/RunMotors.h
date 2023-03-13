#include <Wire.h>
#define I2C_SLAVE_ADDR 0x04 // 4 in hexadecimal

void RunMotors(int x, int y, int z)
{
  Wire.begin();
  Wire.beginTransmission(I2C_SLAVE_ADDR);

  Wire.write((byte)((x & 0x0000FF00) >> 8));    // first byte of x, containing bits 16 to 9
  Wire.write((byte)(x & 0x000000FF));           // second byte of x, containing the 8 LSB - bits 8 to 1
  Wire.write((byte)((y & 0x0000FF00) >> 8));    // first byte of x, containing bits 16 to 9
  Wire.write((byte)(y & 0x000000FF));           // second byte of x, containing the 8 LSB - bits 8 to 1
  Wire.write((byte)((z & 0x0000FF00) >> 8));    // first byte of z, containing bits 16 to 9
  Wire.write((byte)(z & 0x000000FF));           // second byte of z, containing the 8 LSB - bits 8 to 1


  Wire.endTransmission();   // stop transmitting
}

void StopMotors()
{
  RunMotors(0, 0, 102);
}