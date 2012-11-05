/*
	THIS CODE IS NOT FINISHED YET!!
*/
#include "keypad.h"

// includes to be removed later
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

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
#define KEY_QUEUE_SIZE 8								// size of key queue
#define INPUT_SIZE 8

// globals to be moved later
int locked = 1;											// will be updated by Colin's lock/unlock functions
int button_state = 0;									// 0 = unpressed, 1 = pressed
int key_queue[KEY_QUEUE_SIZE];							// holds user inputs
int key_value;											// holds current key value
int correct_password[] = {1,2,3,4,0,0,0,0};				// holds a user specified password
int input_index = 0;									// ????????????
int code_input[INPUT_SIZE];								// ????????????
int submit_input[INPUT_SIZE];							// ????????????

// for convenience
int columns[] = {COL1, COL2, COL3};						// an array of columns. for use in loops.
int rows[] = {ROW1, ROW2, ROW3, ROW4};					// an array of the rows. for use in loops.

// adds the most recent key press to the key_queue
void getKeyPress(void){		
	int row;
	int col;
	int key;							
	if(button_state == 0){
		button_state = 1; 								// button was previously unpressed and is now pressed
		row = getRow();									// get the row of current key being pressed
		col = getCol(row); 								// get the col of current key being pressed		
		key = keyMap(row,col);							// will hold the key value to be added to queue
		pushKey(key); 									// push key onto queue		
	}
	else {
		button_state = 0;								// button was previously pressed and is now unpressed
	}
}

int getRow(void){										// determine which row on the keypad caused the interrupts.
	int r;												// temporary placeholder for the row 
	int rr;
	for(int i=0; i < NUM_ROWS; i++){
		if((PINB & (1 << rows[i])) == 0){				// a logic low indicates that a key was pressed in that row.
			r = i;									
		}
	}
	if(r == 0) rr = ROW1;
	if(r == 1) rr = ROW2;
	if(r == 2) rr = ROW3;
	if(r == 3) rr = ROW4;
	
	
	return rr;
}

int getCol(int r) {										// strobe outputs to determine column
	int c;
	
	PORTB |= (1 << COL3);								// set third column high
	_delay_us(10000);										// wait for debouncing filter
	if((PINB & (1 << r)) == 1) {					// if the row went high
		c = 3;											// then the key is in the third column
	}
	PORTB &= ~(1 << COL3);								// set the third column back to low
	_delay_us(10000);
	
	PORTB |= (1 << COL1);								// set first column high
	_delay_us(10000);										// wait for debouncing filter
	if((PINB & (1 << r)) == 1) {					// if the row went high
		c = 1;											// then the key is in the first column
	}
	PORTB &= ~(1 << COL1);								// set first column back to low
	_delay_us(10000);	
	
	PORTB |= (1 << COL2);								// set second column high
	_delay_us(10000);										// wait for debouncing filter
	if((PINB & (1 << r)) == 1) {					// if the row went high
		c = 2;											// then the key is in the second column
	}
	PORTB &= ~(1 << COL2);								// set the second column back to low
	_delay_us(10000);	
	
	
	return c;
}

int keyMap(int r,int c) { 										// maps (row,column) pairs to an integer value corresponding to a key
	int k;
	r++;														// adjust row value to account for 0 index
	if((r == 1) && (c == 1)) k = 1;
	if((r == 1) && (c == 2)) k = 2;
	if((r == 1) && (c == 3)) k = 3;
	if((r == 2) && (c == 1)) k = 4;
	if((r == 2) && (c == 2)) k = 5;
	if((r == 2) && (c == 3)) k = 6;
	if((r == 3) && (c == 1)) k = 7;
	if((r == 3) && (c == 2)) k = 8;
	if((r == 3) && (c == 3)) k = 9;
	if((r == 4) && (c == 1)) k = STAR;
	if((r == 4) && (c == 2)) k = 11;
	if((r == 4) && (c == 3)) k = POUND;
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

void clearInput(void){ 										// clears submit_input, code_input, and input_index
	input_index = 0;
	for(int i = 0; i < INPUT_SIZE; i++){
		code_input[i] = 0;
		submit_input[0] = 0;
	}
}