#include "main.h"
#include "uart.h"

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 20000000UL
#endif

void init_uart(unsigned long baud)
{
    TRISC6 = 0;                                        /* TX */
    TRISC7 = 1;                                        /* RX */
    SPBRG = (unsigned char)((_XTAL_FREQ / (16UL * baud)) - 1);
    TXSTA = 0x24;                                      /* TXEN, BRGH */
    RCSTA = 0x90;                                      /* SPEN, CREN */
}

void uart_putch(unsigned char data)
{
    while (!TXIF)
        ;
    TXREG = data;
}

void uart_puts(const char *s)
{
    while (*s)
    {
        uart_putch(*s++);
    }
}
