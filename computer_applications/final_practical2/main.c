#include<lpc213x.h>
__irq void T0_ISR(void);
void Timer0_Init(void);
int Counter = 0;
int rate = 5000;
int main (void) {
IO0DIR |= 0x7F; 
IO1DIR &= ~(0x000E0000); 
	
Timer0_Init();
 T0TCR = 0x01; //start timer
while (1)

if ((!(IO1PIN & (1<<16)))){
			if (rate < 9501)  
			rate +=500; 
			Timer0_Init();
		}
		
		if ((!(IO1PIN & (1<<17)))){
			if (rate > 1499){
			rate -=500; 
			Timer0_Init();
		}
		
		if ((!(IO1PIN & (1<<18)))){
			rate = 5000; 
			Counter = 0;
			Timer0_Init();
		}
}
}
__irq void Timer0_IRQ(void)
{
 IO0CLR = 0x7F; //clear 7-seg
 IO0SET = Counter; // load next number 3
 Counter++;
 if(Counter > 7) Counter = 0;
	
T0IR = ( T0IR | (0x01) );
VICVectAddr = 0x00;
}

void Timer0_Init(void)
{
 VPBDIV = 0x1; //make pclk = cclk (=12MHz)
 //timer0 configuration//
 T0CTCR = 0x00; // select timer mode
 T0PR = 12000-1; // pre-scale value 12MHz/12000 = 1000 = 1ms
 T0MR0 = rate-1; // for 1000 ms = 1 sec

 T0MCR = 3;
 T0TC = 0x00;

 VICVectAddr4 = (unsigned) Timer0_IRQ;
 VICVectCntl4 = (0x20 | 4);
 VICIntEnable |= (1<<4);
}



