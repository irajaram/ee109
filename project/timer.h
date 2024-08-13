#ifndef TIMERS_H
#define TIMERS_H

#include <avr/io.h>
#include <avr/interrupt.h>

extern volatile uint32_t timerCount;
extern volatile int freq;
extern volatile unsigned state;

void timer1_init(void);
void timer0_init(void);
void timer2_init(void);

ISR(TIMER1_COMPA_vect);
ISR(TIMER0_COMPA_vect);

#endif