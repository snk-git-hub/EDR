#include <xc.h>
#include "digital_keypad.h"

void init_digital_keypad(void)
{
    /* TODO 1: Configure the keypad switch pins as inputs. */
}

unsigned char read_digital_keypad(unsigned char mode)
{
    /* TODO 2: For LEVEL_DETECTION, return the currently pressed switch. */
    /* TODO 3: For STATE_DETECTION, report a press only once until release. */
    /* TODO 4: Return ALL_RELEASED when no new key press is detected. */
    return ALL_RELEASED;
}
