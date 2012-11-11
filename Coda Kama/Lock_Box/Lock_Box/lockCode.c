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
#define FDBKPIN PB6

#define PULLIN 0b00010000		// FIN on PD4, |= with this to open
#define PUSHOUT 0b00001000		// RIN on PD3, |= with this to close

#define BUFFER 250				// pause for 1/4 sec to allow motor to shove off


int unlock(int status)
{
	cli();							// disable interrupt
	
	if(status != 0)					// if not open, open
		MOTORPORT |= PULLIN;		// FIN goes high, lock shoves off
	
	_delay_ms(BUFFER);				// motor propagation delay
	
	while(status != 0){				// while not open
									
		if((FDBKPORT & (1<<FDBKPIN))==0)	// check for button press
			status = 0;						// lock is 0=open
	}		

	MOTORPORT &= ~PULLIN;		// FIN goes low, lock stops
	sei();						// re-enable interrupt
	return status;				// update lock state
}

int lock(int status)
{
	cli();						// disable interrupt
	
	if(status != 1)					// if not locked, lock
		MOTORPORT |= PUSHOUT;		// RIN goes high, lock shoves off
	
	_delay_ms(BUFFER);				// motor propagation delay
	
	while(status != 1){				// while not open
	
		if((FDBKPORT & (1<<FDBKPIN))==0)	// check for button press
			status = 1;						// lock is 1=locked
	}

	MOTORPORT &= ~PUSHOUT;		// RIN goes low, lock stops
	sei();						// re-enable interrupt
	return status;				// update lock state
}
