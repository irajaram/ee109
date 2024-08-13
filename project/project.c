/********************************************
 *
 *  Name: Isai Rajaraman
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
#include <avr/eeprom.h>

#include "lcd.h"
#include "project.h"
#include "timer.h"
#include "serial.h"



void play_note();
void variable_delay_us(int16_t);
void timer1_init(void);
void timer0_init(void);
void timer2_init(void);
// Frequencies for natural notes from middle C (C4)
// up one octave to C5.


volatile uint8_t new_state, old_state;
volatile uint8_t changed = 0;  // Flag for state change
volatile uint16_t count = 0;     // Count to display

uint8_t a, b;
uint8_t started, stopped; //cp 1
uint8_t start_indicator = 0; //for when start and stop at same time


//checkpoint 2
volatile uint8_t speed_threshold = 0;
uint8_t default_speed = 40;
volatile uint32_t speed = 0;


volatile uint16_t remSpeed = 0;

int main(void) {
    uint8_t x;
    uint8_t y;
    lcd_init();
    // Initialize DDR and PORT registers and LCD
    //DDRC &= ~((1 << PC4) | (1 << PC5));
    PORTC |= (1 << PC4) | (1 << PC5);
    DDRB |= (1 << PB5); 
    DDRB |= (1 << PB3);

    DDRD &= ~((1 << PD2) | (1 << PD3));
    PORTD |= (1 << PD2) | (1 << PD3); // cp 2 enable pull up

    DDRC |= (1 << PC1); // set buzzer output
    
    PCICR |= (1 << PCIE1);
    PCICR |= (1 << PCIE2);
    PCMSK1 |= ((1 << PCINT12)| (1 << PCINT13));
    PCMSK2 |= ((1 << PCINT18) | (1 << PCINT19));

    //checkpoint 3
    UBRR0 = MYUBRR; // Set baud rate (rate at which data is transmitted)
    UCSR0B |= (1 << TXEN0 | 1 << RXEN0); // Enable RX and TX
    UCSR0C = (3 << UCSZ00); // Async., no parity,
                        // 1 stop bit, 8 data bits 
    UCSR0B |= (1 << RXCIE0);        
    //LED segment
    DDRC |= (1<<PC2) | (1<<PC3);
    PORTC |= (1<<PC2) | (1<<PC3);
    DDRB |= (1 <<PB4);
    PORTB &= ~(1<<PB4);
    DDRD |= (1 <<PD1);
    

    sei(); //enables interrupts
    //timer2_init();
    

    lcd_writecommand(0x01);
    // Write a spash screen to the LCD
    lcd_stringout("EE109 Project");
    lcd_moveto(1,0); //moves cursor to next line
    lcd_stringout("Isai Rajaraman");
    _delay_ms(2000);
    lcd_writecommand(0x01);

    timer1_init();
    timer0_init();
    timer2_init();
   
    

    unsigned char x1;
    x1 = eeprom_read_byte((void *) 100);
    if(x1>=1 && x1<=99){
        speed_threshold = (int)x1;
        count = speed_threshold;
    }
    else{
        speed_threshold = 60;
        changed = 1;
    }

    char buf[30];
    snprintf(buf, sizeof(buf), "%4d", speed_threshold);
    lcd_writecommand(0x01);
    lcd_moveto(0, 0); // Move cursor to the first line
    lcd_stringout("Speed Threshold:");
    lcd_moveto(1,0);
    lcd_stringout(buf); 
    
    while (1) {                 // Loop forever

    
       /* uint8_t x;
        x = PINC;
        a = PINC & (1 << PC4);*/

    // lcd_writecommand(1);
    // char str[10]; // Make sure the array is large enough to hold the string
    // sprintf(str, "%d", speed);
    // lcd_stringout(str);

    // lcd_moveto(1,0);
    // char str2[10];
    // sprintf(str2, "%d", remSpeed);
    // lcd_stringout(str2);
    // _delay_ms(2000); 

    // lcd_writecommand(1);


    if ((remSpeed > 0) && (speed > 0)) {
        int difference = (int)remSpeed - (int)speed;
        if ((difference) > 30){
            PORTC &= ~(1<<PC2); //red on
            PORTC|= (1<<PC3); //blue off
            // lcd_stringout("red");
            // _delay_ms(1000);
            
         }
        else if((difference) < -30){
            PORTC &= ~(1<<PC3); //turn blue on
            PORTC |= (1<<PC2); //turn red off
            // lcd_stringout("blue");
            // _delay_ms(1000);
        }
        else {
            PORTC |= (1<<PC2); //red off
            PORTC |= (1<<PC3); //blue off
            // lcd_stringout("green");
            // _delay_ms(1000);
        }
    }

        char buf[30];
        //check point 2
        if(changed){
            unsigned char speed_char = (unsigned char) speed_threshold;
            eeprom_update_byte((void *) 100, speed_char);
            changed = 0;
            speed_threshold = count;

            snprintf(buf, sizeof(buf), "%4d", speed_threshold);
            lcd_writecommand(0x01);
			lcd_moveto(0, 0); // Move cursor to the first line
            lcd_stringout("Speed Threshold:");
            lcd_moveto(1,0);
			lcd_stringout(buf); 
        }

        if(state ==1){
            lcd_writecommand(1);
            lcd_stringout("started");

            state = 0;

            //LED on
            PORTB |= (1 << PB5);
            
        }

        else if(state == 2){
            PORTB &= ~(1 << PB5);

            lcd_writecommand(1);
            lcd_stringout("stopped");
            _delay_ms(1000);


            lcd_writecommand(1);
            uint32_t msecs = timerCount * 8 / 125;
            char temp[11]; // size 9 to include spaces between each digit
            snprintf(temp, 11, "%d", msecs);
            lcd_stringout("Time: ");
            lcd_stringout(temp);
            lcd_stringout(" ms");
            //distance 1.7 in 


            speed = 43180 / msecs;
            char buff[11]; // size 9 to include spaces between each digit
            snprintf(buff, 11, "%ld.%ld", speed/10, speed%10);  // in order ot get it in milliseconds
            lcd_moveto(1,0);
            lcd_stringout("Speed: ");
            lcd_stringout(buff);
            lcd_stringout(" cm/s");

            state = 0;
            if(speed / 10 > speed_threshold){
                play_note();
            }

            //checkpoint 2 servo motor
            //OCR2a min = 12
            //OCR2a max = 35
            OCR2A = ((-23)*(speed/10) +3500) / 100;
            /*char temp[11]; // size 9 to include spaces between each digit
            snprintf(temp, 11, "%d", OCR2A);
            lcd_moveto(0,0);
            lcd_stringout("OCR2A: ");
            lcd_stringout(temp);*/
            _delay_ms(2000); //comment out


            char str[5];
			snprintf(str, 11, "%ld", speed);
            tx_char('{');
			for (int i = 0; str[i] != '\0'; i++) {
				tx_char(str[i]);
			}
			tx_char('}');

        }

        else if(state == 3){
            lcd_writecommand(1);
            lcd_stringout("Timer expired");

            PORTB &= ~(1 << PB5);

            state = 0;
        }

         //checkpoint 3
        if(dataValid){
            //function that converts speed data to single fixe dpoint number
            //lcd_writecommand(1);
            // lcd_stringout("here");
            //lcd_moveto(0,1);
            //lcd_stringout(remoteData);
            dataValid = 0;
			sscanf(remoteData, "%ld", &remSpeed);
			lcd_writecommand(1);
			lcd_moveto(0, 0);
			lcd_stringout("Remote: ");
			lcd_stringout(remoteData);
			_delay_ms(1000);

            // sscanf(remoteData,"%d", remSpeed);
            // lcd_writecommand(0x01);
            // lcd_moveto(0,0);
            // lcd_stringout("Remote Speed:");
            // lcd_moveto(1,0);
            // lcd_stringout(remSpeed);
            //dataValid = 0;

        }
    }


   

    // if(remSpeed!=0 && abs(speed - remSpeed) ==30){
    //     PORTC |= (1<<PC2); //red off
    //     PORTC |= (1<<PC3); //blue off
    // }



}



