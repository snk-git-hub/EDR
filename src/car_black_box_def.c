#include "main.h"
#include <string.h>
#include <xc.h>

/* ------------------------------------------------------------------ */
/* Fallback definitions: used only if main.h does not already have them */
/* ------------------------------------------------------------------ */
#ifndef PASSWORD_ADDRESS
#define PASSWORD_ADDRESS 0x00      /* 4 bytes: ASCII '0'/'1' */
#endif
#ifndef PASSWORD_LEN
#define PASSWORD_LEN 4
#endif
#ifndef MAX_LOGS
#define MAX_LOGS 10
#endif
#ifndef LOG_RECORD_SIZE
#define LOG_RECORD_SIZE 10         /* HHMMSS + EV(2) + SP(2), ASCII */
#endif
#ifndef LOG_START_ADDRESS
#define LOG_START_ADDRESS 0x20     /* 0x10-0x13 = power-on event */
#endif
#ifndef LOCK_SECONDS
#define LOCK_SECONDS 120
#endif
#ifndef MENU_NONE
#define MENU_NONE 0xFF             /* login_menu: nothing selected yet */
#endif

/* KEY_ONE, KEY_ZERO, KEY_ENTER, KEY_BACK are defined in main.h */
#define IN_PROGRESS TASK_FAIL      /* "still running" status */
#define MENU_COUNT  5

/* Shared application data */
unsigned char clock_Freg[3];
char time_buf[9];                  /* "HH:MM:SS" */
int event_count = 0;
static signed char log_index = -1;

/* Both are decremented once per second by the timer2 ISR (see isr.c) */
volatile unsigned char sec = 0;    /* lock-out countdown */
volatile unsigned char return_time = 5;  /* inactivity / message timeout */

char *menu[] = {"View log", "Download log", "Clear log", "Change passwrd", "Set time"};

/* ------------------------------------------------------------------ */
/* Time helpers                                                        */
/* ------------------------------------------------------------------ */
static void read_rtc_time(unsigned char *h, unsigned char *m, unsigned char *s)
{
    *s = read_ds1307(0x00) & 0x7F;     /* mask clock-halt bit   */
    *m = read_ds1307(0x01) & 0x7F;
    *h = read_ds1307(0x02) & 0x3F;     /* 24-hour mode          */
}

/* BCD -> "HH:MM:SS" */
static void convert_time(unsigned char h, unsigned char m, unsigned char s,
                         char *buffer)
{
    buffer[0] = ((h >> 4) & 0x0F) + '0';
    buffer[1] = (h & 0x0F) + '0';
    buffer[2] = ':';
    buffer[3] = ((m >> 4) & 0x0F) + '0';
    buffer[4] = (m & 0x0F) + '0';
    buffer[5] = ':';
    buffer[6] = ((s >> 4) & 0x0F) + '0';
    buffer[7] = (s & 0x0F) + '0';
    buffer[8] = '\0';
}

void get_time(void)
{
    unsigned char h, m, s;

    read_rtc_time(&h, &m, &s);
    convert_time(h, m, s, time_buf);
}

void display_time(void)
{
    get_time();
    clcd_print(time_buf, LINE2(0));
}

/* ------------------------------------------------------------------ */
/* Power-on event                                                      */
/* ------------------------------------------------------------------ */
void log_power_on_event(void)
{
    unsigned char h, m, s;

    read_rtc_time(&h, &m, &s);

    write_internal_eeprom(POWER_ON_SEC_ADDRESS, s);
    write_internal_eeprom(POWER_ON_MIN_ADDRESS, m);
    write_internal_eeprom(POWER_ON_HOUR_ADDRESS, h);
    write_internal_eeprom(POWER_ON_EVENT_ADDRESS, ON);
}

void display_event_log(void)
{
    unsigned char h, m, s, event;
    char event_time[9];

    s = read_internal_eeprom(POWER_ON_SEC_ADDRESS);
    m = read_internal_eeprom(POWER_ON_MIN_ADDRESS);
    h = read_internal_eeprom(POWER_ON_HOUR_ADDRESS);
    event = read_internal_eeprom(POWER_ON_EVENT_ADDRESS);

    convert_time(h, m, s, event_time);
    clcd_print(event_time, LINE2(0));

    if (event == ON)
    {
        clcd_print("ON", LINE2(9));
    }
}

