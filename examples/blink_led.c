#include <msp430.h>
#include "blink_led.h"

#define FALSE 0
#define TRUE !FALSE

void blink_led(void) {
  // setup P1.0 (LED1) as output
  P1DIR |= BIT0;

  // intialize P1.0 (LED1) to low (off)
  P1OUT &= ~BIT0;

  while (TRUE) {
    // toggle P1.0 (LED1)
    P1OUT ^= BIT0;
    volatile int i;
    for (i = 0; i < 20000; ++i);
  }
}