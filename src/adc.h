#ifndef ADC_H
#define ADC_H

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 20000000
#endif

void init_adc(void);
unsigned short read_adc(unsigned char channel);
#define CHANNEL0 0x00   
#define CHANNEL1 0x01 
#define CHANNEL2 0x02
#define CHANNEL3 0x03
#define CHANNEL4 0x04   /* RA5/AN4 - pot on most PIC16F877A trainer boards */
#define CHANNEL5 0x05
#endif