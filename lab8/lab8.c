/********************************************
 *
 *  Name:
 *  Email:
 *  Section:
 *  Assignment: Lab 7 - Rotary Encoder
 *
 ********************************************/

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#include "lcd.h"

void play_note(uint16_t);
void variable_delay_us(int16_t);

// Frequencies for natural notes from middle C (C4)
// up one octave to C5.


volatile uint8_t new_state, old_state;
volatile uint8_t changed = 0;  // Flag for state change
volatile int16_t count = 0;     // Count to display
volatile uint16_t timerCount = 0;
volatile uint16_t playLength = 0;
volatile uint8_t a, b;
volatile int16_t pwmLen = 125;

 void timer2_init(void);
  /*{/**
      TCCR2A |= (0b11 << WGM20);  // Fast PWM mode, modulus = 256
      TCCR2A |= (0b10 << COM2A0); // Turn D11 on at 0x00 and off at OCR2A
      OCR2A = 255;                // Initial pulse duty cycle of 50%
      TCCR2B |= (0b111 << CS20);  // Prescaler = 1024 for 16ms period
  }*/

int main(void) {
    uint8_t x;

    // Initialize DDR and PORT registers and LCD
    DDRC &= ~((1 << PC4) | (1 << PC5));
    PORTC |= (1 << PC4) | (1 << PC5);
    DDRB |= (1 << PB3); 
    DDRB|= (1 << PB2);
    
    PCICR |= (1 << PCIE1);
    PCMSK1 |= (1 << PCINT12)| (1 << PCINT13);
    sei(); //enables interrupts
    timer2_init();
    timer1_init();
    /*_delay_ms(1000);
    lcd_writecommand(0x01);
    lcd_moveto(0,0);
    lcd_stringout(" 0.0");*/


    // Read the A and B inputs to determine the intial state.
    // In the state number, B is the MSB and A is the LSB.
    // Warning: Do NOT read A and B separately.  You should read BOTH inputs
    // at the same time, then determine the A and B values from that value.    
    count = OCR2A;
    x = PINC;
    a = x & (1 << PC4);
    b = x & (1 << PC5);

    if (!b && !a) //00
        old_state = 0;
    else if (!b && a)//01
        old_state = 1;
    else if (b && !a)//10
        old_state = 2;
    else
        old_state = 3;//11

    new_state = old_state;
    

    while (1) {                 // Loop forever
        uint8_t x;
        x = PINC;
        a = PINC & (1 << PC4);
        

        if (changed) { // Did state change?
            changed = 0;        // Reset changed flag
        }
    }
}



/*
    variable_delay_us - Delay a variable number of microseconds
*/
void variable_delay_us(int delay)
{
    int i = (delay + 5) / 10;

    while (i--)
        _delay_us(10);
}

ISR(PCINT1_vect)
{
    // In Task 6, add code to read the encoder inputs and determine the new
    // count value
    //uint8_t a, b;
    uint8_t x;
    x = PINC;
    int i =12;
    a = x & (1 << PC4);
    b = x & (1 << PC5);

    if (old_state == 0) {

            // Handle A and B inputs for state 0
            if(a && !b){
                new_state =1;
                count++;
                pwmLen+=i;
            }
            else if(!a && b){
                new_state = 2;
                count--;
                pwmLen-=i;
            }

        }
        else if (old_state == 1) {

            // Handle A and B inputs for state 1
            if(a && b){
                new_state = 3;
                count++;
                pwmLen+=i; 
            }
            else if(!a && !b){
                new_state = 0;
                count--;
                pwmLen-=i;
            }
        }
        else if (old_state == 2) {

            // Handle A and B inputs for state 2
            if(!a && !b){
                new_state = 0;
                count++;
                pwmLen+=i;
            }
            else if(a && b){
                new_state = 3;
                count--;
                pwmLen-=i;
            }
        }
        else {   // old_state = 3

            // Handle A and B inputs for state 3
            if(!a && b){
                new_state = 2;
                count++;
                pwmLen+=i;
            }
            else if (a && !b){
                new_state = 1;
                count--;
                pwmLen-=i;
            }

        }

        if(count > 255) {
            count = 255;
        }
        if(count < 0){
            count = 0;
        }

        if (pwmLen > 141) {
            pwmLen = 141; // Maximum pulse width
        } 
        if (pwmLen < 47) {
            pwmLen = 47; // Minimum pulse width
        }

        OCR2A = count;
        OCR1B = pwmLen;

        // If state changed, update the value of old_state,
        // and set a flag that the state has changed.
        if (new_state != old_state) {
            changed = 1;
            old_state = new_state;
            //OCR1B = pwmLen;
        }

        

}


void timer1_init()
{
    // In Task 7, add code to inititialize TIMER1, but don't start it counting
    // sets to the CTC mode ->clear
    TCCR1A |= (1 << WGM10)| (1<<WGM11);
    TCCR1B |= (1 << WGM12)| (1<<WGM13);
    //TIMSK1 |= (1 << OCIE1A); //interrupt - stops and calls ISR to complete action

    TCCR1A |= (1 << COM1B1); 
    OCR1A = 1250; //prescalar:256
    OCR1B = 94;
    TCCR1B |= (1<<CS12);
}

void timer2_init(void)
{
TCCR2A |= (0b11 << WGM20);  // Fast PWM mode, modulus = 256
TCCR2A |= (0b10 << COM2A0); // Turn D11 on at 0x00 and off at OCR2A
OCR2A = 255;                // Initial pulse duty cycle of 50%
TCCR2B |= (0b111 << CS20);  // Prescaler = 1024 for 16ms period
}






