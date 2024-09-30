#ifndef HARDWARE_H
#define HARDWARE_H

// Initialize required hardware for the project, including installing
// "accelDoubleTapCallback" as callback for when accelerometer detects
// a double-tap.
//  - x86: pthreads, ncurses, etc
//  - MCU: Accelerometer (I2C? SPI?), Motor/LED/Accel interrupt (GPIO)
//
void initHardware(int argc, char ** argv);

// Returns elapsed system time in milliseconds.
//
uint32_t getMillis(void);

// Read the current x/y/z values (in units of g) from the attached accelerometer.
// Values should not include acceleration due to gravity.
//
void readAccel_gs(double* /* x */, double* /* y */, double* /* z */);

// Set the motor speed from 0 to MAX using double value from 0 to 1.0.
//
void setMotorSpeed(double /* speed */);

// Set the LED brightness using double value from 0 to 1.0.
//
void setLED(double /* brightness */);

// Display a NUL-terminated string to the user
//
void display(const char * /* msg */);

#endif /* HARDWARE_H */