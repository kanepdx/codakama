/*
 * main.c
 * TESTING KEYPAD
 * Created: 10/28/2012 5:37:58 PM
 *  Author: djwurtz
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

// define pin names
#define COL1 PINB3 
#define COL2 PINB4 
#define COL3 PINB5 
#define ROW1 PINB0
#define ROW2 PINB1
#define ROW3 PINB2
#define ROW4 PINB7
#define STAR 10
#define POUND 11
#define KEY_QUEUE_SIZE 4
#define INPUT_SIZE 8

// initialize globals
int locked = 1;
int button_state = 0; // 0 = unpressed, 1 = pressed
int key_queue[KEY_QUEUE_SIZE];
int key_value;
int correct_password[] = {1,2,3,4,0,0,0,0};
int input_index = 0;
int code_input[INPUT_SIZE];
int submit_input[INPUT_SIZE];

// interrupt service routine for a pin level change on port B (key presses)
ISR(PCINT0_vect){
	cli();
	int key = POUND; // will hold the key value to be added to queue
	if(button_state == 0){
		PORTD |= ~(1 << 0); // FOR TESTING ONLY !!!!
		//_delay_ms(1000); // FOR TESTING ONLY !!!!
		int row; // will hold the row of current key being pressed
		button_state = 1; // button was previously unpressed and is now pressed
		
		// check pin levels to determine row (a low indicates that a key was pressed in that row)
		if(!(PINB & (1 << ROW1))){
			row = ROW1;
		}
		if(!(PINB & (1 << ROW2))){
			row = ROW2;
		}
		if(!(PINB & (1 << ROW3))){
			row = ROW3;
		}
		if(!(PINB & (1 << ROW4))){
			row = ROW4;
		}
		
		// strobe outputs to determine column
		int col; // will hold the col of current key being pressed
		
		PINB |= (1 << COL1); // set COL1 high
		_delay_us(100); // wait for cap
		if(PINB & (1 << row)){
			col = COL1;
		}
		PINB |= ~(1 << COL1); // set COL1 low
		
		PINB |= (1 << COL2); // set COL2 high
		_delay_us(100); // wait for cap
		if(PINB & (1 << row)){
			col = COL2;
		}
		PINB |= ~(1 << COL2); // set COL2 low
		
		PINB |= (1 << COL3); // set COL3 high
		_delay_us(100); // wait for cap
		if(PINB & (1 << row)){
			col = COL3;
		}
		PINB |= ~(1 << COL3); // set COL3 low
		
		key = keyMap(row,col);
		
	}
	else {
		// button was previously pressed and is now unpressed
		button_state = 0;
		pushKey(key); // push key onto queue
		PORTD |= (1 << 0); // FOR TESTING ONLY !!!!
	}
	sei();
}

void main(void){	
	// initialize inputs
	clearInput();
	
	// initialize interrupts
	PCMSK0 |= (1 << ROW1)|(1 << ROW2)|(1<< ROW3)|(1 << ROW4);
	PCICR |= (1 << PCIE0);
	sei();

	// initialize GPIO
	PORTB = 0b00000000;
	DDRB = 0b00111001;
	PORTD = 0b11111111; // FOR TESTING ONLY !!!!
	DDRD = 0b11111111; // FOR TESTING ONLY !!!!
	


	
	
	
	
    while(locked == 1){		
		PORTB |= (1 << PINB0); // FOR TESTING ONLY !!!!
		//_delay_ms(1000); // FOR TESTING ONLY !!!!
		PORTB |= ~(1 << PINB0); // FOR TESTING ONLY !!!!
		//_delay_ms(1000); // FOR TESTING ONLY !!!!
        //waiting for the correct password to be entered (followed by *)
		while(submit_input != correct_password){			
			key_value = popKey(); // updates key_value with most recent key press			
			if(key_value == POUND){
				
			}			
			else if(key_value == STAR){
				for(int i; i < INPUT_SIZE; i++){
					submit_input[i] = code_input[i];
				}
				if(submit_input != correct_password){ // if the password was incorrect...
					//writeStr('WRONG',1); 
					_delay_ms(6000);
					clearInput(); // clears submit_input, code_input, and input_index
					//clearLCD();
					//writeStr('GREETING_1',1)
				}				
			}			
			else {
				//continue taking inputs and update LCD
				code_input[input_index] = key_value;
				input_index++;
				//appendStr(key);
			}			
		}					 
    }
	
	while(locked == 0){
		PORTD |= ~(1 << 1);
		//the correct password was entered and the lock is now open
	}
	
}

// define functions
int keyMap(r,c) { // maps row,column pairs to an integer value corresponding to a key
int k;
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

int popKey(void){ // pop first element and shift queue;
	int k = key_queue[0];
	for(int i = 0; i < KEY_QUEUE_SIZE - 1; i++){
		key_queue[i] = key_queue[i+1];
	}	
	return k;
}

void pushKey(int k){ // shift queue and update first element
	for(int i = KEY_QUEUE_SIZE - 1; i > 0; i--){
		key_queue[i] = key_queue[i-1];
	}
	key_queue[0] = k;		
}

void clearInput(void){ // clears submit_input, code_input, and input_index
	input_index = 0;
	for(int i = 0; i < INPUT_SIZE; i++){
		code_input[i] = 0;
		submit_input[0] = 0;
	}
	
}