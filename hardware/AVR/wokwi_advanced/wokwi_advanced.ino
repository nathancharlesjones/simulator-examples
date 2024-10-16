#include <Arduino.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include "application_MainMajor.h"

#define MAX_NUM_ARGS 10
#define ledPin 11
#define motorPin 3
#define btnPin 2

Adafruit_MPU6050 mpu;

// Returns elapsed system time in milliseconds.
//
uint32_t getMillis(void)
{
  return millis();
}

// Read the current x/y/z values (in units of g) from the attached accelerometer.
// Values should not include acceleration due to gravity.
//
void readAccel_gs(double* x, double* y, double* z)
{
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  *x = a.acceleration.x;
  *y = a.acceleration.y;
  *z = a.acceleration.z;
}

// Set the motor speed from 0 to MAX using double value from 0 to 1.0.
//
void setMotorSpeed(double speed)
{
  // Servo motor pulses: [1, 2] ms
  // Formula is on_time = 1 ms + speed (in ms)
  digitalWrite(motorPin, HIGH);
  delayMicroseconds(constrain(1500+(500*speed),1000,2000));
  digitalWrite(motorPin, LOW);
}

// Set the LED brightness using double value from 0 to 1.0.
//
void setLED(double brightness)
{
  analogWrite(ledPin, constrain(brightness * 255, 0, 255));
}

// Display a NUL-terminated string to the user
//
void display(const char * msg)
{
  Serial.print(msg);
}

static void parseString(char * buffer, uint8_t * argc, char ** argv, uint8_t sizeof_argv)
{
    char *rest;
    char *token = strtok_r(buffer, " \n", &rest);
    while (token != NULL && *argc <= sizeof_argv) {
        argv[(*argc)++] = token;
        token = strtok_r(NULL, " \n", &rest);
    }
}

static void processCommand(char * cmd_string)
{
    uint8_t argc = 0;
    char* argv[MAX_NUM_ARGS] = {0};
  
    parseString(cmd_string, &argc, argv, MAX_NUM_ARGS);
  
    if(strcmp(argv[0],"led") == 0) setLED(atof(argv[1]));
    else if(strcmp(argv[0],"speed") == 0) setMotorSpeed(atof(argv[1]));
    else if(strcmp(argv[0],"read") == 0)
    {
      sensors_event_t a, g, temp;
      mpu.getEvent(&a, &g, &temp);
      Serial.print("x = ");
      Serial.print(a.acceleration.x);
      Serial.print(" m/s^2 | y = ");
      Serial.print(a.acceleration.y);
      Serial.print(" m/s^2 | z = ");
      Serial.print(a.acceleration.z);
      Serial.print(" m/s^2\n");
    }
    else display((const char*)"Unknown command\n");
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  mpu.setAccelerometerRange(MPU6050_RANGE_2_G);

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  pinMode(motorPin, OUTPUT);
  digitalWrite(motorPin, LOW);

  pinMode(btnPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(btnPin), accelDoubleTapCallback, RISING);
}

void loop() {
  // put your main code here, to run repeatedly:
  runTheApplication();
  
  if(Serial.available())
  {
    static enum { START, APP_CMD, SIM_CMD } command_type_t = START;
    static String buffer;

    char c = Serial.read();

    if(command_type_t == START)
    {
      if(c == 'l' || c == 's' || c == 'r') command_type_t = SIM_CMD;
      else command_type_t = APP_CMD;
    }

    switch(command_type_t)
    {
      case APP_CMD:
        charReceivedCallback(c);
        if(c == '\n') command_type_t = START;
        break;
      case SIM_CMD:
        // Build string
        buffer += c;
        if(c == '\n')
        {
          processCommand(reinterpret_cast<char*>(&buffer[0]));
          command_type_t = START;
          buffer = "";
        }
        break;
    }
  }
}
