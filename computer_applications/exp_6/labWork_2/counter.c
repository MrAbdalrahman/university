#include <LPC213X.H>
void msDelay(int d) { 
long c = 0 ;
int i,j;	
d*=2;
	for (i = 0; i < d ; i++){
	for (j = 0 ; j < 1000 ; j++){
	c++;
	}
}
}

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

int main (){
	int counter = 0 ;
	IO1DIR &= (~(1<<16));
	IO0DIR |= 0x000000FF;
	IO0CLR = 0x000000FF;
	
	while(1){
		
		
		
if (!(IO1PIN & (1<<16))){
	 msDelay(100);
	IO0CLR = 0x0000007F; 
	
	IO0PIN |= dec_to_7seg(counter);
	counter = (counter == 9)? 0:++counter;
}	
}
}
	
