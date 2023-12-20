#include<lpc213x.h>
int dec_to_7seg (int number) ;
__irq void T0_ISR(void);
void Timer0_Init(void);
int Counter = 0;
int main (void) {
IO0DIR |= 0x7F; //Configure pins 
Timer0_Init() ;
 T0TCR = 0x01; // start timer
while (1); // run forever
}

__irq void Timer0_IRQ(void)
{
 IO0CLR = 0x7F; //clear 7-seg
 IO0SET = dec_to_7seg(Counter); // load next number
 Counter++;
 if(Counter > 9) Counter = 0;
T0IR = ( T0IR | (0x01) );
VICVectAddr = 0x00;
}

void Timer0_Init(void)
{
 VPBDIV = 0x1; //make pclk = cclk (=12MHz)
 //timer0 configuration//
 T0CTCR = 0x00; // select timer mode
 T0PR = 12000-1; // pre-scale value 12MHz/12000 = 1000 = 1ms
 T0MR0 = 1500-1; // for 1000 ms = 1 sec

 T0MCR = 3;
 T0TC = 0x00;

 VICVectAddr4 = (unsigned) Timer0_IRQ;
 VICVectCntl4 = (0x20 | 4);
 VICIntEnable |= (1<<4);
}

int dec_to_7seg(int number)
{
 switch(number){
case 0 : return(0x3F);
case 1 : return(0x06);
case 2 : return(0x5B);
case 3 : return(0x4F);
case 4 : return(0x66);
case 5 : return(0x6D);
case 6 : return(0x7D);
case 7 : return(0x07);
case 8 : return(0x7F);
case 9 : return(0x6F);
default : return(0x00);
}
}

