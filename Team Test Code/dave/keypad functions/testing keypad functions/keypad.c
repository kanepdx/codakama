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
#define COL1 PB3										// column 1 of keypad mapped to pin
#define COL2 PB4 										// column 2 of keypad mapped to pin
#define COL3 PB5 										// column 3 of keypad mapped to pin
#define NUM_ROWS 4										// number of rows on the keypad
#define ROW1 PB0										// row 1 of keypad mapped to pin
#define ROW2 PB1										// row 2 of keypad mapped to pin
#define ROW3 PB2										// row 3 of keypad mapped to pin
#define ROW4 PB7										// row 4 of keypad mapped to pin
#define STAR 10 										// '*'' key mapped to a decimal value
#define POUND 11										// '#' key mapped to a decimal value
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
	for(int i=0; i < NUM_ROWS; i++){
		if((PINB & (1 << rows[i])) == 0){				// a logic low indicates that a key was pressed in that row.
			r = i;									
		}
	}
	switch(r){
		case 0: r = ROW1;
				break;
		case 1: r = ROW2;
				break;
		case 2: r = ROW3;
				break;
		case 3: r = ROW4;
				break;
		default: r = ROW4;
				break;
	}
	return r;
}

int getCol(int r) {										// strobe outputs to determine column
	int c;
	
	PORTB |= (1 << COL1);								// set first column high
	_delay_us(100);										// wait for debouncing filter
	if((PINB & (1 << r))) {					// if the row went high
		c = 1;											// then the key is in the first column
	}
	PORTB &= ~(1 << COL1);								// set first column back to low
	
	PORTB |= (1 << COL2);								// set second column high
	_delay_us(100);										// wait for debouncing filter
	if((PINB & (1 << r))) {					// if the row went high
		c = 2;											// then the key is in the second column
	}
	PORTB &= ~(1 << COL2);								// set the second column back to low
	
	PORTB |= (1 << COL3);								// set third column high
	_delay_us(100);										// wait for debouncing filter
	if((PINB & (1 << r))) {					// if the row went high
		c = 3;											// then the key is in the third column
	}
	PORTB &= ~(1 << COL3);								// set the third column back to low
	
	return c;
}

int keyMap(int r,int c) { 										// maps (row,column) pairs to an integer value corresponding to a key
	int k;
	r++;														// adjust row value to account for 0 index
	switch(r){
		case 1:		switch(c){
						case 1:		k = 1;
						break;
						case 2:		k = 2;
						break;
						case 3:		k = 3;
						break;
						default:	k = POUND;
						break;
					}
					break;
		
		case 2:		switch(c){
						case 1:		k = 4;
						break;
						case 2:		k = 5;
						break;
						case 3:		k = 6;
						break;
						default:	k = POUND;
						break;
					}
					break;
		
		case 3:		switch(c){
						case 1:		k = 7;
						break;
						case 2:		k = 8;
						break;
						case 3:		k = 9;
						break;
						default:	k = POUND;
						break;
					}
					break;
		
		case 4:		switch(c){
						case 1:		k = STAR;
						break;
						case 2:		k = 0;
						break;
						case 3:		k = POUND;
						break;
						default:	k = POUND;
						break;
					}
					break;
		
		default:	k = POUND;
		break;
	}
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