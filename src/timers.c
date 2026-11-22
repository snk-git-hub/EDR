#include "timers.h"
#include <xc.h>

void init_timer2(void) {
  /* TODO 1: Configure Timer2 prescaler. */
  T2CON = 0x00;
  TMR2 = 0x00;
  PR2 = 249;
  T2CONbits.T2CKPS = 0b11;

  T2CONbits.TOUTPS = 0b0000;

  PIR1bits.TMR1IF = 0;
  T2CONbits.TMR2ON = 1;
 
}
