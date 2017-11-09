#include <msp430.h>
#include "timer_pwm.h"
#include "color_sensor.h"
#include <stdio.h>

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

void SetVCoreUp (unsigned int level) {
  // Open PMM registers for write access
  PMMCTL0_H = PMMPW_H;
  // Make sure no flags are set for iterative sequences
  // while ((PMMIFG & SVSMHDLYIFG) == 1);
  // while ((PMMIFG & SVSMLDLYIFG) == 1);
  // Set SVS/SVM high side new level
  SVSMHCTL = SVSHE + SVSHRVL0 * level + SVMHE + SVSMHRRL0 * level;
  // Set SVM low side to new level
  SVSMLCTL = SVSLE + SVMLE + SVSMLRRL0 * level;
  // Wait till SVM is settled
  while ((PMMIFG & SVSMLDLYIFG) == 0);
  // Clear already set flags
  PMMIFG &= ~(SVMLVLRIFG + SVMLIFG);
  // Set VCore to new level
  PMMCTL0_L = PMMCOREV0 * level;
  // Wait till new level reached
  if ((PMMIFG & SVMLIFG))
  while ((PMMIFG & SVMLVLRIFG) == 0);
  // Set SVS/SVM low side to new level
  SVSMLCTL = SVSLE + SVSLRVL0 * level + SVMLE + SVSMLRRL0 * level;
  // Lock PMM registers for write access
  PMMCTL0_H = 0x00;
}

int main( void )
{
  // stop watchdog timer
  WDTCTL = WDTPW + WDTHOLD;
  
  // change core voltage one level at a time
  SetVCoreUp(1);
  SetVCoreUp(2);
  SetVCoreUp(3);
  __delay_cycles(800000);

  // USC module configuration, Fdcoclockdiv = Fmclk = 25MHz
  UCSCTL8 &= ~SMCLKREQEN;         // disable SMCLK clock requests
  UCSCTL3 = (0 * FLLREFDIV0)      // FLL ref divider 1
          + SELREF2;              // set REFOCLK as FLL reference clock source
  UCSCTL4 = SELA__REFOCLK         // ACLK = REFO
          + SELM__DCOCLKDIV       // MCLK = DCOCLKDIV
          + SELS__DCOCLKDIV;      // SMCLK = DCOCLKDIV
  __bis_SR_register(SCG0);        // disable FLL operation
  UCSCTL0 = 0x0000;               // lowest possible DCO, MOD
  UCSCTL1 = DISMOD_L              // modulation disabled
          + DCORSEL_7;            // DCO range for 25MHz operation
  UCSCTL2 = FLLD_0                // D=FLLD=1, so that Fdco=25MHz
          + 761;                  // DCO multiplier for 25MHz
                                  // (N + 1) * FLLRef = Fdcodiv
                                  // (761 + 1) * 32768 = 25MHz (multiplier N = 761)
  __bic_SR_register(SCG0);        // re-enable FLL operation
  
  // worst-case settling time for the DCO when the DCO range bits have been
  // 32 x 32 x 25 MHz / 32,768 Hz = 781250 = MCLK cycles for DCO to settle
  __delay_cycles(781250);

  //init_timer_pwm_lenkung(0xFF);
  //init_timer_pwm_antrieb(125); // 4 khz pwm
  //init_timer_pwm_antrieb(250); // 2 khz pwm
  //init_timer_pwm_antrieb(500); // 1 khz pwm
  //start_timer_pwm();
  //test_antrieb();
  
  // initialize color sensor
  color_sensor_init();

  //_BIS_SR(LPM0_bits + GIE);
  __enable_interrupt();
  while(1) {
    // printf("smclkoff %i\n", UCSCTL6 & BIT1);
    //printf("sel %i\n", UCSCTL1 & 0x70);
  }
}