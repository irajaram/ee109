#ifndef SERIAL_H
#define SERIAL_H

#include <avr/io.h>
#include <avr/interrupt.h>

#define FOSC 16000000 // Clock frequency
#define BAUD 9600 // Baud rate used
#define MYUBRR (FOSC/16/BAUD-1) // Value for UBRR0

extern volatile char remoteData[10]; //buffer to store data from remote sensor
extern volatile uint8_t dataStarted;  //act as a data started flag 
extern volatile uint8_t bufferCount; //number of character received and stored in buffer
extern volatile uint8_t dataValid; //flag that valid data received

char rx_char();
void tx_char(char ch);

#endif