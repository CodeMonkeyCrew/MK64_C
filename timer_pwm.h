#ifndef TIMER_PWM_H
#define TIMER_PWM_H

#include <inttypes.h>

void init_timer_pwm_lenkung(uint8_t value);
void init_timer_pwm_antrieb(int dutyCycle);
void start_timer_pwm(void);
void set_timer_pwm_lenkung(uint8_t value);
void set_timer_pwm_antrieb(int dutyCycle);

#endif