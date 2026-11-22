#include "main.h"
#include <string.h>
#include <xc.h>

#pragma config WDTE = OFF

char *gear[] = {"GN", "GR", "G1", "G2", "G3", "G4"};

static void init_config(void) {
  init_clcd();
  init_digital_keypad();
  init_adc();
  init_ds1307();
  init_i2c(unsigned long baud);
  init_timer2();
}

void log_power_on_event(void) {
  unsigned char sec = read_ds1307(0x00);
  unsigned char min = read_ds1307(0x01);
  unsigned char hour = read_ds1307(0x02);
  unsigned char event;

  event = ON;

  write_internal_eeprom(0x00, sec);

  write_internal_eeprom(0x01, min);

  write_internal_eeprom(0x02, hour);
}
void display_event_log(void) {
  unsigned char sec, min, hour, event;

  sec = read_internal_eeprom(0x00);
  min = read_internal_eeprom(0x01);
  hour = read_internal_eeprom(0x02);
  event = read_internal_eeprom(0x03);
}
void initalize_password(void) {
  unsigned char i;

  for (int i = 0; i < PASSWORD_LENGTH; i++) {
    write_internal_eeprom(PASSWORD_ADDRESS + i, DEFAULT_PASSWORD[i]);
  }
}

void main(void) {
  char event[3] = "ON";
  unsigned char speed = 0;
  unsigned char control_flag = DASH_BOARD_FLAG;
  unsigned char key, reset_flag, menu_pos;
  unsigned char gr = 0;

  init_config();

  log_power_on_event();
  initalize_password();

  while (1) {
    unsigned short adc_value;
    unsigned char key;

    /* TODO 5: Read ADC and convert the result to a speed from 0 to 99. */

    adc_value = read_adc(CHANNEL5);

    /* TODO 6: Read keypad input using state detection. */
    key = read_digital_keypad(STATE_DETECTION);

    /* TODO 7: Handle SW1 (collision), SW2 (next gear), SW3 (previous gear). */
     

    /* TODO 8: From dashboard, SW4/SW5 should enter password login. */
    /* TODO 9: From login menu, SW6 should select the highlighted operation. */

    /* Keep the application as a state machine. Implement each state in
       the switch below and preserve the reset_flag behavior. */
    switch (control_flag) {
    case DASH_BOARD_FLAG:
      /* TODO 10: Display current time, event and speed. */
      break;

    case LOGIN_FLAG:
      /* TODO 11: Call login(); handle RETURN_BACK and TASK_SUCCESS. */
      break;

    case LOGIN_MENU_FLAG:
      /* TODO 12: Call login_menu() and retain the selected menu item. */
      break;

    case VIEW_LOG_FLAG:
      /* TODO 13: Call view_log() and handle navigation/back key. */
      break;

    case CLEAR_LOG_FLAG:
      /* TODO 14: Call clear_log(); return to login menu on completion. */
      break;

    case CHANGE_PASSWORD_FLAG:
      /* TODO 15: Call change_password(); handle completion/failure. */
      break;

    default:
      /* TODO 16: Recover safely from an invalid application state. */
      break;
    }

    /* TODO 17: Restore RESET_NOTHING after one-time state initialization. */
  }
}
