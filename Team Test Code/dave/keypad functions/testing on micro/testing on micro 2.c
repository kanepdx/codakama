/*
 * testing_on_micro.c
 *
 * Created: 11/3/2012 1:58:03 PM
 *  Author: djwurtz
 */ 

void initRows(int r[]);
void initColumns(int c[]);
void drawKey(void);
void initDisplay(void);

#include <avr/io.h>
#include <avr/interrupt.h>
#include "keypad3.c"

// globals for 4 LED display
#define LED1 PORTD0
#define LED2 PORTD1
#define LED3 PORTD2
#define LED4 PORTD3
#define NUM_LEDS 4

// globals for 4 LED display
int display[] = {LED1, LED2, LED3, LED4};


ISR(PCINT0_vect){
	cli();
	if(getButtonState()){								// on key down...
		getKeyPress();
	}	
	if(!getButtonState()){								// on key release
		drawKey();										// draw the key
	}	
	sei();
}

int main(void)
{			
	// initialize IO
	int rows[] = {ROW1, ROW2, ROW3, ROW4};
	int cols[] = {COL1, COL2, COL3};
	initColumns(cols);									// set keypad columns as outputs
	initRows(rows);										// set keypad rows as inputs
	initDisplay();										// initialize a 4 LED display on port D
	
	// initialize interrupts
	PCICR |= (1 << PCIE0);								// set bit 0 of PCICR
	for(int i = 0; i < NUM_ROWS; i++){
		PCMSK0 |= (1 << rows[i]);						// allow row pins to trigger interrupts
	}
	sei();	
    while(1)
    {
        //TODO:: Please write your application code 
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

void drawKey(void){
	int output[NUM_LEDS];
	switch(key_queue[0]){								// determine LED output values
		case 1:		output[0] = 0;						// a 1 indicates that the led is on, 0 off.
					output[1] = 0;						// element 0 is most significant bit
					output[2] = 0;
					output[3] = 1;
					break;
		case 2:		output[0] = 0;						
					output[1] = 0;
					output[2] = 1;
					output[3] = 0;
					break;
		case 3:		output[0] = 0;
					output[1] = 0;
					output[2] = 1;
					output[3] = 1;
					break;
		case 4:		output[0] = 0;
					output[1] = 1;
					output[2] = 0;
					output[3] = 0;
					break;
		case 5:		output[0] = 0;
					output[1] = 1;
					output[2] = 0;
					output[3] = 1;
					break;
		case 6:		output[0] = 0;
					output[1] = 1;
					output[2] = 1;
					output[3] = 0;
					break;
		case 7:		output[0] = 0;
					output[1] = 1;
					output[2] = 1;
					output[3] = 1;
					break;
		case 8:		output[0] = 1;
					output[1] = 0;
					output[2] = 0;
					output[3] = 0;
					break;
		case 9:		output[0] = 1;
					output[1] = 0;
					output[2] = 0;
					output[3] = 1;
					break;
		case STAR:	output[0] = 1;
					output[1] = 0;
					output[2] = 1;
					output[3] = 0;
					break;
		case 11:	output[0] = 1;
					output[1] = 0;
					output[2] = 1;
					output[3] = 1;
					break;
		case POUND:	output[0] = 1;
					output[1] = 1;
					output[2] = 0;
					output[3] = 0;
					break;
		default:	output[0] = 0;
					output[1] = 0;
					output[2] = 0;
					output[3] = 0;
					break;
	}
	
	for(int i = 0; i < NUM_LEDS; i++){
		if(output[i] == 0){
			// set output high (off)
			PORTD |= (1 << display[i]);
		}
		if(output[i] == 1){
			// set output low (on)
			PORTD &= ~(1 << display[i]);
		}
	}
}

void initDisplay(void){
	for(int i = 0; i < NUM_LEDS; i++){
		PORTD |= (1 << display[i]);						// set led outputs high (off)
		DDRD |= (1 << display[i]);						// set led pins as outputs
	}
}