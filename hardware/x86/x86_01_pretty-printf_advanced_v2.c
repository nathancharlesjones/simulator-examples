#include <time.h>			// For clock
#include <pthread.h>		// For pthread_t, pthread_create
#include <stdio.h>			// For printf, scanf
#include <string.h>			// For strtok_r
#include <stdlib.h>			// For atof, atoi
#include <stdint.h>			// For uint32_t
#include <stdbool.h>		// For bool, true, false
#include "application.h"

#define MAX_NUM_ARGS 10
#define MAX_INPUT_CHARS 64

// Banner made with https://www.asciiart.eu/text-to-ascii-art

char background[] =	"+-----------------------------------------------------------------------------------------------+\n\r"
					"|    ____  _                 _       _               _____                           _          |\n\r"
					"|   / ___|(_)_ __ ___  _   _| | __ _| |_ ___  _ __  | ____|_  ____ _ _ __ ___  _ __ | | ___     |\n\r"
					"|   \\___ \\| | '_ ` _ \\| | | | |/ _` | __/ _ \\| '__| |  _| \\ \\/ / _` | '_ ` _ \\| '_ \\| |/ _ \\    |\n\r"
					"|    ___) | | | | | | | |_| | | (_| | || (_) | |    | |___ >  < (_| | | | | | | |_) | |  __/    |\n\r"
					"|   |____/|_|_| |_| |_|\\__,_|_|\\__,_|\\__\\___/|_|    |_____/_/\\_\\__,_|_| |_| |_| .__/|_|\\___|    |\n\r"
					"|                                                                             |_|               |\n\r"
					"+-----------------------------------------------------------------------------------------------+\n\r"
					"|+--------------------------+                             +----------------Total---+            |\n\r"
					"||Values generated: %.8s"  "|                             |          +------------+v-----------+|\n\r"
					"||Task period:  %8d"   " ms |                             +-%.7s" "->|Motor speed |LED brightns||\n\r"
					"||Volatility:       %8lf"  "|                             |      +---+------------+------------+|\n\r"
					"|+--------------------------+    +------------------------+---+  |Val|%12lf"     "|%12lf"     "||\n\r"
					"||X:            %12lf"     "+--->|Exp-weighted moving average:|  +---+------------+------------+|\n\r"
					"|+--------------------------+    | x_avg = w*x + (1-w)*x_avg  |  |Max|%12lf"     "|%12lf"     "||\n\r"
					"||Y:            %12lf"     "+--->+----------------------------+  +---+------------+------------+|\n\r"
					"|+--------------------------+    |w:                  %8lf"  "|  |Avg|%12lf"     "|%12lf"     "||\n\r"
					"||Z:            %12lf"     "+--->+----------------------------+  +---+------------+------------+|\n\r"
					"|+--------------------------+                                    |Min|%12lf"     "|%12lf"     "||\n\r"
					"|                                                                +---+------------+------------+|\n\r"
					"|                                  +--------------------------+            ^             ^      |\n\r"
					"|                                  |Max accel:    %12lf"     "+------------+-------------+      |\n\r"
					"|                                  +--------------------------+                                 |\n\r"
					"+-----------------------------------------------------------------------------------------------+\n\r"
       				"|Application commands:                          |Simulator commands:                            |\n\r"
       				"| 't': Toggle x/y/z/total -> Motor speed        | 'r': Toggle random accel values               |\n\r"
       				"| 'm <double>': Set max accel value             | 'x <double>': Set x accelerometer value       |\n\r"
       				"| 'p <int>': Set main task period               | 'y <double>': Set y accelerometer value       |\n\r"
       				"| 'w <double>': Set EWMA coefficient            | 'z <double>': Set z accelerometer value       |\n\r"
       				"|                                               | 'v <double>': Set the 'volatility' of rand    |\n\r"
       				"+-----------------------------------------------------------------------------------------------+\n\r"
       				">> ";

pthread_t h_scanfPthread;
void* scanfPthread(void* data);
double curr_x = 0.0, curr_y = 0.0, curr_z = 0.0;
double speed_val = 0.0, speed_max = 0.0, speed_avg = 0.0, speed_min = 0.0;
double led_val = 0.0, led_max = 0.0, led_avg = 0.0, led_min = 0.0;
bool rand_accels = false;
double volatility = 0.2;
char * doiStrings[] = { [X] = "---X---", [Y] = "---Y---", [Z] = "---Z---", [TOTAL] = "-Total-" };

