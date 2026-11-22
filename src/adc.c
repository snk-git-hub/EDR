#include "adc.h"
#include <xc.h>

void init_adc(void) {
  ADFM = 1;
  ADON = 1;
}

unsigned short read_adc(unsigned char channel) {
  unsigned short adc_reg_val;
  ADCON0 = (ADCON0 & 0xC7) | (channel << 3);
  GO = 1;
  while (nDONE)
    ;

  adc_reg_val = (ADRESH << 8) | ADRESL;

  return adc_reg_val;
}
