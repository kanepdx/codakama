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
#define FDBKLOCK PINB			// feedback pins are PB6 and PD2
#define FDBKOPEN PIND			// PB6 limits the locking motion
#define FDBKLOCKPIN PB6			// PD2 limits the opening motion
#define FDBKOPENPIN PD2


int unlock(int status)
{
	cli();				// disable interrupt
	
	if(status != 0)				// if not open, open
		MOTORPORT |= PULLIN;		// FIN goes high, lock shoves off
	
	while(status != 0){				// while not open
									
		if((FDBKOPEN & (1<<FDBKOPENPIN))==0)	// check for button press
			status = 0;			// lock is 0=open
	}		

	MOTORPORT &= ~PULLIN;		// FIN goes low, lock stops
	sei();				// re-enable interrupt
	return status;			// update lock state
}

int lock(int status)
{
	cli();						// disable interrupt
	
	if(status != 1)					// if not locked, lock
		MOTORPORT |= PUSHOUT;			// RIN goes high, lock shoves off
	
	while(status != 1){				// while not open
	
		if((FDBKLOCK & (1<<FDBKLOCKPIN))==0)	// check for button press
			status = 1;			// lock is 1=locked
	}

	MOTORPORT &= ~PUSHOUT;		// RIN goes low, lock stops
	sei();				// re-enable interrupt
	return status;			// update lock state
}
