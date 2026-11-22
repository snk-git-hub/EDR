#include "adc.h"
#include <xc.h>

void init_adc(void) {
  /* ADFM=1 (right justified), PCFG=1001: AN0..AN5 analog, AN6/AN7 digital.
     RE1/RE2 drive the LCD (EN/RS) and must stay digital, otherwise
     read-modify-write on PORTE corrupts them. */
  ADCON1 = 0x89;
  /* Fosc/32 conversion clock (1.6 us @ 20 MHz), module on */
  ADCON0 = 0x81;
}

unsigned short read_adc(unsigned char channel) {
  unsigned short adc_reg_val;
  ADCON0 = (ADCON0 & 0xC7) | (channel << 3);
  __delay_us(30);                  /* acquisition time */
  GO = 1;
  while (nDONE)
    ;

  adc_reg_val = (ADRESH << 8) | ADRESL;

  return adc_reg_val;
}
