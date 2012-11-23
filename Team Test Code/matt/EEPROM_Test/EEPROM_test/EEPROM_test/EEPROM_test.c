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
#include "EEPROM_RW.h"			// EEPROM read write prototypes
#include "EEPROM_RW.c"			// EEPRIOM read write functions


int main(void)
{
	int count;																// Temp counter variable to loop through array to test arrays on pins
	int short code_to_save[9] = {'1','2','3','4','5','6','7','8','\0'};		// Array with lock code
	int short saved_code[9];												// declaring an array to hold the saved read code
	uint8_t temp_ee_address;												// declaring variable to hold initial eeprom code address
	
	DDRB = 0xFF;															// Making all of Port B outputs for testing purposes
	
	readFROMeeprom(saved_code);												// Reading the contents of eeprom and storing the content in an array
	
	count = 0;																// Setting count variable for looping
	PORTB = 0x0;															// Clearing PORTB for testing
	while(saved_code[count] != '\0'){										// Looping through array and display contents on PORTC for testing
		PORTB = saved_code[count];											// Displaying data in saved_code array on PORTB for testing
		count++;															// Incrementing count variable for looping
	}
	
	writeTOeeprom(code_to_save);											// Writing the array of saved code to eeprom	
	
	PORTB = 0x0;															// Clearing PORTC for testing
	temp_ee_address = EEPROM_DATA_ADDRESS;									// Setting temporary variable to loop though eeprom address
	while(eeprom_read_byte(temp_ee_address) != '\0'){						// Looping through eeprom and displaying content on PORTC for testing
		PORTB = eeprom_read_byte(temp_ee_address);							// Displaying eeprom address data on PORTC for testing
		temp_ee_address++;													// Incrementing loop varaible
	}
	
	readFROMeeprom(saved_code);												// Reading the contents of eeprom and storing the content in an array
	
	count = 0;																// Setting count variable for looping
	PORTB = 0x0;															// Clearing PORTB for testing
	while(saved_code[count] != '\0'){										// Looping through array and display contents on PORTC for testing
		PORTB = saved_code[count];											// Displaying data in saved_code array on PORTB for testing
		count++;															// Incrementing count variable for looping 
	}

}
