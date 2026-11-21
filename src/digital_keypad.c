#include "digital_keypad.h"
#include <xc.h>

void init_digital_keypad(void) {
  KEYPAD_PORT = 1;
  KEYPAD_PORT_DDR = 1;
}

unsigned char read_digital_keypad(unsigned char mode) {
  static unsigned char once = 1;
  unsigned char key = PORTB & INPUT_LINES;

  if (mode == LEVEL_DETECTION) {
    return key;
  }
  else{
    if(key != INPUT_LINES&&once){
        once=0;
        return key;
    }
    else if(key == INPUT_LINES){
        once =1;
    }
  }
  return INPUT_LINES;
}
