/*
 * testing_keypad_functions.c
 *
 * Created: 11/3/2012 12:34:41 PM
 *  Author: djwurtz
 */ 

/*
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
*/
#include "keypad.c"

int main(void)
{
	/*
		In order to simulate a keypad we need to set all pins as outputs. The output values
		will be set in a way that emulates what the pins would see if they were hooked up to
		a keypad:
		
		All columns will be set low.
		All rows will be set high.
		
		Changing row 2 to low will simulate a keypress on row 2.
		
		In this program we will simulate a user pressing the '5' key (row 2, column 2).
	*/
	DDRB |= 0xFF;										// set all pins as outputs								
	for(int i = 0; i < NUM_COLS; i++){					// set columns low
		PORTB &= ~(1 << columns[i]);
	}												
	for(int i = 0; i < NUM_ROWS; i++){					// set rows high
		PORTB |= (1 << rows[i]);
	}
	
	getKeyPress();
	
    while(1)
    {
        //TODO:: Please write your application code 
    }
}