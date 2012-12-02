/*
 * EEPROM_RW.c
 *
 *  Author: Coda Kama, Matt Grimm
 *
 * EEPROM_RW.c - EEPROM read/write functions
 *
 */

#include "EEPROM_RW.h"

#define EEPROM_NEWCODE_ADDRESS 0x0		// Address to hold new code flag
#define EEPROM_DATA_ADDRESS 0x01		// Starting address of saved data

int short default_code[9] = {'0','0','0','0','\0','\0','\0','\0','\0'};		// Factory default code 


// Writes an 9 element array pass code to EEPROM
void writeTOeeprom(int short code_to_save[]){
	int add_count = 0;													// variable declaration to loop through eeprom addresses
	uint8_t temp_ee_address = EEPROM_DATA_ADDRESS;						// temp address variable to go through eeprom
	for(int i = 0; i < 9; i++){										// filling eeprom wit all NULLS
		eeprom_write_byte(temp_ee_address, '\0');
		temp_ee_address++;
	}
	temp_ee_address = EEPROM_DATA_ADDRESS;
	while(code_to_save[add_count] != '\0'){								// This loop fills temp_code array with saved code and fills the rest with NULL
			if(code_to_save[add_count] == '*'){							// Fixes * bunce
				add_count++;
			}
			else{
				eeprom_write_byte(temp_ee_address, code_to_save[add_count]);		// writing byte to specified address
				add_count++;
				temp_ee_address++;
			}				
	}
	eeprom_write_byte(temp_ee_address, code_to_save[add_count]);		// writing the NULL character to the end of the pass code
	eeprom_write_byte(EEPROM_NEWCODE_ADDRESS,0x0);						// Setting new code flag
}


// Reads the contents of EEPROM and store them in an 9 element array
void readFROMeeprom(int short *saved_code[]){
	int add_count = 0;														// variable declaration to loop through eeprom and elements of array
	uint8_t temp_ee_address = EEPROM_DATA_ADDRESS;							// temp address variable to go through eeprom
		
	if(eeprom_read_byte(EEPROM_NEWCODE_ADDRESS) == 0x0){
		while(eeprom_read_byte(temp_ee_address) != NULL){					// looping through code to save array
			saved_code[add_count] = eeprom_read_byte(temp_ee_address);		// writing byte to specified address
			temp_ee_address++;												// incrementing address counter		
			add_count ++;													// incrementing array counter
		}
		saved_code[add_count] = eeprom_read_byte(temp_ee_address);			// setting NULL character at the end of pass code array
	}
	else{
		while(default_code[add_count] != NULL){								// looping through code to save array
			saved_code[add_count] = default_code[add_count];				// writing byte to specified address
			add_count++;													// incrementing array counter			
		}
		saved_code[add_count] = default_code[add_count];					// setting NULL character at the end of pass code array
	}	
}