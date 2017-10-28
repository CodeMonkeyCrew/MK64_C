#include <msp430.h>
#include "timer_pwm.h"
#include "color_sensor.h"

void test_lenkung(void) {
  volatile int i, j;
  for(i = 128; i < 256; ++i) {
    set_timer_pwm_lenkung(i);
    for(j = 0; j < 20000; ++j); // delay
  }
  while(1) {
    for(i = 255; i >= 0; --i) {
      set_timer_pwm_lenkung(i);
      for(j = 0; j < 20000; ++j); // delay
    }
    for(i = 0; i < 256; ++i) {
      set_timer_pwm_lenkung(i);
      for(j = 0; j < 20000; ++j); // delay
    }
  }
}

void test_antrieb(void) {
  volatile int i, j;
  while(1) {
    for(i = 0; i < 256; ++i) {
      set_timer_pwm_antrieb(i);
      for(j = 0; j < 10000; ++j); // delay
    }
    for(i = 255; i >= 0; --i) {
      set_timer_pwm_antrieb(i);
      for(j = 0; j < 10000; ++j); // delay
    }
  }
}

int main( void )
{
  WDTCTL = WDTPW + WDTHOLD;

  //init_timer_pwm_lenkung(0xFF);
  //init_timer_pwm_antrieb(125); // 4 khz pwm
  //init_timer_pwm_antrieb(250); // 2 khz pwm
  //init_timer_pwm_antrieb(500); // 1 khz pwm
  //start_timer_pwm();
  //test_antrieb();
  
  // initialize color sensor
  color_sensor_init();

  _BIS_SR(LPM0_bits + GIE);
}