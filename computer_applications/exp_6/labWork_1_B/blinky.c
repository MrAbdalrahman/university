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

int main ()
	{
	IO1DIR &= (~(1<<16));
	IO1DIR |= (1<<28) ;
	while(1){
if (!(IO1PIN & (1<<16))){
	 msDelay(50);
	IO1PIN ^= 0x10000000;
}	
}
}
	
