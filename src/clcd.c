#include "clcd.h"
#include <xc.h>

void clcd_write(unsigned char byte, unsigned char mode) {
  /* TODO 1: Set RS according to instruction/data mode. */
  CLCD_RS = mode;
  CLCD_DATA_PORT = byte;
  CLCD_EN = HI;
  __delay_us(100);
  CLCD_EN = LOW;
  __delay_us(4100);
  /* TODO 2: Put byte on the LCD data port. */
  /* TODO 3: Generate the enable pulse and required delays. */
}

static void init_display_controller(void) {

    __delay_ms(30);

    clcd_write(EIGHT_BIT_MODE,INST_MODE);
    __delay_ms(4100);
    clcd_write(EIGHT_BIT_MODE, INST_MODE);
    _delay(1);

    clcd_write(TWO_LINES_5x8_8_BIT_MODE, INST_MODE);
      __delay_us(100);
    clcd_write(CLEAR_DISP_SCREEN, INST_MODE);
    __delay_us(500);
    clcd_write(DISP_ON_AND_CURSOR_OFF, INST_MODE);
    __delay_us(100);
  /* TODO 4: Apply the LCD power-up delay and 8-bit initialization sequence. */
  /* TODO 5: Configure two-line display, clear display and cursor settings. */
}

void init_clcd(void) {
    CLCD_DATA_PORT_DDR = 0x00;
    CLCD_RS_DDR = 0;
    CLCD_EN_DDR = 0;
  /* TODO 6: Configure LCD data, RS and EN pins as outputs. */
  /* TODO 7: Initialize the display controller. */
}

void clcd_putch(const char data, unsigned char addr) {
  /* TODO 8: Set DDRAM address, then write one character. */
    clcd_write(addr, INST_MODE);   /* set DDRAM address */
    clcd_write(data, DATA_MODE);   /* write one character */
}

void clcd_print(const char *str, unsigned char addr) {
  /* TODO 9: Set DDRAM address and write the string until '\\0'. */
  clcd_write(addr, INST_MODE);   /* set DDRAM address */

    while (*str != '\0')
    {
        clcd_write(*str++, DATA_MODE);
    }
}
