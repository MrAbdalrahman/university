#include <LPC213X.H>

// this function use the time equation to delay the program for the given d*0.01 seconds
void msDelay(int d) { 
	long c = 0 ;
	int i,j;	
	d*=2;
	
	for (i = 0; i < d ; i++)
		for (j = 0 ; j < 1000 ; j++)
			c++;
}

// this function decodes a dicimal number into a 7-segment input (7-segment decoder)
int dec_to_7seg(int number){
	
	switch(number){
		
		case 0: return 0x3F;
		case 1: return 0x06;
		case 2: return 0x5B;
		case 3: return 0x4F;
		case 4: return 0x66;
		case 5: return 0x6D;
		case 6: return 0x7D;
		case 7: return 0x07;
		case 8: return 0x7F;
		case 9: return 0x6F;
		default:return 0x00;
	}	
}

// the main function
int main (){
	int counter1 = 0 ; // counetr for ones
	int counter2 = 0;	 // counter for tens
	IO0DIR |= (0x000C000FF); //
	IO0DIR |= (1<<22)|(1<<23); // to multiplix between the two 7-segment displays
	IO0SET = (1<<22)|(1<<23); // setting the multiplixing pins on 0
	IO1DIR &= (~(1<<16)); // input for button
	
	while(1){ // run forever
		
		if (!(IO1PIN & (1<<16))){ // if the button is clicked then : 
			msDelay(50); //delay
			
			IO0CLR = (1<<22); // setting the output for the tens counter on 1 (on)
			IO0CLR = 0x0000007F; // setting the 7 outputs of the 7-segment display on 1 (on)
			IO0PIN |= dec_to_7seg(counter2); // setting the 7-pins output to the decoded number 
			msDelay(1);// delay 
			IO0SET = (1<<22); // closing the tens pin
		
			IO0CLR = (1<<23); // oppening the ones pin
			IO0CLR = 0x0000007F; // setting the 7 outputs of the 7-segment display on 1 (on)
			IO0PIN |= dec_to_7seg(counter1); // setting the 7-pins output to the decoded number 
			msDelay(1); //delay 
			IO0SET = (1<<23); // closing the tens pin
		
		
		// if the ones is nine then recount from zero and if the tens is nine then recount from zero
		if (counter1 == 9 ){ 
			counter1 = 0;
			if (counter2 == 9)
				counter2 = 0;
			else 
			counter2++;
		}
		else 
			counter1++;
	}
	}
}
	
