#ifndef ADC_H
#define ADC_H

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 20000000
#endif

void init_adc(void);
unsigned short read_adc(unsigned char channel);
#define CHANNEL0 0x00   
#define CHANNEL1 0x01 
#define CHANNEL5 0x05
#endif