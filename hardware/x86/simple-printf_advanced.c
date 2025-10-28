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

pthread_t h_scanfPthread;
void* scanfPthread(void* data);
double curr_x = 0.0, curr_y = 0.0, curr_z = 0.0;
bool rand_accels = false;
double volatility = 0.2;
struct timespec start;

void initHardware(int argc, char ** argv)
{
    srand(time(NULL));
    clock_gettime(CLOCK_MONOTONIC, &start);
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

        // Reset string and clear stdin buffer
        while ((c = getchar()) != '\n' && c != EOF);
    }
}

uint32_t getMillis(void)
{
    struct timespec time;
    clock_gettime(CLOCK_MONOTONIC, &time);
    return (uint32_t)(((time.tv_sec-start.tv_sec)*1000) + ((time.tv_nsec-start.tv_nsec)/1000000));
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
    static uint32_t period = 1000;

    if(next - getMillis() > period)
    {
        printf("Setting motor speed: %lf\n", speed);
        next += period;
    }
}

void setLED(double brightness)
{
    static uint32_t next = -1;
    static uint32_t period = 1000;

    if(next - getMillis() > period)
    {
        printf("Setting LED brightness: %lf\n", brightness);
        next += period;
    }
}

void display(const char * msg)
{
    printf("%s", msg);
}