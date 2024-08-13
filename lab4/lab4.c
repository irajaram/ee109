/********************************************
 *
 *  Name: Yazhisai Rajaraman
 *  Email:irajaram@usc.edu
 *  Section:
 *  Assignment: Lab 4 - Up/Down counter on LCD display
 *
 ********************************************/

#include <avr/io.h>
#include <util/delay.h>

#include "lcd.h"

enum states
{
    UP,
    DOWN,
    PAUSE
};

int main(void)
{

    // Setup DDR and PORT bits for the 3 input buttons as necessary
    // automatically zero meaning input so no need to initalize as input
    PORTC |= (1 << PC2);
    PORTB |= (1 << PB3);
    PORTC |= (1 << PC4);

    // Initialize the LCD
    lcd_init();
    lcd_writecommand(1);

    // Use a state machine approach to organize your code
    //   - Declare and initialize a variable to track what state you
    //     are in by assigning the values UP, DOWN or PAUSE to that variable.
    unsigned char count = 0;
    unsigned char state = UP;
    int clock = 0;

    while (1)
    { // Loop forever
        // lcdwritedata(0);
        //  Use "if" statements to read the buttons and determine which
        //  state you are in
        if ((PINC & (1 << PC2)) == 0)
        {
            state = UP;
        }

        else if ((PINB & (1 << PB3)) == 0)
        {
            state = PAUSE;
        }

        else if ((PINC & (1 << PC4)) == 0)
        {
            state = DOWN;
        }

        // Based on the current state, determine the new count value
        if (clock % 10 == 0) {
            if (state == UP)
            {
                if (count == 9)
                {
                    count = 0;
                }
                else
                {
                    count++;
                }
                lcd_writecommand(2);
                lcd_writedata(count + 0x30);
            }

            else if (state == DOWN)
            {
                if (count == 0)
                {
                    count = 9;
                }
                else
                {
                    count--;
                }
                lcd_writecommand(2);
                lcd_writedata(count + 0x30);
            }

            else if (state == PAUSE)
            {
                count = count;
            }
        }

        // Perform the output operations if necessary

        // Delay before we go to the next iteration of the loop
        clock++;
        if (clock == 10){
            clock = 0;
        }
        _delay_ms(50);
    }

    return 0; /* never reached */
}
