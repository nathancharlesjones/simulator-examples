#include <stdint.h>			// For uint32_t, uint8_t
#include <math.h>			// For sqrt, pow
#include <string.h>			// For strtok_r
#include <stddef.h>			// For NULL
#include <stdlib.h>			// For atof, atoi
#include "application.h"
#include "hardware.h"

#define MAX_LINE_LEN 32
#define MAX_NUM_ARGS 10

uint32_t period = 10;
uint32_t next = -1;
enum {X, Y, Z, TOTAL} direction_of_interest = X;
double max_accel = 10.0;

void parseString(char * buffer, uint8_t * argc, char ** argv, uint8_t sizeof_argv)
{
	char *rest;
    char *token = strtok_r(buffer, " \n", &rest);
    while (token != NULL && *argc <= sizeof_argv) {
      argv[(*argc)++] = token;
      token = strtok_r(NULL, " \n", &rest);
    }
}

void processCommand(char * cmd_string)
{
	uint8_t argc = 0;
	char* argv[MAX_NUM_ARGS] = {0};

	parseString(cmd_string, &argc, argv, MAX_NUM_ARGS);

	if(strcmp(argv[0],"p") == 0) period = atoi(argv[1]);
	else if(strcmp(argv[0],"m") == 0) max_accel = atof(argv[1]);
	else display((const char*)"Unknown command\n");
}

void charReceivedCallback(char c)
{
	static uint8_t idx = 0;
	static char buffer[MAX_LINE_LEN+1] = {0};
	if(c != '\n' && idx < MAX_LINE_LEN) buffer[idx++] = c;
	else
	{
		processCommand(buffer);
		idx = 0;
		memset(buffer, 0, MAX_LINE_LEN);
	}
}

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