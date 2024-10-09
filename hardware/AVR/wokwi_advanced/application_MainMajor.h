#ifndef APPLICATION_H
#define APPLICATION_H

typedef enum {X, Y, Z, TOTAL} direction_of_interest_t;

double getMaxAccel(void);
uint32_t getPeriod(void);
double getEwmaCoeff(void);
direction_of_interest_t getDirectionOfInterest(void);
void charReceivedCallback(char c);
void accelDoubleTapCallback(void);
void runTheApplication(void);

#endif /* APPLICATION_H */