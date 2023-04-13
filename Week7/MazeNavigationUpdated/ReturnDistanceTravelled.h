void DriveDistance(int distance)
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
    register float distanceVariable = 3 * 3.141592653589793 / 16;
    int Distance, rightInitial, leftInitial, rightPos, leftPos, rightRaw, leftRaw;

    ReadEncoders(&rightInitial, &leftInitial);

    do
    {
        ReadEncoders(&rightRaw, &leftRaw);
        rightPos = rightRaw - rightInitial;
        leftPos  = leftRaw  - leftInitial;
        Serial.print("Right: ");
        Serial.print(rightPos);
        Serial.print("  Left: ");
        Serial.println(leftPos);

        if (abs(rightPos) > abs(leftPos))
            RunMotors(140, 130, 97);
        else if (abs(rightPos) < abs(leftPos))
            RunMotors(130, 140, 87);
        else
            RunMotors(140, 140, 92);
        Distance = (abs(rightPos) + abs(leftPos)) / 2;
        // This will let me see how the mpu is behaving and what the current value is, see if rotation is in the deisred direction or not.
    }while (Distance < distance*distanceVariable);
    Serial.print("finished!");

    StopMotors();

    return;
}