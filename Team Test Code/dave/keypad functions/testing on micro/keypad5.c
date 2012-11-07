/*
	this shit is working, kinda (best so far)
*/
#include "keypad2.h"

// includes to be removed later
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#define F_CPU 8000000UL

// definitions to be moved later
#define NUM_COLS 3										// number of columns on the keypad
#define COL1 PINB3										// column 1 of keypad mapped to pin
#define COL2 PINB4 										// column 2 of keypad mapped to pin
#define COL3 PINB5 										// column 3 of keypad mapped to pin
#define NUM_ROWS 4										// number of rows on the keypad
#define ROW1 PINB0										// row 1 of keypad mapped to pin
#define ROW2 PINB1										// row 2 of keypad mapped to pin
#define ROW3 PINB2										// row 3 of keypad mapped to pin
#define ROW4 PINB7										// row 4 of keypad mapped to pin
#define STAR 10 										// '*'' key mapped to a decimal value
#define POUND 12										// '#' key mapped to a decimal value
#define KEY_QUEUE_SIZE 9								// size of key queue
#define INPUT_SIZE 8

// globals to be moved later
int lock_state = 1;										// will be updated by Colin's lock/unlock functions
int key_queue[KEY_QUEUE_SIZE];							// holds user inputs
int key;												// holds the current input to be added to the queue

// adds the most recent key press to the key_queue
void getKeyPress(void){		
	int row;
	int col;							
	row = getRow();									// get the row of current key being pressed
	col = getCol(row); 								// get the col of current key being pressed		
	key = keyMap(row,col);							// will hold the key value to be added to queue	
}

int getButtonState(){									// check rows, if any are low then a key is being pressed
	_delay_us(100);
	int s = 0;
	if(!(PINB &(1 << ROW1))) s = 1;
	else if(!(PINB &(1 << ROW2))) s = 1;				
	else if(!(PINB &(1 << ROW3))) s = 1;				
	else if(!(PINB &(1 << ROW4))) s = 1;
	return s;
}

int getRow(void){										// determine which row on the keypad caused the interrupts.
	int r;												// temporary placeholder for the row 
		 if(!(PINB &(1 << ROW1))) r = ROW1;				// if row 1 is low then the key press was in row 1
	else if(!(PINB &(1 << ROW2))) r = ROW2;				// if row 2 is low then the key press was in row 2	
	else if(!(PINB &(1 << ROW3))) r = ROW3;				// if row 3 is low then the key press was in row 3
	else if(!(PINB &(1 << ROW4))) r = ROW4;				// if row 4 is low then the key press was in row 4
	return r;
}

int getCol(int r) {										// strobe outputs to determine column
	int c;
		
	PORTB |= (1 << COL1);								// set first column high
	_delay_us(30);										// wait for debouncing filter
	if((PINB & (1 << r))) {								// if the row went high
		c = 1;											// then the key press was in the first column
	}
	PORTB &= ~(1 << COL1);								// set first column back to low
	
	
	PORTB |= (1 << COL2);								// set second column high
	_delay_us(30);										// wait for debouncing filter
	if((PINB & (1 << r))) {								// if the row went high
		c = 2;											// then the key press was in the second column
	}
	PORTB &= ~(1 << COL2);								// set the second column back to low

	
	PORTB |= (1 << COL3);								// set third column high
	_delay_us(30);										// wait for debouncing filter
	if((PINB & (1 << r))) {								// if the row went high
		c = 3;											// then the key press was in the third column
	}
	PORTB &= ~(1 << COL3);								// set the third column back to low

	
	return c;
}

int keyMap(int r,int c) { 										// maps (row,column) pairs to an integer value corresponding to a key
	int k;
														// adjust row value to account for 0 index
		 if((r == ROW1) && (c == 1)) k = 1;
	else if((r == ROW1) && (c == 2)) k = 2;
	else if((r == ROW1) && (c == 3)) k = 3;
	else if((r == ROW2) && (c == 1)) k = 4;
	else if((r == ROW2) && (c == 2)) k = 5;
	else if((r == ROW2) && (c == 3)) k = 6;
	else if((r == ROW3) && (c == 1)) k = 7;
	else if((r == ROW3) && (c == 2)) k = 8;
	else if((r == ROW3) && (c == 3)) k = 9;
	else if((r == ROW4) && (c == 1)) k = STAR;
	else if((r == ROW4) && (c == 2)) k = 11;
	else if((r == ROW4) && (c == 3)) k = POUND;
	return k;
}

int popKey(void){ 											// pop first element and shift queue;
	int k = key_queue[0];
	for(int i = 0; i < KEY_QUEUE_SIZE - 1; i++){
		key_queue[i] = key_queue[i+1];
	}	
	return k;
}

void pushKey(int k){ 										// shift queue and update first element
	for(int i = KEY_QUEUE_SIZE - 1; i > 0; i--){
		key_queue[i] = key_queue[i-1];
	}
	key_queue[0] = k;		
}

void clearKeyQueue(void){ 										// clears submit_input, code_input, and input_index
	for(int i = 0; i < KEY_QUEUE_SIZE; i++){
		key_queue[i] = 0;
	}
}