/*
 * LCD.h
 *
 *  Author: Coda Kama, Matt Grimm
 *
 * LCD.h - LCD function prototypes
 * 
	*
	* void initializeLCD(void)
			* Initializes the LCD into 4-bit mode
			* Takes no input arguments and no output 
			* Turning on the cursor and blinks the cursor
			* Sets the cursor to home (1x1)
	* void clearLCD (void)
			* Clears the LCD screen
			* Takes no input arguments and no output 
			* Clears screen and sets the cursor to home
	* void writeLCDcharacter(int character)
			* Writes a specified character to the current cursor position
			* Take in a character and has no output 
			* The function typecasts the character, obviously by using int character
				*example - A = writeLCDcharacter(A)
				*example - 1 = writeLCDcharacter(0x30 +1)
	* void cursorPosition(int line)
			* Places the cursor to the 1st position of the specified line
			* Takes in integer line number has not output
	* void writeLCDline(char line_data[16], int line)
			* Writes and Displays 16 characters in an array on a specified line
			* Inputs are the 16 element array and the LCD line (row) has no outputs
			* The data array needs to be of type char
				*example - array = ['A','B','C'. . .];  
 */ 

void initializeLCD(void);  // Sets up LCD and initializes it in 4 bit mode
void clearLCD (void); // Clears the LCD and places cursor at home (1x1)
void writeLCDcharacter(int character); // Writes a character to current cursor position
void cursorPosition(int line); // Places the cursor to the home of the specified line
void writeLCDline(char line_data[16], int line); // Writes an array of 16 characters to the specified line
void pulseElatch(void); // Pulses Enable pin to latch in an instruction
void writeLCDdata(int data_byte); // Writes nibbles on data lines

