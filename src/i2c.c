#include <xc.h>
#include "i2c.h"

void init_i2c(unsigned long baud)
{
    /* TODO 1: Configure MSSP in I2C master mode. */
    /* TODO 2: Calculate and load SSPADD from FOSC and baud. */
    /* TODO 3: Enable the MSSP module. */
}

static void i2c_wait_for_idle(void)
{
    /* TODO 4: Wait until the MSSP module and I2C bus are idle. */
}

void i2c_start(void)
{
    /* TODO 5: Wait for idle, then initiate START condition. */
}

void i2c_rep_start(void)
{
    /* TODO 6: Generate a repeated START condition. */
}

void i2c_stop(void)
{
    /* TODO 7: Wait for idle, then initiate STOP condition. */
}

unsigned char i2c_read(unsigned char ack)
{
    unsigned char data;
    /* TODO 8: Enable receive, wait, and read SSPBUF. */
    /* TODO 9: Send ACK or NACK based on the ack argument. */
    /* TODO 10: Return the received byte. */
    return 0;
}

int i2c_write(unsigned char data)
{
    /* TODO 11: Wait for idle and write data to SSPBUF. */
    /* TODO 12: Return the ACK status to the caller. */
    return 0;
}
