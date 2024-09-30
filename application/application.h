#ifndef APPLICATION_H
#define APPLICATION_H

#include <stdint.h>			// For uint32_t

extern uint32_t period;

enum direction_of_interest_t {X, Y, Z, TOTAL};
extern enum direction_of_interest_t direction_of_interest;

extern double max_accel;

void accelDoubleTapCallback(void);

#endif /* APPLICATION_H */