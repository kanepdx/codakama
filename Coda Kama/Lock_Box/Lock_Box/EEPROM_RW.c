/*
 * EEPROM_RW.c
 *
 *  Author: Coda Kama, Matt Grimm
 *
 * EEPROM_RW.c - EEPROM read/write functions
 *
 */

#include "EEPROM_RW.h"

#define EEPROM_FLAG_ADDRESS 0x00		// Flag to indicate whether a new code exists

#define EEPROM_DATA_ADDRESS 0x01		// Starting address of saved data

typedef unsigned short int unit8_t;			// making uni8_t mean unsigned short int

// Writes an 8 element array pass code to EEPROM
void writeTOeeprom(int short code_to_save[8]){
	int add_count;														// variable declaration to loop through eeprom addresses
	unit8_t temp_ee_address = EEPROM_DATA_ADDRESS;						// temp address variable to go through eeprom
	
	for(int add_count = 0; add_count <8; add_count++){					// looping through code to save array
	eeprom_write_byte(temp_ee_address, code_to_save[add_count]);		// writing byte to specified address
	temp_ee_address++;												// incrementing address counter
	}
}

// Reads the contents of EEPROM and store them in an 8 element array
void readFROMeeprom(int short *saved_code[8]){
	int add_count;														// variable declaration to loop through eeprom and elements of array
	unit8_t temp_ee_address = EEPROM_DATA_ADDRESS;						// temp address variable to go through eeprom
	
	for(int add_count = 0; add_count <  8; add_count++){				// looping through code to save array
		saved_code[add_count] = eeprom_read_byte(temp_ee_address);		// writing byte to specified address
		temp_ee_address++;												// incrementing address counter		
	}
}