void updateDisplay(void)
{
	printf("\0337\033[;H");
	printf(background, rand_accels ? "Randomly" : "Manually", getPeriod(), doiStrings[getDirectionOfInterest()], volatility, speed_val, \
		led_val, curr_x, speed_max, led_max, curr_y, getEwmaCoeff(), speed_avg, led_avg, curr_z, speed_min, led_min, getMaxAccel());
	printf("\0338");
	fflush(stdout);
}

void initHardware(int argc, char ** argv)
{
	printf("\033[2J\033[;H");		// Clear terminal
	updateDisplay();
	printf("\033[32;4H");
	pthread_create(&h_scanfPthread, NULL, scanfPthread, NULL);
}

void* scanfPthread(void* data)
{
	int argc = 0;
	char *argv[MAX_NUM_ARGS] = {0};

	while(1)
	{
		uint8_t idx = 0;
		char buffer[MAX_INPUT_CHARS+1] = {0};
		char c = getchar();
		if(c != 'x' && c != 'y' && c != 'z' && c != 't' && c != 'r' && c != 'v')			// Message meant for application code
		{
			while(c != '\n')
			{
				charReceivedCallback(c);
				c = getchar();
			}
			charReceivedCallback(c);
		}
		else								// Message meant to simulate input
		{
			while(c != '\n' && idx < MAX_INPUT_CHARS)
			{
				buffer[idx++] = c;
				c = getchar();
			}

			// Tokenize
			char *rest;
		    char *token = strtok_r(buffer, " \n", &rest);
		    while (token != NULL) {
		      argv[argc++] = token;
		      token = strtok_r(NULL, " \n", &rest);
		    }

		    switch(argv[0][0])
		    {
		    	case 'x':
		    		curr_x = atof(argv[1]);
		    		break;
		    	case 'y':
		    		curr_y = atof(argv[1]);
		    		break;
		    	case 'z':
		    		curr_z = atof(argv[1]);
		    		break;
		    	case 't':
		    		accelDoubleTapCallback();
		    		break;
		    	case 'r':
		    		rand_accels = !rand_accels;
		    		break;
	    		case 'v':
	    			volatility = atof(argv[1]);
	    			break;
		    }

	    	argc = 0;
		}

		updateDisplay();
		printf("\033[32;4H");
		printf("                                                                                              ");
		printf("\033[32;4H");
		fflush(stdout);
	}
}

uint32_t getMillis(void)
{
	return (uint32_t)((double)clock() * 1000.0 / CLOCKS_PER_SEC);
}

void readAccel_gs(double* x, double* y, double* z)
{
	if(rand_accels)
	{
		curr_x = curr_x + volatility*(((double)rand()/RAND_MAX)*2 - 1);
		curr_y = curr_y + volatility*(((double)rand()/RAND_MAX)*2 - 1);
		curr_z = curr_z + volatility*(((double)rand()/RAND_MAX)*2 - 1);
	}
	*x = curr_x;
	*y = curr_y;
	*z = curr_z;
}

void setMotorSpeed(double speed)
{
	static uint32_t next = -1;
	const uint32_t period = 100;
	static double sum = 0;
	static uint32_t count = 0;

	if(next - getMillis() > period)
	{
		updateDisplay();
		sum = 0;
		count = 0;
		next += period;
	}
	speed_val = speed;
	if(speed > speed_max) speed_max = speed;
	if(speed < speed_min) speed_min = speed;
	sum += speed;
	count++;
	speed_avg = sum/count;
}

void setLED(double brightness)
{
	static uint32_t next = -1;
	const uint32_t period = 100;
	static double sum = 0;
	static uint32_t count = 0;

	if(next - getMillis() > period)
	{
		updateDisplay();
		sum = 0;
		count = 0;
		next += period;
	}
	led_val = brightness;
	if(brightness > speed_max) led_max = brightness;
	if(brightness < speed_min) led_min = brightness;
	sum += brightness;
	count++;
	led_avg = sum/count;
}

void display(const char * msg)
{
	//printf("%s", msg);
}