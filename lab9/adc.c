#include <avr/io.h>

#include "adc.h"


void adc_init(void)
{
    // Initialize the ADC
    ADMUX &= ~(1 << REFS1);
    ADMUX |= (1 << REFS0);
    ADCSRA |= (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2);
    ADMUX |= (1 << ADLAR);
    ADCSRA |= (1 << ADEN);

    
}

uint8_t adc_sample(uint8_t channel)
{
    // Set ADC input mux bits to 'channel' value
    //clear the last four by anding 0xF0 (11110000)
    //set just the last 4 bits (what do you and channel with to keep just the last four bits?)
    ADMUX &= ~(0x0F);
    ADMUX |= (channel & 0x0F);

    // Convert an analog input and return the 8-bit result
    ADCSRA |= (1 << ADSC);
    while((ADCSRA & (1<<ADSC)) != 0){
        
    }
    unsigned char final_result = ADCH;
    return final_result;

}
