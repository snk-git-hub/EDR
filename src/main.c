#include "main.h"
#include <string.h>
#include <xc.h>

#pragma config FOSC = HS
#pragma config WDTE = OFF
#pragma config PWRTE = OFF
#pragma config BOREN = OFF
#pragma config LVP = OFF      /* frees RB3 as a normal I/O pin */
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

char *gear[] = {"GN", "GR", "G1", "G2", "G3", "G4"};

unsigned char gear_index = 0;          /* start in neutral */

static void init_config(void) {
  init_clcd();
  init_digital_keypad();
  init_adc();
  init_i2c(100000);                    /* match the signature in your i2c.h */
  init_uart(9600);                     /* serial for Download Log */
  init_ds1307();                       /* I2C must be up before the RTC */
  init_timer2();
}

/* Write the default password only the first time the chip is programmed,
   otherwise a changed password would be lost on every power-up. */
static void initalize_password(void) {
  unsigned char i;

  if (read_internal_eeprom(EEPROM_INIT_ADDRESS) == EEPROM_INIT_MAGIC) {
    return;
  }

  for (i = 0; i < PASSWORD_LENGTH; i++) {
    write_internal_eeprom(PASSWORD_ADDRESS + i, DEFAULT_PASSWORD[i]);
  }
  write_internal_eeprom(EEPROM_INIT_ADDRESS, EEPROM_INIT_MAGIC);
}

void main(void) {
  char event[3] = "ON";
  unsigned char speed = 0;
  unsigned char control_flag = DASH_BOARD_FLAG;
  unsigned char reset_flag = RESET_NOTHING;
  unsigned char return_flag = 0;
  unsigned char key;
  unsigned short adc_value;

  init_config();

  log_power_on_event();
  initalize_password();
  load_log_state();                      /* restore log count/index from EEPROM */

  while (1) {
    adc_value = read_adc(CHANNEL0);
    if (adc_value / 10 > 99) {
      speed = 99;
    } else {
      speed = (unsigned char)(adc_value / 10);
    }

    key = read_digital_keypad(STATE_DETECTION);

    switch (control_flag) {
    case DASH_BOARD_FLAG:
      display_dash_board(event, speed);

      if (key == SW1) {
        strcpy(event, "CO");
        log_car_event(event, speed);
      } else if (key == SW2) {
        if (gear_index < 5) {
          gear_index++;
        }
        strcpy(event, gear[gear_index]);
        log_car_event(event, speed);
      } else if (key == SW3) {
        if (gear_index > 0) {
          gear_index--;
        }
        strcpy(event, gear[gear_index]);
        log_car_event(event, speed);
      } else if (key == SW4 || key == SW5) {
        control_flag = LOGIN_FLAG;
        reset_flag = RESET_PASSWORD;
      }
      break;

    case LOGIN_FLAG:
      return_flag = login(key, reset_flag);
      reset_flag = RESET_NOTHING;            /* consumed */

      if (return_flag == TASK_SUCCESS) {
        control_flag = LOGIN_MENU_FLAG;
        reset_flag = RESET_LOGIN_MENU;
      } else if (return_flag == RETURN_BACK) {
        clear_screen();
        control_flag = DASH_BOARD_FLAG;
      }
      break;

    case LOGIN_MENU_FLAG:
      return_flag = login_menu(key, reset_flag);
      reset_flag = RESET_NOTHING;

      if (return_flag == RETURN_BACK) {
        clear_screen();
        control_flag = DASH_BOARD_FLAG;
      } else if (return_flag == VIEW_LOG_FLAG) {
        control_flag = VIEW_LOG_FLAG;
        reset_flag = VIEW_LOG_RESET;
      } else if (return_flag == DOWNLOAD_LOG_FLAG) {
        control_flag = DOWNLOAD_LOG_FLAG;
        reset_flag = RESET_DOWNLOAD_LOG;
      } else if (return_flag == CLEAR_LOG_FLAG) {
        control_flag = CLEAR_LOG_FLAG;
        reset_flag = RESET_MEMORY;
      } else if (return_flag == CHANGE_PASSWORD_FLAG) {
        control_flag = CHANGE_PASSWORD_FLAG;
        reset_flag = RESET_CHANGE_PASSWORD;
      } else if (return_flag == SET_TIME_FLAG) {
        control_flag = SET_TIME_FLAG;
        reset_flag = RESET_SET_TIME;
      }
      break;

    case VIEW_LOG_FLAG:
      view_log(key, reset_flag);
      reset_flag = RESET_NOTHING;

      if (key == KEY_BACK) {
        control_flag = LOGIN_MENU_FLAG;
        reset_flag = RESET_LOGIN_MENU;
      }
      break;

    case DOWNLOAD_LOG_FLAG:
      return_flag = download_log(reset_flag);
      reset_flag = RESET_NOTHING;

      if (return_flag == TASK_SUCCESS) {
        control_flag = LOGIN_MENU_FLAG;
        reset_flag = RESET_LOGIN_MENU;
      }
      break;

    case CLEAR_LOG_FLAG:
      return_flag = clear_log(reset_flag);
      reset_flag = RESET_NOTHING;

      if (return_flag == TASK_SUCCESS) {
        control_flag = LOGIN_MENU_FLAG;
        reset_flag = RESET_LOGIN_MENU;
      }
      break;

    case CHANGE_PASSWORD_FLAG:
      return_flag = change_password(key, reset_flag);
      reset_flag = RESET_NOTHING;

      if (return_flag == TASK_SUCCESS || return_flag == RETURN_BACK) {
        control_flag = LOGIN_MENU_FLAG;
        reset_flag = RESET_LOGIN_MENU;
      }
      break;

    case SET_TIME_FLAG:
      return_flag = set_time(key, reset_flag);
      reset_flag = RESET_NOTHING;

      if (return_flag == TASK_SUCCESS || return_flag == RETURN_BACK) {
        control_flag = LOGIN_MENU_FLAG;
        reset_flag = RESET_LOGIN_MENU;
      }
      break;

    default:
      clear_screen();
      control_flag = DASH_BOARD_FLAG;
      reset_flag = RESET_NOTHING;
      break;
    }
  }
}
