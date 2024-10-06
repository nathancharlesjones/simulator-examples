#include <stdint.h>         // For uint32_t, uint8_t
#include <math.h>           // For sqrt, pow
#include "application.h"
#include "hardware.h"

uint32_t period = 10;
uint32_t next = -1;
enum {X, Y, Z, TOTAL} direction_of_interest = X;
double max_accel = 10.0;

void accelDoubleTapCallback(void)
{
    if(X == direction_of_interest)      direction_of_interest = Y;
    else if(Y == direction_of_interest) direction_of_interest = Z;
    else if(Z == direction_of_interest) direction_of_interest = TOTAL;
    else                                direction_of_interest = X;
}

int main(int argc, char ** argv)
{
    double accel_vals[4] = {0};

    initHardware(argc, argv);

    while(1)
    {
        if(next - getMillis() > period)
        {
            uint32_t start = getMillis();

            readAccel_gs(&accel_vals[X], &accel_vals[Y], &accel_vals[Z]);
            accel_vals[TOTAL] = sqrt(pow(accel_vals[X], 2) + pow(accel_vals[Y], 2) + pow(accel_vals[Z], 2));

            double motorSpeed = accel_vals[direction_of_interest]/max_accel;
            motorSpeed = motorSpeed >  1.0 ?  1.0 :
                         motorSpeed < -1.0 ? -1.0 : motorSpeed;
            setMotorSpeed(motorSpeed);

            double ledBrightness = accel_vals[TOTAL]/max_accel;
            ledBrightness = ledBrightness >  1.0 ?  1.0 : ledBrightness;
            setLED(ledBrightness);
            
            next = start + period;
        }
    }
}