/*
 * Lock_Box.c
 *
 * 
 *  Author: Coda Kama
 */ 

#define F_CPU 1000000			// 8Mhz/8 = 1Mhz = System clock

#include <avr/io.h>				// AVR I/O library
#include <util/delay.h>			// Delay library
#include <avr/interrupt.h>		// Interrupt library
#include <avr/eeprom.h>			// EEPROM library

#include "LCD.h"				// LCD.h function library - Prototypes only
#include "LCD.c"				// LCD.c function library - Functions only
#include "EEPROM_RW.h"			// EEPROM_RW.h - EEPROM read/write function prototypes
#include "EEPROM_RW.c"			// EEPROM_RW.c - EEPROM read/write functions
#include "keypad.h"				// keypad.h function library - Function prototypes
#include "keypad.c"				// keypad.c function library - Functions only
#include "lockCode.h"			// lockCode.h function library - Function prototypes
#include "lockCode.c"			// lockcCocde.h function library - Functions only


void initRows(int r[]);
void initColumns(int c[]);

ISR(PCINT0_vect){
	_delay_ms(5);
	if(getButtonState()){
		getKeyPress();
		_delay_ms(50);
	}
	else if(!getButtonState()){
		pushKey(current_key);
		writeLCDcharacter(key_queue[0]);
	}
}

int main(void)
{
	// initialize
	int rows[] = {ROW1, ROW2, ROW3, ROW4};
	int cols[] = {COL1, COL2, COL3};
	int count;											// counter variable used for loops
	int code_is_correct;								 
	int lock_state = 2;									// initialized lock variable to intermediate status		
	
	// initialize interrupts
	PCICR |= (1 << PCIE0);								// set bit 0 of PCICR
	for(int i = 0; i < NUM_ROWS; i++){
		PCMSK0 |= (1 << rows[i]);						// allow row pins to trigger interrupts
	}
	sei();												// global interrupt enable
	
	// Initializing arrays 
	int enter_code[16] = {'E','N','T','E','R',' ','C','O','D','E',' ',' ',' ',' ',' ',' '};
	int incorrect_code[16] = {'I','N','C','O','R','R','E','C','T',' ','C','O','D','E',' ',' '};
	int current_code[9] = {
	
	initRows(rows);										// set keypad rows as inputs
	initColumns(cols);									// set keypad columns as outputs
	initializeLCD();									// set up LCD and initialize in 4 bit mode
	clearKeyQueue();
	clearLCD();
	
	
	// wait loop
	while(1)											
	{
		// Print greeting
		writeLCDline(enter_code,1);
		cursorPosition(2);
		
		// Wait for a key press
		while(key_queue[0] == '\0');
		
		// Now that we have a key press we need to look at what was pressed.
		// Is there first key in the queue a '#'...
		if(key_queue[0] == '#'){
			clearLCD();
			clearKeyQueue();
		
		
		// ...or is the first key in the queue a digit?...
		} else if(key_queue[0] >= '0' && key_queue[0] <= '9'){
			writeLCDcharacter(key_queue[0]);
		
		// ...or if first key in the queue isn't a digit or '#' then
		// it must be the '*'
		} else {
			count = 0;
			code_is_correct = 1;
			// Checks the current queue code with the correct code
			while(key_queue[count] != '\0'){
				if(key_queue[count] != current_code[count])
					code_is_correct = 0;				
			}
			if(!code_is_correct){
				clearLCD();
				writeLCDline(incorrect_code[],1);
				_delay_ms(5000);
				clearKeyQueue();
				clearLCD();
			}else{
				lock_state = unlock(lock_state);
			}
		}		
		
	}
}

void initRows(int r[]){									// sets keypad rows as inputs
	for(int i = 0; i < NUM_ROWS; i++){
		DDRB &= ~(1 << r[i]);
	}
}

void initColumns(int c[]){								// sets keypad columns as outputs
	for(int i = 0; i < NUM_COLS; i++){
		PORTB &= ~(1 << c[i]);							// columns to output low
		DDRB |= (1 << c[i]);							// set columns as outputs
	}
}