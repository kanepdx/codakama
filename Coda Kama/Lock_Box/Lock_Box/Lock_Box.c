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

<<<<<<< HEAD
int main(void)
{
    while(1)
    {
		//TODO:: Please write your application code 
    }
}
=======

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

int main(void) {
	// initialize
	int rows[] = {ROW1, ROW2, ROW3, ROW4};
	int cols[] = {COL1, COL2, COL3};
	int count;											// counter variable used for loops
	int count_queue;									// Keep track of how many characters are
														// in the queue
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
	int unlocked_menu_1[16] = {'#',' ','-',' ','T','O',' ','L','O','C','K',' ',' ',' ',' ',' ',};
	int unlocked_menu_2[16] = {'*',' ','-',' ','S','E','T',' ','N','E','W',' ','C','O','D','E',};
	int new_code_menu[16] = {'S','E','T',' ','N','E','W',' ','C','O','D','E',' ',' ',' ',' ',};
	int cancel_code_change[16] = {' ',' ','-','-','C','A','N','C','E','L','E','D','-','-',' ',' ',};
	int current_code[9];
	
	initRows(rows);										// set keypad rows as inputs
	initColumns(cols);									// set keypad columns as outputs
	initializeLCD();									// set up LCD and initialize in 4 bit mode
	clearKeyQueue();
	count_queue = 0;


	// Ensure we're locked to start with.
	while (lock_state != 1) {
			lock_state = lock(2);
	}

	
	// wait loop
	while(1) {

		// Print greeting
		clearLCD();
		writeLCDline(enter_code,1);
		cursorPosition(2);

		// While box is in locked state
		while (lock_state == 1) {

				// Wait for a key press
				while(key_queue[count_queue] == '\0');
				
				// Now that we have a key press we need to look at what was pressed.
				// Is the first key in the queue a '#'...
				if(key_queue[0] == '#'){
					clearLCD();
					clearKeyQueue();
					count_queue = 0;
				
				// ...or is the first key in the queue a digit?...
				} else if(key_queue[0] >= '0' && key_queue[0] <= '9'){
					// QUESTION: DO WE NEED TO MANUALLY SHIFT THE QUEUE??
					writeLCDcharacter(key_queue[0]);
					++count_queue;
				
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
						writeLCDline(incorrect_code,1);
						_delay_ms(5000);
						clearKeyQueue();
						count_queue = 0;
						clearLCD();
					}else{

						// Unlock the box
						lock_state = unlock(lock_state);
					}
				}		
			} // End while (lock_state == 1)

		// While box is in unlocked state
			while (lock_state == 0 ) {

				// Write unlocked menu
				clearLCD();
				writeLCDline(unlocked_menu_1, 1);
				writeLCDline(unlocked_menu_2, 2);

				clearKeyQueue();
				count_queue = 0;

				// Wait for keypress
				while(key_queue[count_queue] == '\0');

				// Now that we have a key press we need to look at what was pressed.
				// Is the first key in the queue a '#'...
				if(key_queue[0] == '#'){
					lock_state = lock(lock_state);
					clearKeyQueue();
					count_queue = 0;

				// ...or is the first key in the queue a '*'?
				} else if(key_queue[0] == '*'){

					clearLCD();
					clearKeyQueue();
					count_queue = 0;
					writeLCDline(new_code_menu, 1);
					cursorPosition(2);

					// Now we start entering the new code. An '*' indicates that we're
					// done entering the code.
					while(key_queue[0] != '*') {
						
						clearKeyQueue();

						// Wait for keypress
						while(key_queue[count_queue] == '\0');

						// Was the latest key pressed between 0 and 9, AND has the user
						// entered less than the max number (8) of digits allowed for the
						// code.
						if(key_queue[0] >= '0' && key_queue[0] <= '9' && count_queue < 9){
							// QUESTION: DO WE NEED TO MANUALLY SHIFT THE QUEUE??
							writeLCDcharacter(key_queue[0]);
							++count_queue;
						
						// If the latest key pressed is '*' then lets write the new
						// code to eeprom
						} else if (key_queue[0] == '*') {
							writeTOeeprom();


						// Otherwise the only key left is '#', so that's what must've been
						// pressed. :)
						} else {
							clearLCD();
							writeLCDline(cancel_code_change,1);
							_delay_ms(5000);
							clearKeyQueue();
							count_queue = 0;
							clearLCD();
						}
				
					}

					
				}		
			} // End while (lock_state == 0 )

	} // End while(1);
} // End main();

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
>>>>>>> 36901f85fc843ec0329de10e3d5d8b31d6d5e5c8
