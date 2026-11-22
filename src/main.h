#ifndef MAIN_H
#define MAIN_H

#include <string.h>
#include <xc.h>

#include "adc.h"
#include "car_black_box_def.h"
#include "clcd.h"
#include "digital_keypad.h"
#include "ds1307.h"
#include "i2c.h"
#include "timers.h"
#include "uart.h"

#define LOGIN_SUCCESS 0x01
#define RETURN_BACK 0x02
#define TASK_SUCCESS 0x11
#define TASK_FAIL 0x22

#define DASH_BOARD_FLAG 0x02
#define LOGIN_FLAG 0x04
#define LOGIN_MENU_FLAG 0x06
#define VIEW_LOG_FLAG 0x08
#define CLEAR_LOG_FLAG 0x0A
#define CHANGE_PASSWORD_FLAG 0x0B
#define DOWNLOAD_LOG_FLAG 0x0C
#define SET_TIME_FLAG 0x0E

#define RESET_PASSWORD 0x11
#define RESET_LOGIN_MENU 0x13
#define VIEW_LOG_RESET 0x15
#define RESET_MEMORY 0x17
#define RESET_CHANGE_PASSWORD 0x19
#define RESET_DOWNLOAD_LOG 0x1B
#define RESET_SET_TIME 0x1D
#define RESET_NOTHING 0xFF
#define MENU_NONE 0xFF

#define DEFAULT_PASSWORD "0000"
#define PASSWORD_LENGTH 4
#define PASSWORD_LEN PASSWORD_LENGTH
#define PASSWORD_ADDRESS 0x00

#define POWER_ON_SEC_ADDRESS   0x10
#define POWER_ON_MIN_ADDRESS   0x11
#define POWER_ON_HOUR_ADDRESS  0x12
#define POWER_ON_EVENT_ADDRESS 0x13
#define EEPROM_INIT_ADDRESS    0x14   /* marks "default password written" */
#define EEPROM_INIT_MAGIC      0xA5
#define LOG_HEAD_ADDRESS       0x15   /* log_index + 1, survives power cycle */
#define LOG_COUNT_ADDRESS      0x16   /* number of stored logs               */

#define LOG_START_ADDRESS 0x20
#define LOG_RECORD_SIZE   10
#define MAX_LOGS          10
#define LOCK_SECONDS      120

#define ON 0
#define OFF 1

/* Key roles (only SW1..SW5 are used) */
#define KEY_ONE   SW4   /* password '1' / menu up   / log newer / time +  */
#define KEY_ZERO  SW5   /* password '0' / menu down / log older / time -  */
#define KEY_ENTER SW2   /* menu select / next field / save                */
#define KEY_BACK  SW1   /* back / cancel                                  */

#endif
