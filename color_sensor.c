
#include <msp430.h>
#include <inttypes.h>
#include <stdio.h>
#include "color_sensor.h"

#define FALSE 0
#define TRUE !FALSE
#define NUMBER_OF_COLORS 4

// order of values reflects sensor color table
typedef enum color {
  red,
  blue,
  clear,
  green
} Color_t;

static Color_t current_color;
static uint8_t color_values[NUMBER_OF_COLORS];

static int overflow;
static uint16_t previous_timer_value;
static uint16_t timer_ticks_arr[100];
static int cursor;

// measured average values for black and white
// row index = color, row[0] = min, row[1] = max
static int colorRanges[NUMBER_OF_COLORS][2] = {
  {85, 221},
  {629, 205},
  {453, 73},
  {341, 249}
};

void swap(int* x, int* y) {
  int tmp = *x;
  *x = *y;
  *y = tmp;
}

int map(int x, int in_min, int in_max, int out_min, int out_max) {
  int tmp;
  if (in_min > in_max) {
    // swap in_min and in_max
    tmp = in_min;
    in_min = in_max;
    in_max = tmp;
  }
  if (out_min > out_max) {
    // swap out_min and out_max
    tmp = out_min;
    out_min = out_max;
    out_max = tmp;
  }
  
  int res = (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
  if (res > out_max) {
    return out_max;
  }
  if (res < out_min) {
    return out_min;
  }
  return res;
}

static void set_color(Color_t color) {
  if (color & BIT0) {
    // set S3 to high
    P1OUT |= BIT2;
  } else {
    // set S3 to low
    P1OUT &= ~BIT2;
  }
  if (color & BIT1) {
    // set S2 to high
    P2OUT |= BIT0;
  } else {
    // set S2 to low
    P2OUT &= ~BIT0;
  }
  current_color = color;
}

void color_sensor_init(void) {
  // setup P2.4 (S0), P2.2 (S1) and P2.0 (S2) as output
  P2DIR |= BIT4 + BIT2 + BIT0;
  
  // setup P1.2 (S3) as output
  P1DIR |= BIT2;
  
  // setup P1.1 (OUT) as input
  P1DIR &= ~BIT1;
  
  // setup P2.1 (OE) as output
  P2DIR |= BIT1;
  
  // set frequency scaling to 100% (S0 to high, S1 to high)
  P2OUT |= BIT4;
  P2OUT |= BIT2;
  
  // set OE to low
  P2OUT &= ~BIT1;
  
  // select peripheral module function (CCI0A) for P1.1 (OUT)
  P1SEL |= BIT1;
  
  // capture on rising edge, select CCIxA input, synchronize capture source, capture mode, capture/compare interrupt enable
  TA0CCTL0 = CM_1 + CCIS_0 + SCS + CAP + CCIE;
  
  // SMCLK, divider 0, continuous mode, timer_clear
  TA0CTL = TASSEL_2 + ID_0 + MC_2 + TACLR;
  
  set_color(green);
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void timer_isr(void) {
  // check for timer overflow
  if (TA0CTL & TAIFG) {
    overflow = TRUE;
    // clear timer interrupt flag
    TA0CTL &= ~TAIFG;
  } else {
    uint16_t current_timer_value = TA0CCR0;
    uint16_t timer_ticks = 0;
    if (overflow) {
      timer_ticks = current_timer_value + (0xFFFF - previous_timer_value);
      overflow = FALSE;
    } else {
      timer_ticks = current_timer_value - previous_timer_value;
    }
    // TODO: calculate color value
    // color_values[current_color] = get_color_value(timer_ticks);
    if (current_color == (NUMBER_OF_COLORS - 1)) {
      // TODO: notify listeners
    }
    // next color
    // set_color((Color_t) (++current_color % NUMBER_OF_COLORS));

    if (cursor < 100) {
      timer_ticks_arr[cursor++] = timer_ticks;
    } else {
      unsigned int sum = 0;
      int i;
      for(i = 1; i < cursor; ++i) {
        sum += timer_ticks_arr[i];
      }
      printf("%i mapped to %i\n", sum / 99, map(sum / 99, colorRanges[current_color][0], colorRanges[current_color][1], 0, 255));
      cursor = 0;
    }
    // overwrite previous timer value
    previous_timer_value = current_timer_value;
  }
}



