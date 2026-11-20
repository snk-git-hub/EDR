#include <xc.h>
#include "adc.h"

void init_adc(void)
{
    /* TODO 1: Configure ADC result justification. */
    /* TODO 2: Configure the required analog input channel and ADC clock. */
    /* TODO 3: Enable the ADC module. */
}

unsigned short read_adc(void)
{
    unsigned short adc_reg_val;

    /* TODO 4: Start ADC conversion. */
    /* TODO 5: Wait until conversion is complete. */
    /* TODO 6: Combine ADRESH and ADRESL into a 10-bit result. */
    /* TODO 7: Return the ADC result (0-1023). */
    return 0; /* Replace with the converted value. */
}
