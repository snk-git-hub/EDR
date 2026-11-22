#include "i2c.h"
#include <xc.h>

void init_i2c(unsigned long baud) {
  TRISC3 = 1;
  TRISC4 = 1;

  SSPCON = 0x28;
  SSPADD = (unsigned char)((I2C_FOSC / (4UL * baud)) - 1);   /* 100 kHz -> 49 */
  SSPSTAT = 0x80;
}

static void i2c_wait_for_idle(void) {

  while ((SSPCON2 & 0x1F) || (SSPSTAT & 0x04));
}

void i2c_start(void) {
  i2c_wait_for_idle();
  SEN = 1;
  while (SEN);
}

void i2c_rep_start(void) {
  i2c_wait_for_idle();
  RSEN = 1;
  while (RSEN);
}

void i2c_stop(void) {
  /* TODO 7: Wait for idle, then initiate STOP condition. */
  i2c_wait_for_idle();
  PEN = 1;
  while (PEN);
}

unsigned char i2c_read(unsigned char ack) {
  unsigned char data;
  i2c_wait_for_idle();
  RCEN = 1;
  while (!BF);
  data = SSPBUF;

  i2c_wait_for_idle();
  ACKDT = ack ? 0 : 1;
  ACKEN = 1;
  while (ACKEN);
  return data;
}

int i2c_write(unsigned char data) {
    i2c_wait_for_idle();
    SSPBUF = data;
    i2c_wait_for_idle();
    i2c_wait_for_idle();
    
  return ACKSTAT;
}