/* ------------------------------------------------------------------ */
/* Dashboard                                                           */
/* ------------------------------------------------------------------ */
void display_dash_board(char event[], unsigned char speed)
{
    clcd_print("TIME     EV  SP", LINE1(0));
    display_time();
    clcd_print(event, LINE2(9));
    clcd_putch((speed / 10) + '0', LINE2(14));
    clcd_putch((speed % 10) + '0', LINE2(15));
}

/* ------------------------------------------------------------------ */
/* Log state kept in internal EEPROM so logs survive a power cycle     */
/* ------------------------------------------------------------------ */
static void save_log_state(void)
{
    write_internal_eeprom(LOG_HEAD_ADDRESS, (unsigned char)(log_index + 1));
    write_internal_eeprom(LOG_COUNT_ADDRESS, (unsigned char)event_count);
}

void load_log_state(void)
{
    unsigned char head = read_internal_eeprom(LOG_HEAD_ADDRESS);
    unsigned char cnt  = read_internal_eeprom(LOG_COUNT_ADDRESS);

    if (head > MAX_LOGS || cnt > MAX_LOGS)     /* blank EEPROM (0xFF) */
    {
        head = 0;
        cnt = 0;
    }
    log_index = (signed char)head - 1;
    event_count = cnt;
}

/* ------------------------------------------------------------------ */
/* Logging: 10-byte ASCII record  H H M M S S E E S S                  */
/* ------------------------------------------------------------------ */
static void log_event(char event[], unsigned char speed)
{
    unsigned char add;
    unsigned char h, m, s;

    log_index++;
    if (log_index >= MAX_LOGS)
    {
        log_index = 0;
    }

    add = LOG_START_ADDRESS + (unsigned char)(log_index * LOG_RECORD_SIZE);

    read_rtc_time(&h, &m, &s);

    write_internal_eeprom(add++, ((h >> 4) & 0x0F) + '0');
    write_internal_eeprom(add++, (h & 0x0F) + '0');
    write_internal_eeprom(add++, ((m >> 4) & 0x0F) + '0');
    write_internal_eeprom(add++, (m & 0x0F) + '0');
    write_internal_eeprom(add++, ((s >> 4) & 0x0F) + '0');
    write_internal_eeprom(add++, (s & 0x0F) + '0');

    write_internal_eeprom(add++, event[0]);
    write_internal_eeprom(add++, event[1]);

    write_internal_eeprom(add++, (speed / 10) + '0');
    write_internal_eeprom(add++, (speed % 10) + '0');

    if (event_count < MAX_LOGS)
    {
        event_count++;
    }

    save_log_state();
}

void log_car_event(char event[], unsigned char speed)
{
    log_event(event, speed);
}

/* ------------------------------------------------------------------ */
/* LCD / input helpers                                                 */
/* ------------------------------------------------------------------ */
void clear_screen(void)
{
    unsigned int i;

    clcd_write(CLEAR_DISP_SCREEN, INST_MODE);
    for (i = 0; i < 2000; i++)         /* clear needs ~1.6 ms */
        ;
}

/* Map a key press to a password bit character, or 0 if not a digit key */
static char key_to_digit(unsigned char key)
{
    if (key == KEY_ONE)  return '1';
    if (key == KEY_ZERO) return '0';
    return 0;
}

static void show_stars(unsigned char count)
{
    unsigned char i;

    clcd_print("                ", LINE2(0));
    for (i = 0; i < count; i++)
    {
        clcd_putch('*', LINE2(4 + i));
    }
}

/* ------------------------------------------------------------------ */
/* Login                                                               */
/* ------------------------------------------------------------------ */
char login(unsigned char key, unsigned char reset_flag)
{
    static unsigned char attempts, index, locked;
    static char entered[PASSWORD_LEN];
    unsigned char i;
    char digit;

    if (reset_flag == RESET_PASSWORD)
    {
        attempts = 3;
        index = 0;
        locked = 0;
        return_time = 5;
        clear_screen();
        clcd_print(" ENTER PASSWORD ", LINE1(0));
        show_stars(0);
        return IN_PROGRESS;
    }

    /* Locked out after too many wrong attempts */
    if (locked)
    {
        clcd_print("LOCKED, WAIT    ", LINE1(0));
        clcd_putch((sec / 100) + '0', LINE2(6));
        clcd_putch(((sec / 10) % 10) + '0', LINE2(7));
        clcd_putch((sec % 10) + '0', LINE2(8));
        clcd_print(" sec", LINE2(9));

        if (sec == 0)
        {
            locked = 0;
            attempts = 3;
            index = 0;
            return_time = 5;
            clear_screen();
            clcd_print(" ENTER PASSWORD ", LINE1(0));
            show_stars(0);
        }
        return IN_PROGRESS;
    }

    /* Inactivity timeout */
    if (return_time == 0)
    {
        return RETURN_BACK;
    }

    digit = key_to_digit(key);
    if (digit)
    {
        entered[index++] = digit;
        return_time = 5;
        show_stars(index);
    }

    if (index == PASSWORD_LEN)
    {
        unsigned char ok = 1;

        for (i = 0; i < PASSWORD_LEN; i++)
        {
            if (entered[i] != read_internal_eeprom(PASSWORD_ADDRESS + i))
            {
                ok = 0;
            }
        }

        if (ok)
        {
            clear_screen();
            clcd_print("LOGIN SUCCESS   ", LINE1(0));
            return TASK_SUCCESS;
        }

        attempts--;
        index = 0;

        if (attempts == 0)
        {
            locked = 1;
            sec = LOCK_SECONDS;
            clear_screen();
        }
        else
        {
            clcd_print("WRONG PASSWORD  ", LINE1(0));
            clcd_print("                ", LINE2(0));
            clcd_putch(attempts + '0', LINE2(0));
            clcd_print(" tries left", LINE2(1));
            return_time = 5;
        }
    }

    return IN_PROGRESS;
}

