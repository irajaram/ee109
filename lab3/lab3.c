/********************************************
*
*  Name: Isai Rajaraman
*  Email: irajaram@usc.edu
*  Lab section:
*  Assignment: Lab 3 - Arduino Input and Output
*
********************************************/

#include <avr/io.h>
#include <util/delay.h>

#define DOT_LENGTH  250         /* Define the length of a "dot" time in msec */

void dot(void);
void dash(void);
void makeOutput(char);
char checkInput(char);

int main(void)
{

    // Initialize appropriate DDR registers
    DDRB|=(1<<DDB2);
  
    // Initialize the LED output to 0
    PORTB &= ~(1 << PB4);
	
    // Enable the pull-up resistors for the 
    // 3 button inputs 
    PORTB |= (1 << 3);
    PORTB |= (1 << 4);
    PORTB |= (1 << 5);
    // Loop forever
    
    while (1) {                 
    
	//  Use "if" statements and the checkInput()
	//  function to determine if a button
	//  is being pressed and then output
	//  the correct dot/dash sequence by
	//  calling the dot() and dash(), and 
	//  using appropriate delay functions
      if (checkInput(3)) {   // Is button 1 pressed?
        dot();
        dot();
        dash(); 
        _delay_ms(DOT_LENGTH*3); 
      }  

      else if(checkInput(4)) {
        dot();
        dot();
        dot();      
        _delay_ms(DOT_LENGTH*3);
      }

      else if(checkInput(5)){
        dash();
        dot();
        dash();
        dot();
        _delay_ms(DOT_LENGTH*3);
      }  
      
    }
      return 0;   /* never reached */
  }

/*
  dot() - Makes the output LED blink a "dot".

  Write code to generate a dot by using the makeOutput function
  to turn the output ON and OFF with appropriate delays.
  Be sure you don't forget to also delay an
  appropriate time after you turn the output off.
*/
void dot()
{
  makeOutput(1);
  _delay_ms(DOT_LENGTH);
  makeOutput(0);
  _delay_ms(DOT_LENGTH);
}

/*
  dash() - Makes the output LED blink a "dash".

  Write code to generate a dash by using the makeOutput function
  to turn the output ON and OFF with appropriate delays.
  Be sure you don't forget to also delay an
  appropriate time after you turn the output off.
*/
void dash()
{
 makeOutput(1);
  _delay_ms(DOT_LENGTH * 3);
  makeOutput(0);
  _delay_ms(DOT_LENGTH * 3);
 
}

/*
  makeOutput() - Changes the output bit (Group D, bit 2) to either
  a zero or one, based on the input argument "value".
  
  If the argument is zero, turn the output OFF,
  otherwise turn the output ON. 
  
  Do not use any delays here.  Just use bit-wise operations
  to make the appropriate PORT bit turn on or off.
*/
void makeOutput(char value)
{
  if(value ==0){
    PORTD &= ~(1 << PD2);
  }
  else{
    PORTD |= (1 << PD2);
  }
}

/*
  checkInput(bit) - Checks the state of the input bit in Group B specified by
  the "bit" argument (0-7), and returns 1 if the button is pressed, or 0 if
  the button is not pressed.
  
  Write code to use the appropriate group's PIN register and determine if  
  the specified bit (which is passed as the argument) of that group is
  pressed or not.  Think carefully about what bit value means "pressed"
  when connected to a pushbutton.
 */
char checkInput(char bit)
{
  if((PINB & (1<<bit))!=0){ //getting the whole 8 bit and then checking if at the specific bit it is 0 or 1 (0 is pressed or 1 is open)
    return 0;
    
  }
  else{
    return 1;
  }
}


