/*
 * LCD_Test.c
 *
 * Created: 10/30/2012 6:18:31 AM
 *  Author: Matt Grimm
 */ 

#define F_CPU 1000000			// 8Mhz/8 = 1Mhz = System clock

#include <avr/io.h>				// AVR library
#include <util/delay.h>			// Delay library
#include "LCD.h"				// LCD.h library - Prototypes only
#include "LCD.c"				// LCD.c function library - Functions only


int main(void){
	
	/* Defining counter variables*/
	int w = 0;
	
	/* Defining 16 element arrays to Display */
	char line1[16] = {'C','O','D','A',' ','K','A','M','A',' ',' ',' ',' ',' ',' ',' '};
	char line2[16] = {'R','O','C','K','S','!','!','!',' ',' ',' ',' ',' ',' ',' ',' ',' '};
	char line1a[16] = {'E','N','T','E','R',' ','C','O','D','E',' ',' ',' ',' ',' ',' '};
	char numbers[10] = {'0','1','2','3','4','5','6','7','8','9'};
				
	initializeLCD();				// Initializing LCD in 4-bit mode
	
	clearLCD();						// Clearing the LCD
	
	BacklightLCD(1);				// Turning on the LCD back light
		
	_delay_ms(500);				
	writeLCDline(line1, 1);			// Writing array line1 to LCD in the 1st row
	_delay_ms(500);
	writeLCDline(line2, 2);			// Writing array line2 to LCD in the 2nd row
	_delay_ms(5000);
	
	clearLCD();						// Clearing the LCD
	writeLCDline(line1a,1);			// Writing array line1a to LCD in the 1st row
	cursorPosition(2);				// Placing the cursor to the first position of the 2nd row
	_delay_ms(1000);
	
	for(w=0;w<10;w++){				// Looping through integers to display
		writeLCDcharacter(numbers[w]);	// Writing specified integer to display in HEX
		_delay_ms(1000);
	}
	
	_delay_ms(5000);
	clearLCD();						// Clearing the LCD
	_delay_ms(500);
	writeLCDline(line1, 1);			// Writing array line1 to LCD in the 1st row
	_delay_ms(500);
	writeLCDline(line2, 2);			// Writing array line2 to LCD in the 2nd row
	_delay_ms(1000);

	clearLCD();
	BacklightLCD(0);		

}