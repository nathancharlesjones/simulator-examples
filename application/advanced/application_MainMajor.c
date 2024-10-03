#include <stdint.h>     // For uint32_t, uint8_t
#include <math.h>     // For sqrt, pow
#include <string.h>     // For strtok_r
#include <stddef.h>     // For NULL
#include <stdlib.h>     // For atof, atoi
#include <stdbool.h>    // For bool, true, false
#include "application.h"
#include "hardware.h"

extern char *strtok_r(char *, const char *, char **);

#define MAX_LINE_LEN 32
#define MAX_NUM_ARGS 10

uint32_t period = 10;
uint32_t next = -1;
direction_of_interest_t direction_of_interest = X;
double max_accel = 10.0;
double ewma_coeff = 0.2;

double getMaxAccel(void)
{
    return max_accel;
}

uint32_t getPeriod(void)
{
    return period;
}

double getEwmaCoeff(void)
{
    return ewma_coeff;
}

direction_of_interest_t getDirectionOfInterest(void)
{
    return direction_of_interest;
}

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
  
    if(argc > 1)
    {
        if(strcmp(argv[0],"p") == 0) period = atoi(argv[1]);
        else if(strcmp(argv[0],"m") == 0) max_accel = atof(argv[1]);
        else if(strcmp(argv[0],"w") == 0) ewma_coeff = atof(argv[1]);
        else display((const char*)"Unknown command\n");
    }
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
    if(X == direction_of_interest)      direction_of_interest = Y;
    else if(Y == direction_of_interest) direction_of_interest = Z;
    else if(Z == direction_of_interest) direction_of_interest = TOTAL;
    else                                direction_of_interest = X;
}

int runTheApplication(void)
{
    while(1)
    {
        double accel_vals[4] = {0};
        bool first = true;

        if(next - getMillis() > period)
        {
            double x, y, z;
      
            readAccel_gs(&x, &y, &z);
            if(first)
            {
                accel_vals[X] = x;
                accel_vals[Y] = y;
                accel_vals[Z] = z;
                first = false;
            }
            else
            {
                accel_vals[X] = ewma_coeff*x + (1.0 - ewma_coeff)*accel_vals[X];
                accel_vals[Y] = ewma_coeff*y + (1.0 - ewma_coeff)*accel_vals[Y];
                accel_vals[Z] = ewma_coeff*z + (1.0 - ewma_coeff)*accel_vals[Z];
            }
      
            accel_vals[TOTAL] = sqrt(pow(accel_vals[X], 2) + pow(accel_vals[Y], 2) + pow(accel_vals[Z], 2));
            
            double motorSpeed = accel_vals[direction_of_interest]/max_accel;
            motorSpeed = motorSpeed >  1.0 ?  1.0 :
                         motorSpeed < -1.0 ? -1.0 : motorSpeed;
            setMotorSpeed(motorSpeed);

            double ledBrightness = accel_vals[TOTAL]/max_accel;
            ledBrightness = ledBrightness >  1.0 ?  1.0 :
                            ledBrightness < -1.0 ? -1.0 : ledBrightness;
            setLED(ledBrightness);
            
            next += period;
        }
    }
}