#include <stdio.h>

#define KEY_QUEUE_SIZE 9								// size of key queue
int key_queue[KEY_QUEUE_SIZE];							// holds user inputs

int do_escape = 0;

// Prototypes
void initializeQueue ();
int popKey();
void pushKey(int k); 					// shift queue and update first element
void printQueue ();


void main () {
	printf ("\n\n");
	initializeQueue;

	int current_code[9] = {0,2,4,6,0,0,0,0,0};

	int j = 0;

	/*
	pushKey(1);
	printQueue();
	pushKey(2);
	printQueue();
	popKey();
	pushKey(3);
	pushKey(4);
	printQueue();
	*/

	/*
	while (! do_escape) {
		printf (".");
		++j;
		if (j == 30) {
			pushKey('1');
			printQueue();
		} else if (j == 60) {
			pushKey('2');
			printQueue();
		} else if (j == 90) {
			pushKey('3');
			printQueue();
		} else if (j == 120) {
			pushKey('4');
			printQueue();
		} 

		if (key_queue[3] != 0)
			break;
	}

	printf ("\n\n...the end...\n\n");
	*/
	
	readCode (current_code);
	updateCode (current_code);
	readCode (current_code);

}

void initializeQueue () {
	int i;
	for (i=0; i< KEY_QUEUE_SIZE; ++i) {
		key_queue[i] = 0;
	}
}

int popKey(void){ 						// pop first element and shift queue;
	int i;
	int k = key_queue[0];

	for(i = 0; i < KEY_QUEUE_SIZE - 1; i++){
		key_queue[i] = key_queue[i+1];
	}	
	key_queue[KEY_QUEUE_SIZE-1] = '\0';
	return k;
}

void pushKey(int k){ 					// shift queue and update first element
	int i;
	for(i = KEY_QUEUE_SIZE - 1; i > 0; i--){
		key_queue[i] = key_queue[i-1];
	}
	key_queue[0] = k;		
}

void printQueue () {
	int i;
	//printf ("Queue: ");
	for (i=0; i< KEY_QUEUE_SIZE; ++i) {
		if ( key_queue[i] != 0 )
			printf ("%d", key_queue[i]);
	}
	//printf ("\n");
}

void readCode (int *this_code) {
	int i;

	for (i=0; i< 10; i++) {
		printf ("%d - %d\n", i, this_code[i]);
	}
}

void updateCode (int *this_code) {
	int i;

	for (i=0; i< 10; i++) {
		this_code[i] = 0;
	}

	this_code[0] = 1;
	this_code[1] = 3;
	this_code[2] = 5;
	this_code[3] = 7;

}
