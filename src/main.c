/* Car Black Box - Application skeleton
 * Target: PIC16F877A
 * Implement each TODO without changing the provided public interfaces.
 */
#include <xc.h>
#include <string.h>
#include "main.h"

#pragma config WDTE = OFF

/* Gear labels used by the dashboard. */
char *gear[] = {"GN", "GR", "G1", "G2", "G3", "G4"};

static void init_config(void)
{
    /* TODO 1: Initialize the CLCD, I2C, DS1307, ADC, keypad and Timer2. */
    /* TODO 2: Enable peripheral and global interrupts after initialization. */
}

void main(void)
{
    char event[3] = "ON";
    unsigned char speed = 0;
    unsigned char control_flag = DASH_BOARD_FLAG;
    unsigned char key, reset_flag, menu_pos;
    unsigned char gr = 0;

    init_config();

    /* TODO 3: Log the power-on event. */
    /* TODO 4: Initialize the default four-digit password in EEPROM only
       when the project requirements specify first-time initialization. */

    while (1)
    {
        /* TODO 5: Read ADC and convert the result to a speed from 0 to 99. */
        /* TODO 6: Read keypad input using state detection. */
        /* TODO 7: Handle SW1 (collision), SW2 (next gear), SW3 (previous gear). */
        /* TODO 8: From dashboard, SW4/SW5 should enter password login. */
        /* TODO 9: From login menu, SW6 should select the highlighted operation. */

        /* Keep the application as a state machine. Implement each state in
           the switch below and preserve the reset_flag behavior. */
        switch (control_flag)
        {
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
