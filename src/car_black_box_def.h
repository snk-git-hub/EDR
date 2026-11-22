#ifndef CAR_BLACK_BOX_H
#define	CAR_BLACK_BOX_H
void display_dash_board(char event[], unsigned  char speed);
void log_car_event(char event[], unsigned char speed);
void clear_screen(void);
char login(unsigned char key,unsigned char reset_flag);
char login_menu(unsigned char key,unsigned char reset_flag);
void view_log(unsigned char key,unsigned char reset_flag);
char download_log(unsigned char reset_flag);
char clear_log(unsigned char reset_flag);
char change_password(unsigned char key,unsigned char reset_flag);
char set_time(unsigned char key, unsigned char reset_flag);
void load_log_state(void);
void write_internal_eeprom(unsigned char address, unsigned char data);
unsigned char read_internal_eeprom(unsigned char address);
void log_power_on_event(void);
void display_event_log(void);
#endif
