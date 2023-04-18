#include <MPU6050_tockn.h>

MPU6050 mpu6050(Wire);
//right=true,left=false
void TurnAngle (bool right = true)
{
  int angleturned = 0, targetangle;

  if (right)
  {
    Serial.print("Turning right");
    RunMotors(140, 120, 122);
    targetangle = 178;
  }
  else
  {
    RunMotors(120,140,62);
    targetangle = 165;
  }

  mpu6050.update();
  int startingangle = mpu6050.getAngleZ();

  while(abs(angleturned) < targetangle)
  {
    mpu6050.update();
    angleturned = ((int)mpu6050.getAngleZ() - startingangle);
    Serial.print("Current angle turned: ");
    Serial.println(angleturned);   
    
  }
  

return;
}