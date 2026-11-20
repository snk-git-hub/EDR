#include <xc.h>
#include "clcd.h"

void clcd_write(unsigned char byte, unsigned char mode)
{
    /* TODO 1: Set RS according to instruction/data mode. */
    /* TODO 2: Put byte on the LCD data port. */
    /* TODO 3: Generate the enable pulse and required delays. */
}

static void init_display_controller(void)
{
    /* TODO 4: Apply the LCD power-up delay and 8-bit initialization sequence. */
    /* TODO 5: Configure two-line display, clear display and cursor settings. */
}

void init_clcd(void)
{
    /* TODO 6: Configure LCD data, RS and EN pins as outputs. */
    /* TODO 7: Initialize the display controller. */
}

void clcd_putch(const char data, unsigned char addr)
{
    /* TODO 8: Set DDRAM address, then write one character. */
}

void clcd_print(const char *str, unsigned char addr)
{
    /* TODO 9: Set DDRAM address and write the string until '\\0'. */
}
