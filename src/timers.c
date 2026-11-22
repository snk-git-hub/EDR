#include "timers.h"
#include <xc.h>

void init_timer2(void) {
  T2CON = 0x00;
  TMR2 = 0x00;
  PR2 = 249;
  T2CONbits.T2CKPS = 0b11;     /* 1:16 prescaler */
  T2CONbits.TOUTPS = 0b0000;   /* 1:1 postscaler */

  PIR1bits.TMR2IF = 0;
  PIE1bits.TMR2IE = 1;         /* enable Timer2 interrupt */
  INTCONbits.PEIE = 1;         /* peripheral interrupts   */
  INTCONbits.GIE = 1;          /* global interrupts       */

  T2CONbits.TMR2ON = 1;
}