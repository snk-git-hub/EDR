#include "main.h"
#include <string.h>
#include <xc.h>

// /* Shared application data. Keep definitions in this one source file. */ DONE
unsigned char clock_Freg[3];
char time[7];
char log[11];
char pos = -1;
int event_count = 0;
extern unsigned char sec;
extern unsigned char return_time;
char *menu[] = {"View log", "Clear log", "Change passwrd"};

void get_time(void) {
  /* TODO 1: Read hour, minute and second registers from DS1307. */
  /* TODO 2: Convert each BCD digit into ASCII characters. */
  /* TODO 3: Null-terminate the HHMMSS string in time[]. */
}

void display_time(void) {
  /* TODO 4: Call get_time() and display HH:MM:SS at the specified LCD
   * positions. */
}

void display_dash_board(char event[], unsigned char speed) {
  /* TODO 5: Display dashboard headings, current time, event and two-digit
   * speed. */
}

void log_event(void) {
  unsigned char add;
  /* TODO 6: Advance circular log position and wrap at the maximum entry count.
   */
  /* TODO 7: Calculate EEPROM address and write the complete log record. */
  /* TODO 8: Update event_count without exceeding the log capacity. */
}

void log_car_event(char event[], unsigned char speed) {
  /* TODO 9: Get current time and build the fixed-size event record:
     HHMMSS + 2-character event + 2-digit speed + string terminator. */
  /* TODO 10: Pass the record to log_event(). */
}

void clear_screen(void) {
  /* TODO 11: Send LCD clear-display instruction and wait for completion. */
}

char login(unsigned char key, unsigned char reset_flag) {
  /* TODO 12: On RESET_PASSWORD, initialize attempt count, input index and
   * timeout. */
  /* TODO 13: Accept the two keypad keys used to enter password digits. */
  /* TODO 14: Display masked characters, compare with EEPROM password and
     handle success, failed attempts and timeout. */
  /* TODO 15: Return RETURN_BACK, TASK_SUCCESS or the in-progress status. */
  return 0;
}

char login_menu(unsigned char key, unsigned char reset_flag) {
  /* TODO 16: Initialize menu position when RESET_LOGIN_MENU is received. */
  /* TODO 17: Navigate through View log, Clear log and Change password. */
  /* TODO 18: Display selection and return the selected menu index. */
  return 0;
}

void view_log(unsigned char key, unsigned char reset_flag) {
  /* TODO 19: Initialize log-view position on VIEW_LOG_RESET. */
  /* TODO 20: Read stored records from EEPROM and display time/event/speed. */
  /* TODO 21: Implement next/previous navigation and return-key handling. */
}

char clear_log(unsigned char reset_flag) {
  /* TODO 22: On RESET_MEMEORY, clear/reset stored log metadata and entries. */
  /* TODO 23: Display completion and return TASK_SUCCESS when finished. */
  return 0;
}

char change_password(unsigned char key, unsigned char reset_flag) {
  /* TODO 24: On RESET_CHANGE_PASSWORD, initialize new-password input state. */
  /* TODO 25: Read the new password twice and compare both entries. */
  /* TODO 26: If they match, write all password digits to EEPROM. */
  /* TODO 27: Display success/failure and return the required task status. */
  return 0;
}
void write_internal_eeprom(unsigned char address, unsigned char data){
EEADR = address;
EEDATA = data;

EECON1bits.EEPGD = 0;

EECON1bits.WREN = 1;

INTCONbits.GIE = 0;

EECON2 = 0x55;
EECON2 = 0xAA;

EECON1bits.WR =1;
while (EECON1bits.WR);

EECON1bits.WREN = 0;
INTCONbits.GIE = 1;

}

unsigned char read_internal_eeprom(unsigned char address)
{
    EEADR = address;

    EECON1bits.EEPGD = 0;
    EECON1bits.RD = 1;

    return EEDATA;
}