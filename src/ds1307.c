#include <xc.h>
#include "i2c.h"
#include "ds1307.h"

void init_ds1307(void)
{
    unsigned char dummy;
    /* TODO 1: Read the seconds register and clear the clock-halt bit. */
}

unsigned char read_ds1307(unsigned char addr)
{
    unsigned char data;
    /* TODO 2: START -> slave write -> register address. */
    /* TODO 3: Repeated START -> slave read -> read byte with NACK -> STOP. */
    /* TODO 4: Return the register value. */
    return 0;
}

void write_ds1307(unsigned char addr, unsigned char data)
{
    /* TODO 5: START -> slave write -> register address -> data -> STOP. */
}
