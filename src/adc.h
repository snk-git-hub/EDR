#ifndef ADC_H
#define ADC_H

void init_adc(void);
unsigned short read_adc(unsigned char channel);
#define CHANNEL5 0x05
#endif
