/*
 * LCD.h
 *
 *  Author: Coda Kama, Matt Grimm
 *
 * EEPROM_RW.h - EEPROM read/write function prototypes
 * 
 *
 *	void writeTOeeprom(int short code_to_save[8]);
		* Writes an 8 element array pass code to EEPROM
		* Input is an 8 element array and has no output
		* EEPROM address specified in function macro
*	void readFROMeeprom(int short *saved_code[8]);
		* Reads the contents of EEPROM and store them in an 8 element array
		* Input is the address of an 8 element array and has no output
		* EEPROM address specified in function macro
*/


void writeTOeeprom(int short code_to_save[9]);
void readFROMeeprom(int short *saved_code[9]);