/* ------------------------------------------------------------------ */
/* Menu: returns VIEW_LOG_FLAG / DOWNLOAD_LOG_FLAG / CLEAR_LOG_FLAG /  */
/* CHANGE_PASSWORD_FLAG / SET_TIME_FLAG when selected, RETURN_BACK on  */
/* back, MENU_NONE while browsing                                      */
/* ------------------------------------------------------------------ */
static void draw_menu(unsigned char sel)
{
    unsigned char top = (sel == 0) ? 0 : (sel - 1);

    clcd_print("                ", LINE1(0));
    clcd_print("                ", LINE2(0));

    clcd_putch((sel == top) ? '>' : ' ', LINE1(0));
    clcd_print(menu[top], LINE1(2));
    clcd_putch((sel == top + 1) ? '>' : ' ', LINE2(0));
    clcd_print(menu[top + 1], LINE2(2));
}

char login_menu(unsigned char key, unsigned char reset_flag)
{
    static unsigned char sel;
    static const unsigned char menu_flag[MENU_COUNT] =
        {VIEW_LOG_FLAG, DOWNLOAD_LOG_FLAG, CLEAR_LOG_FLAG,
         CHANGE_PASSWORD_FLAG, SET_TIME_FLAG};

    if (reset_flag == RESET_LOGIN_MENU)
    {
        sel = 0;
        return_time = 5;
        clear_screen();
        draw_menu(sel);
        return MENU_NONE;
    }

    if (return_time == 0 || key == KEY_BACK)
    {
        return RETURN_BACK;
    }

    if (key == KEY_ONE && sel > 0)
    {
        sel--;
        return_time = 5;
        draw_menu(sel);
    }
    else if (key == KEY_ZERO && sel < MENU_COUNT - 1)
    {
        sel++;
        return_time = 5;
        draw_menu(sel);
    }
    else if (key == KEY_ENTER)
    {
        return menu_flag[sel];
    }

    return MENU_NONE;
}

/* ------------------------------------------------------------------ */
/* View log: newest first. KEY_ONE = newer, KEY_ZERO = older.          */
/* (Leaving the screen is handled by the caller, since this is void.)  */
/* ------------------------------------------------------------------ */
void view_log(unsigned char key, unsigned char reset_flag)
{
    static int n;                      /* 0 = newest */
    unsigned char add, i;
    int slot;
    char rec[LOG_RECORD_SIZE];
    char disp[9];

    if (reset_flag == VIEW_LOG_RESET)
    {
        n = 0;
        clear_screen();
        if (event_count == 0)
        {
            clcd_print("NO LOGS", LINE1(0));
            return;
        }
        clcd_print("# TIME     EV SP", LINE1(0));
    }

    if (event_count == 0)
    {
        return;
    }

    if (key == KEY_ZERO && n < event_count - 1)
    {
        n++;
    }
    else if (key == KEY_ONE && n > 0)
    {
        n--;
    }

    slot = (log_index - n + MAX_LOGS) % MAX_LOGS;
    add = LOG_START_ADDRESS + (unsigned char)(slot * LOG_RECORD_SIZE);

    for (i = 0; i < LOG_RECORD_SIZE; i++)
    {
        rec[i] = read_internal_eeprom(add + i);
    }

    disp[0] = rec[0];
    disp[1] = rec[1];
    disp[2] = ':';
    disp[3] = rec[2];
    disp[4] = rec[3];
    disp[5] = ':';
    disp[6] = rec[4];
    disp[7] = rec[5];
    disp[8] = '\0';

    clcd_putch((unsigned char)((n % 10) + '0'), LINE2(0));
    clcd_print(disp, LINE2(2));
    clcd_putch(rec[6], LINE2(11));
    clcd_putch(rec[7], LINE2(12));
    clcd_putch(rec[8], LINE2(14));
    clcd_putch(rec[9], LINE2(15));
}

