#ifndef TIMER_PWM_H
#define TIMER_PWM_H

void init_timer_pwm_lenkung(int dutyCycle);
void init_timer_pwm_antrieb(int dutyCycle);
void start_timer_pwm(void);
void set_timer_pwm_lenkung(int dutyCycle);
void set_timer_pwm_antrieb(int dutyCycle);

#endif