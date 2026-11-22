#ifndef UART_H
#define UART_H
void init_uart(unsigned long baud);
void uart_putch(unsigned char data);
void uart_puts(const char *s);
#endif