/* ------------------------------------------------------------------ */
/* Download log: sends all logs to the serial terminal, oldest first   */
/* ------------------------------------------------------------------ */
char download_log(unsigned char reset_flag)
{
    int k, slot;
    unsigned char add, i;

    if (reset_flag == RESET_DOWNLOAD_LOG)
    {
        clear_screen();
        clcd_print("DOWNLOADING...  ", LINE1(0));

        uart_puts("\r\n---- CAR BLACK BOX LOG ----\r\n");

        if (event_count == 0)
        {
            uart_puts("NO LOGS\r\n");
            clcd_print("NO LOGS         ", LINE2(0));
        }
        else
        {
            uart_puts("# TIME     EV SP\r\n");

            for (k = event_count - 1; k >= 0; k--)      /* k = age, 0 = newest */
            {
                slot = (log_index - k + MAX_LOGS) % MAX_LOGS;
                add = LOG_START_ADDRESS + (unsigned char)(slot * LOG_RECORD_SIZE);

                uart_putch((unsigned char)((event_count - 1 - k) % 10) + '0');
                uart_putch(' ');
                for (i = 0; i < LOG_RECORD_SIZE; i++)   /* HHMMSSEESS */
                {
                    uart_putch(read_internal_eeprom(add + i));
                    if (i == 1 || i == 3)
                        uart_putch(':');
                    else if (i == 5 || i == 7)
                        uart_putch(' ');
                }
                uart_puts("\r\n");                      /* fixes garbled lines */
            }
            clcd_print("DOWNLOAD DONE   ", LINE2(0));
        }

        return_time = 2;
        return IN_PROGRESS;
    }

    if (return_time == 0)
    {
        return TASK_SUCCESS;
    }
    return IN_PROGRESS;
}

/* ------------------------------------------------------------------ */
/* Clear log                                                           */
/* ------------------------------------------------------------------ */
char clear_log(unsigned char reset_flag)
{
    if (reset_flag == RESET_MEMORY)
    {
        /* Resetting the metadata is enough: old records are never read
           again and get overwritten. This also saves EEPROM wear. */
        event_count = 0;
        log_index = -1;
        save_log_state();

        clear_screen();
        clcd_print("  LOG CLEARED   ", LINE1(0));
        clcd_print("  SUCCESSFULLY  ", LINE2(0));
        return_time = 2;
        return IN_PROGRESS;
    }

    if (return_time == 0)
    {
        return TASK_SUCCESS;
    }
    return IN_PROGRESS;
}

/* ------------------------------------------------------------------ */
/* Change password: enter twice, write to EEPROM if both match         */
/* ------------------------------------------------------------------ */
char change_password(unsigned char key, unsigned char reset_flag)
{
    static unsigned char stage, index, done, result;
    static char first[PASSWORD_LEN], second[PASSWORD_LEN];
    unsigned char i;
    char digit;

    if (reset_flag == RESET_CHANGE_PASSWORD)
    {
        stage = 0;
        index = 0;
        done = 0;
        return_time = 5;
        clear_screen();
        clcd_print(" ENTER NEW PASS ", LINE1(0));
        show_stars(0);
        return IN_PROGRESS;
    }

    /* Result message is on screen: wait for it to time out */
    if (done)
    {
        if (return_time == 0)
        {
            return result;
        }
        return IN_PROGRESS;
    }

    if (return_time == 0)
    {
        return RETURN_BACK;
    }

    digit = key_to_digit(key);
    if (digit)
    {
        if (stage == 0)
            first[index++] = digit;
        else
            second[index++] = digit;

        return_time = 5;
        show_stars(index);
    }

    if (index == PASSWORD_LEN)
    {
        index = 0;

        if (stage == 0)
        {
            stage = 1;
            clear_screen();
            clcd_print(" RE-ENTER PASS  ", LINE1(0));
            show_stars(0);
            return IN_PROGRESS;
        }

        /* Compare both entries */
        for (i = 0; i < PASSWORD_LEN; i++)
        {
            if (first[i] != second[i])
                break;
        }

        clear_screen();
        if (i == PASSWORD_LEN)
        {
            for (i = 0; i < PASSWORD_LEN; i++)
            {
                write_internal_eeprom(PASSWORD_ADDRESS + i, first[i]);
            }
            clcd_print("PASSWORD CHANGED", LINE1(0));
            result = TASK_SUCCESS;
        }
        else
        {
            clcd_print("PASS MISMATCH   ", LINE1(0));
            result = RETURN_BACK;
        }

        done = 1;
        return_time = 2;
    }

    return IN_PROGRESS;
}

