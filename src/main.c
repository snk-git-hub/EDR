#include "main.h"
#include <string.h>
#include <xc.h>

#pragma config WDTE = OFF

char *gear[] = {"GN", "GR", "G1", "G2", "G3", "G4"};

unsigned char gear_index = 1;
unsigned char menu_index = 0;

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

  write_internal_eeprom(0x03, ON);
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
  unsigned char key;
  unsigned char reset_flag = RESET_NOTHING;

  unsigned short adc_valu;
  unsigned char return_flag;

  init_config();

  log_power_on_event();
  initalize_password();

  while (1) {
    unsigned short adc_value;
    unsigned char key;

    adc_value = read_adc(CHANNEL5);

    key = read_digital_keypad(STATE_DETECTION);

    switch (control_flag) {
    case DASH_BOARD_FLAG:
      display_dash_board(event, speed);
      if (key == SW1) {
        log_car_event("CO", speed);

      } else if (key == SW2) {
        if (gear_index < 5) {
          gear_index++;
        }
      } else if (key == SW3) {
        if (gear_index > 1) {
          gear_index--;
        }
      } else if (key == SW4 || key == SW5) {
        control_flag = LOGIN_FLAG;
        reset_flag = RESET_PASSWORD;
      }
      break;

    case LOGIN_FLAG:
      reset_flag = login(key, reset_flag);
      if (return_flag == TASK_SUCCESS) {
        control_flag = LOGIN_MENU_FLAG;
        reset_flag = RESET_PASSWORD;
      } else if (return_flag == RETURN_BACK) {
        control_flag = DASH_BOARD_FLAG;
        reset_flag = RESET_NOTHING;
      }
      break;

    case LOGIN_MENU_FLAG:
      return_flag = login_menu(key, reset_flag);

      if (return_flag == RETURN_BACK) {
        control_flag = DASH_BOARD_FLAG;
        reset_flag = RESET_NOTHING;
      } else if (return_flag == VIEW_LOG_FLAG ||
                 return_flag == CLEAR_LOG_FLAG ||
                 return_flag == CHANGE_PASSWORD_FLAG) {
        control_flag = return_flag;
        reset_flag = RESET_PASSWORD;
      }
       break;

    case VIEW_LOG_FLAG:
      view_log(key, reset_flag);
      break;

    case CLEAR_LOG_FLAG:
      return_flag = clear_log(reset_flag);

      if (return_flag == TASK_SUCCESS) {
        control_flag = LOGIN_MENU_FLAG;
        reset_flag = RESET_PASSWORD;
      }

      break;

    case CHANGE_PASSWORD_FLAG:
      return_flag = change_password(key, reset_flag);

      if (return_flag == TASK_SUCCESS) {
        control_flag = LOGIN_MENU_FLAG;
        reset_flag = RESET_PASSWORD;
      } else if (return_flag == RETURN_BACK) {
        control_flag = LOGIN_MENU_FLAG;
        reset_flag = RESET_NOTHING;
      }

      break;

    default:
      control_flag = DASH_BOARD_FLAG;
      reset_flag = RESET_NOTHING;
      break;
    }

    reset_flag = RESET_NOTHING;
  }
}
