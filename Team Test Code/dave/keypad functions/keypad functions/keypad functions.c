#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "keypad.c"

ISR(PCINT0_vect){									// port B PCI routine
	cli();												// disable interrupts
	getKeyPress();								// clear bit 0 of PORTB
	sei();												// re-enable interrupts
}

int main(){
	cli();
	/*
		In order to simulate a keypad we need to set all pins as outputs. The output values
		will be set in a way that emulates what the pins would see if they were hooked up to
		a keypad:
		
		All columns will be set low.
		All rows will be set high.
		
		Changing a row to low will simulate a keypress on row 2.
		
		In this program we will simulate a user pressing the '5' key (row 2, column 2).
	*/
	DDRB |= 0xFF;										// set all pins as outputs								
	for(int i = 0; i < NUM_COLS; i++){					// set columns low
		PORTB &= ~(1 << columns[i]);
	}												
	for(int i = 0; i < NUM_ROWS; i++){					// set rows high
		PORTB |= (1 << rows[i]);
	}
	
	
	// initialize interrupts
	PCICR |= (1 << PCIE0);								// set bit 0 of PCICR
	for(int i = 0; i < NUM_ROWS; i++){
		PCMSK0 |= (1 << rows[i]);						// allow row pins to trigger interrupts
	}
	sei();												// enable interrupts
	
	while(1){										// primary loop
		/*
			In order to simulate a user pressing the '5' key we need to pull the row 2 pin low.
			This will trigger the ISR. 
		*/
		PORTB &= ~(1 << ROW2); 
	}
}