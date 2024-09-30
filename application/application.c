#include <stdint.h>			// For uint32_t
#include <math.h>			// For sqrt, pow
#include "application.h"
#include "hardware.h"

uint32_t period = 10;
uint32_t next = -1;
enum direction_of_interest_t direction_of_interest = X;
double max_accel = 10.0;

void accelDoubleTapCallback(void)
{
	if(X == direction_of_interest)		direction_of_interest = Y;
	else if(Y == direction_of_interest)	direction_of_interest = Z;
	else if(Z == direction_of_interest)	direction_of_interest = TOTAL;
	else								direction_of_interest = X;
}

int main(int argc, char ** argv)
{
	initHardware(argc, argv);

	while(1)
	{
		if(next - getMillis() > period)
		{
			double accel, total, x, y, z;
			readAccel_gs(&x, &y, &z);
			total = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
			
			switch(direction_of_interest)
			{
				case X:
					accel = x;
					break;
				case Y:
					accel = y;
					break;
				case Z:
					accel = z;
					break;
				case TOTAL:
					accel = total;
					break;
			}
			setMotorSpeed((accel/max_accel) > 1.0 ? 1.0 : (accel/max_accel));
			setLED((total/max_accel) > 1.0 ? 1.0 : (total/max_accel));
			
			next += period;
		}
	}
}