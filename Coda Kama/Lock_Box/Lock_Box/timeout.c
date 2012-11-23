#include "timeout.h"
#define TIMEOUT 0x1000						// approx 4096 ms

ISR(TIMER1_COMPA_vect){
	if((lock_state == 1)&&(key_queue[0])){
		clearLCD();
		writeLCDline(enter_code, 1);
		cursorPosition(2);
		count_queue = 0;
		clearKeyQueue();
		resetTimer();
	}
	else {
		resetTimer();
	}
}

void initializeTimeout(void){
	TIMSK1 |= (1 << OCIE1A);				// enable output compare A match interrupt
	TCCR1B |= (1 << CS12)|(1 << CS10);		// configure timer/counter clock prescaling
	OCR1A = TIMEOUT;
	resetTimer();
}

void resetTimer(void){
	TCNT1 = 0x0000;							// reset 16-bit counter to 0
}