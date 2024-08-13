/********************************************
 *
 *  Name:Isai Rajaraman 
 *  Email: irajaram@usc.edu
 *  Section:
 *  Assignment: Lab 5 - Analog-to-digital conversion
 *
 ********************************************/

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h> //in order to use rand()

#include "lcd.h"
#include "adc.h"

void rand_init(void);

int main(void)
{
    int user_number[3] = {0};
    // Initialize the LCD
    lcd_init();

    //clearing screen
    //lcd_writecommand(0x01);

//     /* Call lcd_stringout to print out your name */
//     lcd_stringout("Isai Rajaraman");
//     /* Use snprintf to create a string with your birthdate */
//     char date[11];
//     unsigned char month, day, year;
//     month = 8;
//     day = 15;
//     year = 04;
//     snprintf(date, 11, "%d/%d/%d", month, day, year);
    

//    /* Use lcd_moveto to start at an appropriate column 
//     in the bottom row to appear centered */
//     lcd_moveto(1, 3);
//    /* Use lcd_stringout to print the birthdate string */
//     lcd_stringout(date);
//     /* Delay 1 second */
//     _delay_ms(1000);
//     lcd_writecommand(0x01); 
    // Initialize the ADC
    adc_init();

    // Initialize the random number function
    rand_init();
    lcd_writecommand(1);
    // Write splash screen and delay for 1 second
    lcd_moveto(0,0);
    lcd_stringout("EE109 Lab 5");
    lcd_moveto(1,0); //moves cursor to next line
    lcd_stringout("Isai Rajaraman");

    _delay_ms(2000);

    lcd_writecommand(1);
    // Find three random numbers and display on top line
    int randnum1, randnum2, randnum3;
    randnum1 = rand() % 32;
    randnum2 = rand() % 32;
    randnum3 = rand() % 32;
    lcd_moveto(0,1);
    char temp[9]; // size 9 to include spaces between each digit
    snprintf(temp, 9, "%2d %2d %2d", randnum1, randnum2, randnum3);
    lcd_stringout(temp);

    // Show the number selector ('>') on the bottom line
    lcd_moveto(1,0);
    lcd_stringout(">");

    int curspos = 0; //initializng position of cursor to position of first number of combo

    while (1) {                 // Loop forever
	// Convert ADC channel for buttons to 0-255
        // uint8_t adc_result = adc_sample(3);
        // char buf[5];
        // snprintf(buf, 5, "%4d", adc_result);
        // lcd_moveto(1, 0);
        // lcd_stringout(buf);

        // Convert ADC channel for buttons to 0-255
        uint8_t adc_result = adc_sample(0);

        // If right or left button pressed, move '>' to new position
        if ((adc_result==155) && (curspos>0)){ //moving to left
            lcd_moveto(1, curspos);
            lcd_stringout(" ");
            curspos-=3; // decrease by 3 because 2 digits so need cursor to go one past last digit
            lcd_moveto(1,curspos);
            lcd_stringout(">");
            _delay_ms(500);
        }

        else if((adc_result==0)&&(curspos<6)){ //moving to right
            lcd_moveto(1, curspos);
            lcd_stringout(" ");
            curspos+=3; // increase by 3 because 2 digits so need cursor to go one past last digit
            lcd_moveto(1,curspos);
            lcd_stringout(">");
            _delay_ms(500);
        }
        // Read potentiometer ADC channel
        unsigned char pot_val = adc_sample(3);

	// Convert ADC channel for potentiometer to 0-255, change to 0-31 and display
    int pot_num = pot_val / 8; //converting 0-255 to 0-31
    lcd_moveto(1,12);
    char temp[3]; //max is 2 character with space for null character
    snprintf(temp, 3, "%d", pot_num);
    lcd_stringout("  ");
    lcd_moveto(1,12);
    lcd_stringout(temp);

	// If select button pressed copy number to one of the lock inputs positions
    if(adc_result == 205){
        lcd_moveto(1, curspos +1);
        lcd_stringout(temp);
        user_number[curspos / 3] = pot_num;
        _delay_ms(100);
    }

	// Check to see if all three lock inputs match the combination
    if((user_number[0]== randnum1) &&(user_number[1]== randnum2)&&(user_number[2]== randnum3)){
        lcd_writecommand(0x01);
        lcd_moveto(0,0);
        lcd_stringout("Unlock");
        while (1){
            
        }
    }

    }

    return 0;   /* never reached */
}

void rand_init()
{
    int16_t seed = 0;
    uint8_t i, j, x;

    // Build a 15-bit number from the LSBs of an ADC
    // conversion of the channels 1-5, 3 times each
    for (i = 0; i < 3; i++) {
	for (j = 1; j < 6; j++) {
	    x = adc_sample(j);
	    x &= 1;	// Get the LSB of the result
	    seed = (seed << 1) + x; // Build up the 15-bit result
	}
    }
    srand(seed);	// Seed the rand function
}
