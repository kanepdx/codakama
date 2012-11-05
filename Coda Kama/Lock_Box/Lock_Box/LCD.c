/*
 * LCD.c
 *
 *  Author: Coda Kama, Matt Grimm
 *
 * LCD.c - LCD functions  
 *
 */

#include "LCD.h"

#define BL PB1					// LCD back light LED Cathode- connected to PB1
#define R_W PB0					// Read/Write (LCD) connected to PB0 (MC)
#define RS PC5					// Register Select (LCD) connected to PC5 (MC)
#define E PC4					// Enable (LCD) connected to PC5 (MC)
#define LCD_PORT_1 PORTC		// Using Port C (MC) for the LCD
#define LCD_DDR_1 DDRC			// With Port C (MC) being assigned direction register assignment
#define LCD_PORT_2 PORTB		// Using Port B (MC) for the LCD
#define LCD_DDR_2 DDRB			// With Port B (MC) being assigned direction register assignment


// Sets up LCD and initializes it in 4 bit mode
void initializeLCD(void){
	int i = 0;						// counter variable for FOR loop
	
	LCD_DDR_1 = 0x3F;				// Making all LCD pins of port C outputs
	LCD_PORT_1 &= ~(0x3F);			// Clearing LCD pins in port C
	LCD_DDR_2 |= (1 << R_W);		// Making pin PB1 an output
	LCD_PORT_2 &= ~(1 << R_W);		// Masking or clearing pin PB1 to be low for write operation
	
	/* 3 8-bit Function Set instructions to reset LCD */
	for(i=0; i<=3; i++){
		LCD_PORT_1 |= 0x3;
		pulseElatch();
		_delay_ms(1);
		LCD_PORT_1 &= ~(0x3);
	}
	
	/* Setting Function in 4-bit mode*/
	LCD_PORT_1 |= 0x2;
	pulseElatch();
	_delay_ms(1);
	LCD_PORT_1 &= ~(0x2);
	
	/* 4 BIT MODE NOW */

	writeLCDdata(0x28); /* Setting Function Set (4 bit, Dual Line, 5x8 dots */
	writeLCDdata(0x0F); /* Setting Display (Display ON, Cursor Blinking, Cursor Position ON */
	writeLCDdata(0x06); /*  Setting Entry Mode (Increment after every read write op.*/
}

// Clears the LCD and places cursor at home (1x1)
void clearLCD (void){
	LCD_DDR_1 = 0x3F;				// Making LCD PORTC pins outputs
	LCD_PORT_1 &= ~(1 << RS);		// Clearing RS to send command
	LCD_DDR_2 |= (1 << R_W);		// Making pin PB0 an output
	LCD_PORT_2 &= ~(1 << R_W);		// Masking or clearing pin PB0 to be low for write operation
	writeLCDdata(0x01);				// Writing clear instruction
}

// Writes a character to current cursor position
void writeLCDcharacter(int character){
	LCD_DDR_1 = 0x3F;				// Making LCD PORTC pins outputs
	LCD_PORT_1 |= (1 << RS);		// Setting RS to send data
	LCD_DDR_2 |= (1 << R_W);		// Making pin PB0 an output
	LCD_PORT_2 &= ~(1 << R_W);		// Masking or clearing pin PB0 to be low for write operation
	writeLCDdata(character);		// Sending the character on the data lines
}

// Places the cursor to the home of the specified line
void cursorPosition(int line){
	int i = 1;
	
	LCD_DDR_1 = 0x3F;				// Making LCD PORTC pins outputs
	LCD_PORT_1 &= ~(1 << RS);		// Clearing RS to send command
	LCD_DDR_2 |= (1 << R_W);		// Making pin PB1 an output
	LCD_PORT_2 &= ~(1 << R_W);		// Masking or clearing pin PB0 to be low for write operation
	
	writeLCDdata(0x0C);				// Turning the cursor OFF
	
	if (line == 2)					// Move cursor to line 2
	{
		writeLCDdata(0x2);			// Sending the cursor home
		for(i=1;i<16+25;i++){		// Loop cursor move to the right instruction
		writeLCDdata(0x14);
		}
	}
	else
	writeLCDdata(0x2);			// Sending cursor home

	writeLCDdata(0x0F);				// Turning on the cursor ON
}

// Writes an array of 16 characters to the specified line
void writeLCDline(char line_data[16], int line){
	cursorPosition(line);					// Setting cursor position
	for(int i=0; i<16; i++){				// Looping through character array
		writeLCDcharacter(line_data[i]);	// Displaying character in specified array element
	}
}

// Pulses Enable pin to latch in an instruction
void pulseElatch(void){	
	LCD_PORT_1 |= 1 << E;		// Setting Enable high
	_delay_ms(1);				// Delaying 1 ms
	LCD_PORT_1 &= ~(1 << E);	// Clearing Enable 
}

// Writes nibbles on data lines 
void writeLCDdata(int data_byte){
	int uppernibble = (data_byte & 0xF0) >> 4;	// Defining uppernibble to variable
	int lowernibble = data_byte & 0x0F;			// Defining lowernibble to variable
	LCD_PORT_1 &= ~0x0F;						// Clearing nibble on LCD
	LCD_PORT_1 |= uppernibble;					// Sending uppernibble
	pulseElatch();								// Latching in upper nibble
	LCD_PORT_1 &= ~(uppernibble);				// Clearing uppernibble
	LCD_PORT_1 |= lowernibble;					// Sending lowernibble
	pulseElatch();								// Latching in lowernibble
	_delay_ms(1);								// Delaying to ensure instruction is executed
	LCD_PORT_1 &= ~(lowernibble);				// Clearing lower nibble
}

// Turns the LCD back light on or off
void BacklightLCD (int on1_off0){
	LCD_DDR_2 |= (1 << BL);		// Making pin PB1 an output
	if (on1_off0 == 1)
		LCD_PORT_2 &= ~(1 << BL);		// Masking or clearing pin PB1 sinking current turning on Back light
	else
		LCD_PORT_2 |= (1 << BL);		// Setting PB1 allowing no current flow to turn bakc light off
	
}




