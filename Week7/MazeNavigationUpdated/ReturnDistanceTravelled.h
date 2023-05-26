//forward=true, backward=false
void DriveDistance(int distance, int driveForward)
{

    /*convert from distance to encoder ticks
    diameter = 6cm -> radius = 3cm
    circumference = 3xpi (cm)
    circumference = 16 encoder ticks
    so 16 encoder ticks = 3pi cm
    16/3pi encoder ticks = 1cm
    we get a distance in cm and need to convert to encoder ticks
    encoder ticks = xcm * 3pi/16
    encTick = 1/16 x 3 x pi x distance
    encTick = distance * distanceVariable where distanceVariable = 1/16 x 3 x pi
*/
    register float distanceVariable = 6 * 3.141592653589793 / 16;
    int Distance, rightInitial, leftInitial, rightPos, leftPos, rightRaw, leftRaw, back, oldRight, oldLeft;
    while(Wire.available()>0)
    Wire.read();

    Serial.print("Initial: ");
    ReadEncoders(&rightInitial, &leftInitial, &oldRight, &oldLeft);

    while(Wire.available()>0)
    Wire.read();

    Serial.print(driveForward);

    if (driveForward)
    {
        back = 1;
        Serial.print("Forwards!");
    }
    else
    {
        back = -1;
        Serial.print("Backwards!");
    }
    RunMotors(100*back, 100*back, 94);

    do
    {
        ReadEncoders(&rightRaw, &leftRaw, &oldRight, &oldLeft);
        rightPos = abs(rightRaw - rightInitial);
        leftPos  = abs(leftRaw  - leftInitial);
        Serial.print("Right: ");
        Serial.print(rightPos);
        Serial.print("  Left: ");
        Serial.println(leftPos);

        if (rightPos > leftPos)
            RunMotors(100*back, 90*back, 99);
        else if (rightPos < leftPos)
            RunMotors(90*back, 100*back, 89);
        else
            RunMotors(100*back, 100*back, 94);
        Distance = (rightPos + leftPos) / 2;
    }while (Distance < distance*distanceVariable);
    Serial.print("finished!");

    StopMotors();

    return;
}