ISR(PCINT1_vect)
{
    // In Task 6, add code to read the encoder inputs and determine the new
    // count value
    uint8_t x = PINC;
    started = (x & (1 << PC4)) >> PC4;
    stopped = (x & (1 << PC5)) >> PC5;


    if(stopped){
        if(start_indicator==1){
            TCCR1B &= ~((1 << CS12) | (1 << CS10)); 
            timerCount = TCNT1;
            state = 2;
            start_indicator = 0;
        }        
    }

    else if(started){
       
        TCNT1 =0;
        TCCR1B |= ((1 << CS12) | (1 << CS10));
        start_indicator = 1;
        state = 1;
    }

    
}

void play_note()
{
    timerCount = 0;
	TCCR0B |= (1 << CS02); //starts the ti,mer
}


ISR(PCINT2_vect) //rotary encoder
{
	uint8_t y = PIND;
    a = (y & (1 << PD2)) >> PD2;
	b = (y & (1 << PD3)) >> PD3;
	if (old_state == 0) {

	    // Handle A and B inputs for state 0
		if(a && !b)
		{
			new_state = 1;
			count++;
		}
		else if(!a && b)
		{
			new_state = 2;
			count--;
		}

	}
	else if (old_state == 1) {

	    // Handle A and B inputs for state 1
		if(a && b)
		{
			new_state = 3;
			count++;
		}
		else if(!a && !b)
		{
			new_state = 0;
			count--;
		}


	}
	else if (old_state == 2) {

	    // Handle A and B inputs for state 2
		if(!a && !b)
		{
			new_state = 0;
			count++;
		}
		else if(a && b)
		{
			new_state = 3;
			count--;
		}

	}
	else {   // old_state = 3

	    // Handle A and B inputs for state 3
		if(!a && b)
		{
			new_state = 2;
			count++;
		}
		else  if(a && !b)
		{
			new_state = 1;
			count--;
		}

	}

    if(count > 99) {
        count = 99;
    }
    if(count <= 0){
        count = 1;
    }

    
    // If state changed, update the value of old_state,
    // and set a flag that the state has changed.
    if (new_state != old_state) {
        changed = 1;
        old_state = new_state;
        //OCR1B = pwmLen;
    }
	

}





