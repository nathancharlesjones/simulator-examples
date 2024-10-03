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
  if(X == direction_of_interest)    direction_of_interest = Y;
  else if(Y == direction_of_interest) direction_of_interest = Z;
  else if(Z == direction_of_interest) direction_of_interest = TOTAL;
  else                direction_of_interest = X;
}

int main(int argc, char ** argv)
{
  initHardware(argc, argv);

  while(1)
  {
    if(next - getMillis() > period)
    {
      static bool first = true;
      double accel, total, x, y, z;
      double x_avg, y_avg, z_avg;

      readAccel_gs(&x, &y, &z);
      if(first)
      {
        x_avg = x;
        y_avg = y;
        z_avg = z;
        first = false;
      }
      else
      {
        x_avg = ewma_coeff*x + (1.0 - ewma_coeff)*x_avg;
        y_avg = ewma_coeff*y + (1.0 - ewma_coeff)*y_avg;
        z_avg = ewma_coeff*z + (1.0 - ewma_coeff)*z_avg;
      }

      total = sqrt(pow(x_avg, 2) + pow(y_avg, 2) + pow(z_avg, 2));
      
      switch(direction_of_interest)
      {
        case X:
          accel = x_avg;
          break;
        case Y:
          accel = y_avg;
          break;
        case Z:
          accel = z_avg;
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