#include <xc.h>
#include "main.h"

/* Shared with the password/login implementation. */
unsigned char sec, return_time;

void __interrupt() isr(void)
{
    static unsigned int count = 0;

    /* TODO 1: Check whether the Timer2 interrupt flag is set. */
    /* TODO 2: Count timer interrupts to produce a one-second interval. */
    /* TODO 3: Decrement sec or return_time at the correct time. */
    /* TODO 4: Clear the Timer2 interrupt flag before leaving the ISR. */
}
