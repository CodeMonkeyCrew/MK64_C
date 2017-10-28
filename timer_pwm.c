#include <msp430.h>
#include "timer_pwm.h"

// timer PWM auf P4.1 -> LENKUNG
// timer PWM auf P4.2 -> ANTRIEB

void init_timer_pwm_lenkung(int dutyCycle) {  
  // setup P4.1 (GPIO) as output
  P4DIR |= BIT1;
  
  // select P4.1 (GPIO) peripheral module function
  P4SEL |= BIT1;
  
  TB0CCR0 = 0xFFFF;
  set_timer_pwm_lenkung(dutyCycle);
  TB0CCTL1 = OUTMOD_3;
}

void init_timer_pwm_antrieb(int dutyCycle) {  
  // setup P4.2 (GPIO) as output
  P4DIR |= BIT2;
  
  // select P4.2 (GPIO) peripheral module function
  P4SEL |= BIT2;
  
  TB0CCR0 = dutyCycle * 2;
  set_timer_pwm_antrieb(dutyCycle);
  TB0CCTL2 = OUTMOD_3;
}

void start_timer_pwm() {
  TB0CTL = TASSEL_2 + ID_0 + MC_1 + TACLR;
}

void set_timer_pwm_lenkung(int dutyCycle) {
  TB0CCR1 = dutyCycle;
}

void set_timer_pwm_antrieb(int dutyCycle) {
  TB0CCR2 = dutyCycle;
}