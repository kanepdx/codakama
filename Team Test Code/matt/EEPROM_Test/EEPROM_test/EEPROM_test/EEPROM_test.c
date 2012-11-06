/*
 * EEPROM_test.c
 *
 * Created: 11/4/2012 6:56:37 PM
 *  Author: Matt Grimm
 */ 

#define F_CPU 100000000

#include <avr/io.h>				// AVR library
#include <util/delay.h>			// Delay library
#include <avr/eeprom.h>			// EEPROM

#include "EEPROM_RW.h"
#include "EEPROM_RW.c"

int main(void)
{
	int count;															// Temp counter variable to loop through array to test arrays on pins
	int short code_to_save[8] = {'0','1','2','3','4','5','6','7','8'};	// Array with lock code
	int short saved_code[8];											// declaring an array to hold the saved read code
	unit8_t temp_ee_address;											// declaring variable to hold initial eeprom code address
	
	DDRC = 0xFF;														// Making all of Port C outputs for testing purposes
	DDRB = 0xFF;														// Making all of Port C outputs for testing purposes
	
	writeTOeeprom(code_to_save);										// Writing the array of saved code to eeprom	
	
	temp_ee_address = EEPROM_DATA_ADDRESS;
	
	for(int count = 0; count < 8; count++){								// Looping through eeprom and displaying content on PORTC for testing
		PORTC = eeprom_read_byte(temp_ee_address);
		temp_ee_address++;
	}
	
	readFROMeeprom(saved_code);											// Reading the contents of eeprom and storing the content in an array
	
	for(int count = 0; count < 8; count++){								// Looping through array and display contents on PORTC for testing
		PORTB = saved_code[count];	
	}

}