#include<lpc213x.h>
__irq void Timer0_IRQ(void);
void Timer0_Init(void);
int counter;
int main(void) {
 IO0DIR |= (1<<16);
 IO0CLR = 1<<16;
 Timer0_Init(); //initialize timer0 with 1 sec delay
 T0TCR = 0x01; //start timer0
 while(1)
{
 if(counter ==3){ // when counter reachs 3 seconds
IO0PIN ^= (1<<16); //toggle the led
counter=0;
}
}
}
__irq void Timer0_IRQ(void)
{
//IO0PIN ^=(1<<16);
counter++;
T0IR = ( T0IR | (0x01) );
VICVectAddr = 0x00;
}

void Timer0_Init(void)
{
 VPBDIV = 0x1; //make pclk = cclk (=12MHz)
 //timer0 configuration//
 T0CTCR = 0x00; // select timer mode
 T0PR = 12000-1; // pre-scale value 12MHz/12000 = 1000 = 1ms
 T0MR0 = 1000-1; // for 1000 ms = 1 sec

 T0MCR = 3;
 T0TC = 0x00;

 VICVectAddr4 = (unsigned) Timer0_IRQ;
 VICVectCntl4 = (0x20 | 4);
 VICIntEnable |= (1<<4);
}
 