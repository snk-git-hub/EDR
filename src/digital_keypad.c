#include "digital_keypad.h"
#include <xc.h>

void init_digital_keypad(void) {
  KEYPAD_PORT_DDR |= INPUT_LINES;   /* RB0..RB5 must be inputs (was TRISB = 1) */
  OPTION_REG &= 0x7F;               /* nRBPU = 0: weak pull-ups on PORTB */
}

unsigned char read_digital_keypad(unsigned char mode) {
  static unsigned char once = 1;
  unsigned char key = PORTB & INPUT_LINES;

  if (mode == LEVEL_DETECTION) {
    return key;
  } else {
    if (key != INPUT_LINES && once) {
      once = 0;
      return key;
    } else if (key == INPUT_LINES) {
      once = 1;
    }
  }
  return INPUT_LINES;
}
