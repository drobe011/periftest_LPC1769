#ifndef PWM_H
#define PWM_H

#define KHZ(x) x * 100

volatile int updatePWMTimer = 0;

void setupPWM(uint32_t period, uint32_t onTime);

#endif