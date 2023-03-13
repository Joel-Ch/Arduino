#include <MPU6050_tockn.h>

MPU6050 mpu6050(Wire);

void ReturnAngleTurned (int targetangle)
{
  int angleturned = 0;

  mpu6050.update();
  int startingangle = mpu6050.getAngleZ();

  while(abs(angleturned) < targetangle)
  {
    mpu6050.update();
    angleturned = ((int)mpu6050.getAngleZ() - startingangle);
    Serial.print("Current angle turned: ");
    Serial.println(angleturned);   
    
         //This will let me see how the mpu is behaving and what the current value is, see if rotation is in the deisred direction or not.
  }
  StopMotors();
  

return;
}