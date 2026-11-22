#include "clcd.h"
#include <xc.h>

void clcd_write(unsigned char byte, unsigned char mode) {
  CLCD_RS = mode;
  CLCD_DATA_PORT = byte;
  CLCD_EN = HI;
  __delay_us(100);
  CLCD_EN = LOW;
  __delay_us(100);          /* enough for normal commands/data */
}

static void init_display_controller(void) {

  __delay_ms(30);           /* LCD power-up time */

  clcd_write(EIGHT_BIT_MODE, INST_MODE);
  __delay_ms(5);            /* was 4100 ms by mistake */
  clcd_write(EIGHT_BIT_MODE, INST_MODE);
  __delay_us(200);

  clcd_write(TWO_LINES_5x8_8_BIT_MODE, INST_MODE);
  __delay_us(100);
  clcd_write(CLEAR_DISP_SCREEN, INST_MODE);
  __delay_ms(2);            /* clear needs ~1.6 ms */
  clcd_write(DISP_ON_AND_CURSOR_OFF, INST_MODE);
  __delay_us(100);
}

void init_clcd(void) {
  /* Pins must be outputs BEFORE talking to the LCD */
  CLCD_DATA_PORT_DDR = 0x00;
  CLCD_RS_DDR = 0;
  CLCD_EN_DDR = 0;

  init_display_controller();
}

void clcd_putch(const char data, unsigned char addr) {
  clcd_write(addr, INST_MODE);
  clcd_write(data, DATA_MODE);
}

void clcd_print(const char *str, unsigned char addr) {
  clcd_write(addr, INST_MODE);
  while (*str != '\0') {
    clcd_write(*str++, DATA_MODE);
  }
}