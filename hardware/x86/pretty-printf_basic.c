#include <time.h>           // For clock
#include <pthread.h>        // For pthread_t, pthread_create
#include <stdio.h>          // For printf, scanf
#include <string.h>         // For strtok_r
#include <stdlib.h>         // For atof, atoi
#include <stdint.h>         // For uint32_t
#include "application.h"

#define MAX_NUM_ARGS 10
#define MAX_INPUT_CHARS 64

char background[] = "+-----------------------------------------------------+\n\r"
                    "|Motor speed:    %12lf"     "                         |\n\r"
                    "|LED brightness: %12lf"     "                         |\n\r"
                    "+-----------------------------------------------------+\n\r"
                    "|Application commands:                                |\n\r"
                    "| 't': Toggle which direction is used to control the  |\n\r"
                    "|      motor speed (x -> y -> z -> total -> x)        |\n\r"
                    "|Simulator commands:                                  |\n\r"
                    "| 'x <double>': Set x accelerometer value to <double> |\n\r"
                    "| 'y <double>': Set y accelerometer value to <double> |\n\r"
                    "| 'z <double>': Set z accelerometer value to <double> |\n\r"
                    "+-----------------------------------------------------+\n\r";

pthread_t h_scanfPthread;
void* scanfPthread(void* data);
double curr_x = 0.0, curr_y = 0.0, curr_z = 0.0, curr_motor_speed = 0.0, curr_led_brightness = 0.0;

void updateDisplay(void)
{
    printf("\033[;H");      // Send cursor to top-left corner
    printf(background, curr_motor_speed, curr_led_brightness);
}

void initHardware(int argc, char ** argv)
{
    printf("\033[2J\033[;H");       // Clear terminal
    updateDisplay();
    pthread_create(&h_scanfPthread, NULL, scanfPthread, NULL);
}

void* scanfPthread(void* data)
{
    int argc = 0;
    char *argv[MAX_NUM_ARGS] = {0};

    while(1)
    {
        char buffer[MAX_INPUT_CHARS+1] = {0};
        scanf("%[^\n]", buffer);        // Read until newline is reached

        // Tokenize
        char *rest;
        char *token = strtok_r(buffer, " \n", &rest);
        while (token != NULL)
        {
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
        }

        // Reset string and clear stdin buffer
        argc = 0;
        char c;
        while ((c = getchar()) != '\n' && c != EOF);
    }
}

uint32_t getMillis(void)
{
    struct timespec time;
    clock_gettime(CLOCK_MONOTONIC, &time);
    return (uint32_t)((time.tv_sec*1000) + (time.tv_nsec/1000000));
}

void readAccel_gs(double* x, double* y, double* z)
{
    *x = curr_x;
    *y = curr_y;
    *z = curr_z;
}

void setMotorSpeed(double speed)
{
    static uint32_t next = -1;
    static uint32_t period = 100;

    if(next - getMillis() > period)
    {
        curr_motor_speed = speed;
        updateDisplay();
        next += period;
    }
}

void setLED(double brightness)
{
    static uint32_t next = -1;
    static uint32_t period = 100;

    if(next - getMillis() > period)
    {
        curr_led_brightness = brightness;
        updateDisplay();
        next += period;
    }
}

void display(const char * msg)
{
    printf("%s", msg);
}