
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <avr/eeprom.h>
#include "project.h"

char remoteData[10]; //buffer to store data from remote sensor
uint8_t dataStarted = 0;  //act as a data started flag 
uint8_t bufferCount= 0; //number of character received and stored in buffer
uint8_t dataValid = 0; //flag that valid data received


char rx_char()
{
    // Wait for receive complete flag to go high
    while ( !(UCSR0A & (1 << RXC0)) ) {}
    char string[2];
    // string[0] = UDR0;
    // string[1] ='\0';
    // lcd_writecommand(1);
    // lcd_stringout("r ");
    // lcd_stringout(string);
    // _delay_ms(500);
    
    return UDR0;

    
}
void tx_char(char ch)
{
    // Wait for transmitter data register empty
    while ((UCSR0A & (1<<UDRE0)) == 0) {}
    UDR0 = ch;
    // char string[2];
    // string[0] = ch;
    // string[1] ='\0';
    // lcd_writecommand(1);
    // lcd_stringout(string);
    // _delay_ms(500);
}

ISR(USART_RX_vect)
{
   //UCSR0B |= (1 << RXCIE0);
//    lcd_writecommand(1);
//     lcd_stringout("entered");
//    _delay_ms(1000);

   char recChar;
   recChar = UDR0;
    if(recChar == '{'){
        dataStarted = 1;
        bufferCount = 0;
        dataValid = 0;
    }   
    else if(dataStarted && !dataValid && recChar >= '0' && recChar <='9'){
        if(bufferCount < 4){
            remoteData[bufferCount] = recChar;
            bufferCount ++;
        }
        else{
            dataStarted = 0;
            bufferCount = 0;
        }

    }
    else if(recChar == '}'){
        if (bufferCount > 0)
        {
            dataValid=1;
        }
        else{
            dataValid = 0;
            bufferCount = 0;
            dataStarted = 0;
        }
        

    }
   
    else if((recChar<'0') || (recChar > '9') || (recChar!='}')){
        dataStarted = 0; 
        bufferCount = 0;  
        dataValid = 0;
    }
}