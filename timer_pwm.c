#include <msp430.h>
#include "timer_pwm.h"

// timer PWM auf P4.1 -> LENKUNG
// timer PWM auf P4.2 -> ANTRIEB

void init_timer_pwm_lenkung(uint8_t value) {  
  // setup P4.1 (GPIO) as output
  P4DIR |= BIT1;
  
  // select P4.1 (GPIO) peripheral module function
  P4SEL |= BIT1;
  
  // 655 ticks equals 20 ms (50Hz PWM frequency)
  TB0CCR0 = 655;
  // 0 -> CCR1 auf 32 (ganz links), 255 CCR1 auf 64 (ganz rechts)
  set_timer_pwm_lenkung(value);
  // reset/set mode
  TB0CCTL1 = OUTMOD_7;
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
  // ACLK (32768 Hz), divider 0, up mode, timer clear
  TB0CTL = TASSEL_1 + ID_0 + MC_1 + TACLR;
}

void set_timer_pwm_lenkung(uint8_t ticksHigh) {
  TB0CCR1 = ticksHigh;
}

void set_timer_pwm_antrieb(int dutyCycle) {
  TB0CCR2 = dutyCycle;
}