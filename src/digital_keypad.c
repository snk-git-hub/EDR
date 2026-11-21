#include "digital_keypad.h"
#include <xc.h>

void init_digital_keypad(void) {
  /* TODO 1: Configure the keypad switch pins as inputs. */
  KEYPAD_PORT = 1;
  KEYPAD_PORT_DDR = 1;
}

unsigned char read_digital_keypad(unsigned char mode) {
  /* TODO 2: For LEVEL_DETECTION, return the currently pressed switch. */
  /* TODO 3: For STATE_DETECTION, report a press only once until release. */
  /* TODO 4: Return ALL_RELEASED when no new key press is detected. */
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