/* ------------------------------------------------------------------ */
/* Set time: KEY_ONE = +, KEY_ZERO = -, KEY_ENTER = next field / save, */
/* KEY_BACK or inactivity = cancel                                     */
/* ------------------------------------------------------------------ */
static unsigned char bcd_to_dec(unsigned char b)
{
    return ((b >> 4) * 10) + (b & 0x0F);
}

static unsigned char dec_to_bcd(unsigned char d)
{
    return ((d / 10) << 4) | (d % 10);
}

static void show_set_time(unsigned char field, unsigned char *v)
{
    static char *label[3] = {" SET HOUR       ", " SET MINUTE     ", " SET SECOND     "};
    unsigned char i;

    clcd_print(label[field], LINE1(0));
    clcd_print("                ", LINE2(0));
    for (i = 0; i < 3; i++)
    {
        clcd_putch((v[i] / 10) + '0', LINE2(4 + i * 3));
        clcd_putch((v[i] % 10) + '0', LINE2(5 + i * 3));
        if (i < 2)
        {
            clcd_putch(':', LINE2(6 + i * 3));
        }
    }
}

char set_time(unsigned char key, unsigned char reset_flag)
{
    static unsigned char field, done, result;
    static unsigned char val[3];                       /* H, M, S in decimal */
    static const unsigned char max_val[3] = {23, 59, 59};
    unsigned char h, m, s;

    if (reset_flag == RESET_SET_TIME)
    {
        read_rtc_time(&h, &m, &s);
        val[0] = bcd_to_dec(h);
        val[1] = bcd_to_dec(m);
        val[2] = bcd_to_dec(s);
        field = 0;
        done = 0;
        return_time = 10;
        clear_screen();
        show_set_time(field, val);
        return IN_PROGRESS;
    }

    if (done)
    {
        if (return_time == 0)
        {
            return result;
        }
        return IN_PROGRESS;
    }

    if (return_time == 0 || key == KEY_BACK)
    {
        return RETURN_BACK;
    }

    if (key == KEY_ONE)
    {
        val[field] = (val[field] >= max_val[field]) ? 0 : val[field] + 1;
        return_time = 10;
        show_set_time(field, val);
    }
    else if (key == KEY_ZERO)
    {
        val[field] = val[field] ? val[field] - 1 : max_val[field];
        return_time = 10;
        show_set_time(field, val);
    }
    else if (key == KEY_ENTER)
    {
        if (field < 2)
        {
            field++;
            return_time = 10;
            show_set_time(field, val);
        }
        else
        {
            write_ds1307(0x02, dec_to_bcd(val[0]));    /* hour, 24-hour mode */
            write_ds1307(0x01, dec_to_bcd(val[1]));    /* minute */
            write_ds1307(0x00, dec_to_bcd(val[2]));    /* second, CH = 0 */
            clear_screen();
            clcd_print("  TIME UPDATED  ", LINE1(0));
            result = TASK_SUCCESS;
            done = 1;
            return_time = 2;
        }
    }

    return IN_PROGRESS;
}

/* ------------------------------------------------------------------ */
/* Internal EEPROM                                                     */
/* ------------------------------------------------------------------ */
void write_internal_eeprom(unsigned char address, unsigned char data)
{
    unsigned char gie_state = INTCONbits.GIE;

    EEADR = address;
    EEDATA = data;

    EECON1bits.EEPGD = 0;
    EECON1bits.WREN = 1;

    INTCONbits.GIE = 0;
    EECON2 = 0x55;
    EECON2 = 0xAA;
    EECON1bits.WR = 1;
    INTCONbits.GIE = gie_state;        /* restore, don't force on */

    while (EECON1bits.WR)
        ;

    EECON1bits.WREN = 0;
}

unsigned char read_internal_eeprom(unsigned char address)
{
    EEADR = address;

    EECON1bits.EEPGD = 0;
    EECON1bits.RD = 1;

    return EEDATA;
}
