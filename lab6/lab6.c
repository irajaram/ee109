/********************************************
 *
 *  Name: Yazhisai Rajaraman
 *  Email:
 *  Section:
 *  Assignment: Lab 6 - Timers
 *
 ********************************************/

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "lcd.h"

void debounce(uint8_t);
volatile unsigned int1 = 0; //decimal digit
volatile unsigned int2 = 0;//middle digit
volatile unsigned int3 = 0; // beginning digit
//3 ints of the time 


void timer1_init(void);

enum states { PAUSE, STARTRUN, RUN, LAPPED };
volatile unsigned state = PAUSE;

int main(void) {

    uint8_t state;


    // Initialize the LCD and TIMER1
    
    lcd_init();
    timer1_init();

    // Enable pull-ups for buttons
    PORTC |= (1 << PC2) | (1 << PC4); //PC2 = start/stop  PC4= lap/reset

    // Show the splash screen
    
    lcd_stringout("EE109 Lab 6");
    lcd_moveto(1,0); //moves cursor to next line
    lcd_stringout("Isai Rajaraman");
    _delay_ms(1000);
    lcd_writecommand(0x01);
    lcd_moveto(0,0);
    lcd_stringout(" 0.0");

    // Enable interrupts
    lcd_moveto(0,0);
        lcd_writecommand(0x01);
        if(int3 > 0){
            lcd_writedata(int3 + '0');
        }
        else{
            lcd_writedata(' ');
        }
        lcd_writedata(int2 + '0');
        lcd_writedata('.');
        lcd_writedata(int3 + '0');
        _delay_ms(100);
        
    sei();

    
    while (1) {                 // Loop forever


	// Read the buttons
    if (state == PAUSE) {               // PAUSE state
        if((PINC & (1 << PC2)) == 0) {
            //debounce(2); /*using main while loop as debouncer*/
            state = STARTRUN;
            TCCR1B |= (1<< CS12);
        }
        if((PINC & (1 <<PC4)) ==0){
            debounce(4);
            int1 = 0;
            int2 = 0;
            int3 =0;
            lcd_moveto(0,0);
            lcd_stringout(" 0.0");
        }
    
    //if button is pressed, 

	}

    if(state == STARTRUN){
        if((PINC & (1 << PC2)) == 0) {
            state = STARTRUN;
        }
        else{
            state = RUN;

        }
    }
	else if (state == RUN) {            // RUN state
    //run  
        if((PINC & (1 << PC2)) == 0) {
            debounce(2);
            state = PAUSE;
            TCCR1B &= ~(1<< CS12);
        }

        if((PINC & (1 <<PC4)) ==0){
            debounce (4);
            state = LAPPED;
        }

	}
	else if (state == LAPPED) {         // LAPPED state
        if((PINC & (1 << PC2)) == 0){
            debounce(2);
            state = RUN;
            //TCCR1B &= ~ (1<< CS12);
        }
        if((PINC & (1 << PC4)) == 0){
            debounce(4);
            state = RUN;
        }



	}

	// If necessary write time to LCD
    if(state == RUN || state == STARTRUN){
        lcd_moveto(0,0);
        lcd_writecommand(0x01);
        if(int3 > 0){
            lcd_writedata(int3 + '0');
        }
        else{
            lcd_writedata(' ');
        }
        lcd_writedata(int2 + '0');
        lcd_writedata('.');
        lcd_writedata(int1 + '0');
        _delay_ms(100);
    }


    }

    return 0;   /* never reached */
}

/* ----------------------------------------------------------------------- */

void debounce(uint8_t bit)
{
    // Add code to debounce input "bit" of PINC
    // assuming we have sensed the start of a press.
        _delay_ms(5);
        while( (PINC & 1<<bit) == 0){}
        _delay_ms(5);

}

/* ----------------------------------------------------------------------- */

void timer1_init(void)
{
    // Add code to configure TIMER1 by setting or clearing bits in
    // the TIMER1 registers.

    // sets to the CTC mode ->clear
    TCCR1B |= (1 << WGM12);

   
    TIMSK1 |= (1 << OCIE1A); //interrupt - stops and calls ISR to complete action
    OCR1A = 6250;

    //sets prescaler to 256 and starts at 0;
    TCCR1B |= (1 << CS12); //so counting up to 256 at interrupt increment

}

ISR(TIMER1_COMPA_vect)
{
    // Increment the time
    //PORTC ^= (1 << PC5);    // Flip PC5 each time ISR is run
    
    //when the decimal reaches ten, set it to zero and increment the middle number
    //when the middle number reaches ten, change beginning number
    int1++;
    if(int1 >= 10){
        int1= 0;
        int2++;
    }

    if(int2>=10){
        int2 = 0;
        int3++; 
    }

    if(int3>=6){
        int3 = 0;
    }

}
