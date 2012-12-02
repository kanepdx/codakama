/*
 * lockCode.c
 *
 * Created: 10/29/2012
 * Author: Colin Doolittle
 * Coda Kama Lock Box
 *
 * Will require avr/io.h, util/delay.h, avr/interrupt.h
 * 
 */ 

#define MOTORPORT PORTD			// what port is FIN on? PD4
								// what port is RIN on? PD3
#define FDBKPORT PINB			// what port is FDBK on? PB6
#define FDBK2PORT PIND			// what port is FDBK2 on? PD2
#define FDBKPIN PB6				// pressed when unlocked (active low)
#define FDBK2PIN PD2			// pressed when locked (active low)

#define PULLIN 0b00010000		// FIN on PD4, |= with this to open
#define PUSHOUT 0b00001000		// RIN on PD3, |= with this to close

#define BUFFER 250				// pause for 1/4 sec to allow motor to shove off

void initializeMotorPins(void){
	DDRD |= (1 << PD3)|(1 << PD4);		// FIN and RIN are outputs on port D
	MOTORPORT &= ~((1 << PD3)|(1 << PD4));	// FIN and RIN are initially low
	DDRD &= ~(1 << PD2);				// FDBK2 is an input on port D
	DDRB &= ~(1 << PB6);				// FDBK is an input on port B
	
	DDRD |= (1 << PD5)|(1 << PD7);		// LED ports for debugging
	PORTD |= (1 << PD5)|(1 << PD7);		// LEDs are active low, initialized off
}

int unlock(int status)
{
	cli();							// disable interrupt
	MOTORPORT |= PULLIN;
	while(FDBKPORT & (1 <<FDBKPIN)){
		_delay_ms(1);
		if(!(FDBKPORT & (1 <<FDBKPIN))){
			_delay_ms(1);
		}
	}
	
	PORTD &= ~(1 << PD5);			// turn pin 5 LED on
	PORTD |= (1 << PD7);			// turn pin 7 LED off
	
	/*
	if(status != 0)					// if not open, open
		MOTORPORT |= PULLIN;		// FIN goes high, lock shoves off
	
	_delay_ms(BUFFER);				// motor propagation delay
	
	while(status != 0){				// while not open
									
		if((FDBKPORT & (1<<FDBKPIN))==0)	// check for button press
			status = 0;						// lock is 0=open
	}		

	MOTORPORT &= ~PULLIN;		// FIN goes low, lock stops
	*/
	MOTORPORT &= ~PULLIN;
	sei();						// re-enable interrupt
	return status = 0;				// update lock state
}

int lock(int status)
{
	cli();						// disable interrupt
	MOTORPORT |= PUSHOUT;
	
	while(FDBK2PORT & (1 <<FDBK2PIN)){
		_delay_ms(1);
		if(!(FDBK2PORT & (1 <<FDBK2PIN))){
			_delay_ms(1);
		}
	}
	
	PORTD &= ~(1 << PD7);			// turn pin 7 LED on
	PORTD |= (1 << PD5);			// turn pin 5 LED off
	/*
	if(status != 1)					// if not locked, lock
		MOTORPORT |= PUSHOUT;		// RIN goes high, lock shoves off
	
	_delay_ms(BUFFER);				// motor propagation delay
	
	while(status != 1){				// while not open
	
		if((FDBKPORT & (1<<FDBKPIN))==0)	// check for button press
			status = 1;						// lock is 1=locked
	}
	*/
	MOTORPORT &= ~PUSHOUT;		// RIN goes low, lock stops
	sei();						// re-enable interrupt
	return status = 1;				// update lock state
}
