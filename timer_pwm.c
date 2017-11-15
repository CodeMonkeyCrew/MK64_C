#include <msp430.h>
#include "timer_pwm.h"
#include "util.h"


// timer PWM auf P4.1 -> LENKUNG
// timer PWM auf P4.2 -> ANTRIEB

void init_timer_pwm_lenkung(int value) {  
  // setup P4.1 (GPIO) as output
  P4DIR |= BIT1;
  
  // select P4.1 (GPIO) peripheral module function
  P4SEL |= BIT1;
  
  // 62500 ticks equals 20 ms (100Hz PWM frequency)
  TB0CCR0 = 62500;
  // 0 -> CCR1 auf 3125 (ganz links), 255 -> CCR1 auf 6250 (ganz rechts)
  set_timer_pwm_lenkung(map(value,0,255, 3125, 6250));
  // set/reset mode
  TB0CCTL1 = OUTMOD_3;
}

void init_timer_pwm_antrieb(int value) {  
  // setup P4.2 (GPIO) as output
  P4DIR |= BIT2;
  
  // select P4.2 (GPIO) peripheral module function
  P4SEL |= BIT2;
  
  // 62500 ticks equals 20 ms (100Hz PWM frequency)
  TB0CCR0 = 62500;
   // 0 -> CCR1 auf 3125 (vollgas rückwärts), 255 -> CCR1 auf 6250 (vollgas vorwärts)
  set_timer_pwm_antrieb(map(value,0,255, 3125, 6250));
  TB0CCTL2 = OUTMOD_3;
}

void start_timer_pwm() {
  // SMCLK (25MHz), divider 8, up mode, timer clear
  TB0CTL = TASSEL_2 + ID_3 + MC_1 + TACLR;
}

void set_timer_pwm_lenkung(int ticksHigh) {
  TB0CCR1 = ticksHigh;
}

void set_timer_pwm_antrieb(int ticksHigh) {
  TB0CCR2 = ticksHigh;
}