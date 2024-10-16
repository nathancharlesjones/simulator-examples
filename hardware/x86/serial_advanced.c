#include <time.h>           // For clock
#include <pthread.h>        // For pthread_t, pthread_create
#include <stdio.h>          // For printf, scanf
#include <string.h>         // For strtok_r
#include <stdlib.h>         // For atof, atoi
#include <stdint.h>         // For uint32_t
#include <stdbool.h>        // For bool, true, false
#include <termios.h>
#include <fcntl.h> // Contains file controls like O_RDWR
#include <errno.h> // Error integer and strerror() function
#include <unistd.h> // write(), read(), close()
#include "application.h"

#define MAX_NUM_ARGS 10
#define MAX_INPUT_CHARS 64

int serial_port;
struct termios tty;
pthread_t h_readThread;
void* readThread(void* data);
double curr_x = 0.0, curr_y = 0.0, curr_z = 0.0;
bool new_vals = false;

void initHardware(int argc, char ** argv)
{
    if(argc > 1) serial_port = open(argv[1], O_RDWR);
    else
    {
        printf("ERROR: Serial port not given\n");
        exit(EXIT_FAILURE);
    }
    
    if (serial_port < 0)
    {
        printf("Error %i from open: %s\n", errno, strerror(errno));
    }
    
    if(tcgetattr(serial_port, &tty) != 0)
    {
        printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    pthread_create(&h_readThread, NULL, readThread, NULL);
}

void* readThread(void* data)
{
    int argc = 0;
    char *argv[MAX_NUM_ARGS] = {0};

    while(1)
    {
        uint8_t idx = 0;
        char buffer[MAX_INPUT_CHARS+1] = {0};
        char c = '\0';
        read(serial_port, &c, 1);
        if(c != 'a' && c != 't')            // Message meant for application code
        {
            charReceivedCallback(c);
            do
            {
                read(serial_port, &c, 1);
                charReceivedCallback(c);
            } while(c != '\n');
        }
        else                                // Message meant to simulate input
        {
            while(c != '\n' && idx < MAX_INPUT_CHARS)
            {
                buffer[idx++] = c;
                read(serial_port, &c, 1);
            }

            // Tokenize
            char *rest;
            char *token = strtok_r(buffer, " \n", &rest);
            while (token != NULL && argc < MAX_NUM_ARGS)
            {
              argv[argc++] = token;
              token = strtok_r(NULL, " \n", &rest);
            }

            switch(argv[0][0])
            {
                case 'a':
                    curr_x = atof(argv[1]);
                    curr_y = atof(argv[2]);
                    curr_z = atof(argv[3]);
                    new_vals = true;
                    break;
                case 't':
                    accelDoubleTapCallback();
                    break;
            }

            argc = 0;
        }
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
    // Send out "r" command to request updated accel values
    // Wait until new_vals is true (which is set above when
    // the GUI sends new accel values), or send out "r" again
    // after 1 sec if nothing is received.
    do
    {
        uint32_t start = getMillis();
        write(serial_port, "r\n", 2);
        while(!new_vals && (start + 1000 > getMillis()));
    } while(!new_vals);
    *x = curr_x;
    *y = curr_y;
    *z = curr_z;
    new_vals = false;
}

void setMotorSpeed(double speed)
{
    char buffer[32] = {0};
    sprintf(buffer, "m %lf\n", speed);
    write(serial_port, buffer, strlen(buffer));
}

void setLED(double brightness)
{
    char buffer[32] = {0};
    sprintf(buffer, "l %lf\n", brightness);
    write(serial_port, buffer, strlen(buffer));
}

void display(const char * msg)
{
    //printf("%s", msg);
}