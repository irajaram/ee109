#ifndef PROJECT_H
#define PROJECT_H

#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>

void play_note();
void variable_delay_us(int16_t);



extern volatile uint8_t new_state, old_state; //
extern volatile uint8_t changed; //  // Flag for state change
extern volatile uint16_t count;     // Count to display

extern uint8_t a, b;//
extern uint8_t started, stopped; //cp 1
extern uint8_t start_indicator; //for when start and stop at same time


//checkpoint 2
extern volatile uint8_t speed_threshold; //
extern uint8_t default_speed; //
extern volatile uint32_t speed; //

//checkpoint 3
extern volatile uint16_t remSpeed; //

#endif
