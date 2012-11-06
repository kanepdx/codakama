/*
 * Lock_Box.c
 *
 * 
 *  Author: Coda Kama
 */ 

#define F_CPU 1000000			// 8Mhz/8 = 1Mhz = System clock

#include <avr/io.h>				// AVR I/O library
#include <util/delay.h>			// Delay library
#include <avr/interrupt.h>		// Interrupt library
#include <avr/eeprom.h>			// EEPROM library

#include "LCD.h"				// LCD.h function library - Prototypes only
#include "LCD.c"				// LCD.c function library - Functions only
#include "EEPROM_RW.h"			// EEPROM_RW.h - EEPROM read/write function prototypes
#include "EEPROM_RW.c"			// EEPROM_RW.c - EEPROM read/write functions
#include "keypad.h"				// keypad.h function library - Function prototypes
#include "keypad.c"				// keypad.c function library - Functions only

int main(void)
{
    while(1)
    {
        //TODO:: Please write your application code 
    }
}