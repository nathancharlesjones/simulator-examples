#include <time.h>           // For clock
#include <pthread.h>        // For pthread_t, pthread_create
#include <stdio.h>          // For printf, scanf
#include <string.h>         // For strtok_r
#include <stdlib.h>         // For atof, atoi
#include <stdint.h>         // For uint32_t
#include <stdbool.h>        // For bool, true, false
#include "application.h"

#define MAX_NUM_ARGS 10
#define MAX_INPUT_CHARS 64

char background[] = "+-----------------------------------------------------+\n\r"
                    "|X:              %12lf"     "                         |\n\r"
                    "|Y:              %12lf"     "                         |\n\r"
                    "|Z:              %12lf"     "                         |\n\r"
                    "|Values generated: %10s"    " | Volatility: %8lf"  "  |\n\r"
                    "+-----------------------------------------------------+\n\r"
                    "|Max accel:      %12lf"     "                         |\n\r"
                    "|Motor speed:    %12lf"     "                         |\n\r"
                    "|LED brightness: %12lf"     "                         |\n\r"
                    "+-----------------------------------------------------+\n\r"
                    "|Application commands:                                |\n\r"
                    "| 't': Toggle which direction is used to control the  |\n\r"
                    "|      motor speed (x -> y -> z -> total -> x)        |\n\r"
                    "| 'm <double>': Set max accel value to <double> (used |\n\r"
                    "|      to scale motor speed and LED brightness)       |\n\r"
                    "| 'p <int>': Set main task period to <int> ms (min: 1)|\n\r"
                    "| 'w <double>': Set coefficient for the moving average|\n\r"
                    "|      of x/y/z (0: Heavy averaging; 1: No averaging) |\n\r"
                    "|Simulator commands:                                  |\n\r"
                    "| 'x <double>': Set x accelerometer value to <double> |\n\r"
                    "| 'y <double>': Set y accelerometer value to <double> |\n\r"
                    "| 'z <double>': Set z accelerometer value to <double> |\n\r"
                    "| 'r': Toggle setting x/y/z randomly or manually      |\n\r"
                    "| 'v <double>': Set the 'volatility' of the randomly  |\n\r"
                    "|      generated numbers (x = x + rand(-1,1)*vol)     |\n\r"
                    "+-----------------------------------------------------+\n\r";

pthread_t h_scanfPthread;
void* scanfPthread(void* data);
double curr_x = 0.0, curr_y = 0.0, curr_z = 0.0, curr_motor_speed = 0.0, curr_led_brightness = 0.0;
bool rand_accels = false;
double volatility = 0.2;

void updateDisplay(void)
{
    printf("\033[;H");      // Send cursor to top-left corner
    printf(background, curr_x, curr_y, curr_z, rand_accels ? "Randomly" : "Manually", volatility, getMaxAccel(), curr_motor_speed, curr_led_brightness);
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
        char c = getchar();
        if(c != 'x' && c != 'y' && c != 'z' && c != 't' && c != 'r' && c != 'v')            // Message meant for application code
        {
            while(c != '\n' && strlen(buffer) < MAX_INPUT_CHARS)
            {
                charReceivedCallback(c);
                c = getchar();
            }
            charReceivedCallback(c);
        }
        else                                // Message meant to simulate input
        {
            ungetc(c, stdin);
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
                case 'r':
                    rand_accels = !rand_accels;
                    break;
                case 'v':
                    volatility = atof(argv[1]);
                    break;
            }

            argc = 0;
        }
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
        curr_x += volatility*(((double)rand()/RAND_MAX)*2 - 1);
        curr_y += volatility*(((double)rand()/RAND_MAX)*2 - 1);
        curr_z += volatility*(((double)rand()/RAND_MAX)*2 - 1);
    }
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