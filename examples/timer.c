#include <msp430.h>
#include "timer.h"

void timer(void) {
  // setup P1.0 (LED1) as output
  P1DIR |= BIT0;

  // intialize P1.0 (LED1) to low (off)
  P1OUT &= ~BIT0;
  
  TA0CCR0 = 0xFFFF;
  TA0CCTL0 = CCIE;
  TA0CTL = TASSEL_2 + ID_0 + MC_1 + TACLR;
  
  _BIS_SR(LPM0_bits + GIE);
}

/* uncomment this block before running this example
#pragma vector=TIMER0_A0_VECTOR
__interrupt static void timer_isr(void) {
  // clear timer interrupt flag
  TA0CTL &= ~TAIFG;
  // toggle P1.0 (LED)
  P1OUT ^= BIT0;
}
*/