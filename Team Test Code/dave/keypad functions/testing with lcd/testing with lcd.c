/*
 * testing with lcd.c
 *
 * Created: 11/9/2012 5:04:34 PM
 *  Author: djwurtz
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "keypad4.c"
#include "LCD.c"

void initRows(int r[]);
void initColumns(int c[]);

int enter_code[16] = {'E','N','T','E','R',' ','C','O','D','E',' ',' ',' ',' ',' ',' '};
int incorrect_code[16] = {'I','N','C','O','R','R','E','C','T',' ','C','O','D','E',' ',' '};
int unlocked_menu_1[16] = {'#',' ','-',' ','T','O',' ','L','O','C','K',' ',' ',' ',' ',' ',};
int unlocked_menu_2[16] = {'*',' ','-',' ','S','E','T',' ','N','E','W',' ','C','O','D','E',};
int new_code_menu[16] = {'S','E','T',' ','N','E','W',' ','C','O','D','E',' ',' ',' ',' ',};
int cancel_code_change[16] = {' ',' ','-','-','C','A','N','C','E','L','E','D','-','-',' ',' ',};

ISR(PCINT0_vect){
	_delay_ms(5);
	if(getButtonState()){		
		getKeyPress();		
		//_delay_ms(500);
		//writeLCDcharacter('p');
		_delay_ms(50);
	}
	else if(!getButtonState()){
		//writeLCDcharacter('q');
		pushKey(current_key);
		writeLCDcharacter(key_queue[0]);		
	}
}

int main(void)
{
	// initialize
	int rows[] = {ROW1, ROW2, ROW3, ROW4};
	int cols[] = {COL1, COL2, COL3};
	int i;
	initRows(rows);										// set keypad rows as inputs
	initColumns(cols);									// set keypad columns as outputs
	initializeLCD();									// set up LCD and initialize in 4 bit mode
	clearKeyQueue();
	clearLCD();
	writeLCDline(enter_code,1);
	cursorPosition(2);
	writeLCDcharacter('#');
	
	// initialize interrupts
	PCICR |= (1 << PCIE0);								// set bit 0 of PCICR
	for(int i = 0; i < NUM_ROWS; i++){
		PCMSK0 |= (1 << rows[i]);						// allow row pins to trigger interrupts
	}
	sei();												// global interrupt enable
	
	// wait loop
	while(1)											
	{
		_delay_us(5);
		
		if(key_queue[0] == '#'){
			i = 1;
			while(key_queue[i] != '\0'){
				writeLCDcharacter(key_queue[i]);
				i++;	
			}
			key_queue[0] = '?';
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