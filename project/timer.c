#include "timer.h"


volatile uint32_t timerCount = 0;
volatile int freq = 392;
volatile unsigned state = 0;

void timer0_init(void)
{
    TCCR0A |= (1 << WGM01);
    TIMSK0 |= (1 << OCIE0A);
    OCR0A = 79;
}

ISR(TIMER0_COMPA_vect){
    PORTC ^= (1 << PC1);
	timerCount ++;
	


	if (timerCount >= freq){
		TCCR0B &= ~(1 << CS02);
	}
}

void timer1_init(void)
{
    // Add code to configure TIMER1 by setting or clearing bits in
    // the TIMER1 registers.

    // sets to the CTC mode ->clear
    TCCR1B |= (1 << WGM12);

   
    TIMSK1 |= (1 << OCIE1A); //interrupt - stops and calls ISR to complete action
    OCR1A = 62500; //when it reaches this prescalar it triggers the ISR

    //sets prescaler to 256 and starts at 0;
     //so counting up to 256 at interrupt increment

}

ISR(TIMER1_COMPA_vect)
{
    TCCR1B &= ~((1 << CS12) | (1 << CS10));
    
    state = 3;
}

void timer2_init(void)
{
    TCCR2A |= (0b11 << WGM20);  // Fast PWM mode, modulus = 256
    TCCR2A |= (0b10 << COM2A0); // Turn D11 on at 0x00 and off at OCR2A
    OCR2A = 35;                // Initial pulse duty cycle of 50%
    TCCR2B |= (0b111 << CS20);  // Prescaler = 1024 for 16ms period
    
}