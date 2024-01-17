#include<lpc213x.h>
__irq void Timer0_IRQ(void);
void Timer0_Init(void);
int dec_to_7seg(int number);
void delay(void);
int s1, s2, m1, m2 =0;

int main(void) {
	IO0DIR |= 0x7F; //P0.0 ... P0.6 output
	IO0DIR |= 0xF00; // P0.8 ... P0.11 output
	Timer0_Init(); //initialize timer0 with 1 sec delay
	T0TCR = 0x01; //start timer0
	while(1){
		IO0SET |= (1<<9) | (1<<10) | (1<<11); //disable s2,m1,m2
		IO0CLR |= (1<<8); //Enable s1
		IO0CLR = 0x7F; //clear 7-seg
		IO0SET = dec_to_7seg(s1); //load s1 on 7-seg
		delay(); //display s1 for short time
		IO0SET |= (1<<8) | (1<<10) | (1<<11); //disable s1,m1,m2
		IO0CLR |= (1<<9); //Enable s2
		IO0CLR = 0x7F; //clear 7-seg
			IO0SET = dec_to_7seg(s2); //load s2 on 7-seg
		delay(); //display s2 for short time
		IO0SET |= (1<<8) | (1<<9) | (1<<11); //disable s1,s2,m2
		IO0CLR |= (1<<10); //Enable m1
		IO0CLR = 0x7F; //clear 7-seg
		IO0SET = dec_to_7seg(m1); //load s1 on 7-seg
		delay(); //display m1 for short time
		IO0SET |= (1<<8) | (1<<9) | (1<<10); //disable s1,s2,m1
		IO0CLR |= (1<<11); //Enable m2
		IO0CLR = 0x7F; //clear 7-seg
		IO0SET = dec_to_7seg(m2); //load m2 on 7-seg
		delay(); //display m2 for short time
	}
}
__irq void Timer0_IRQ(void){
	s1++;
	if(s1 == 10){
	s1 = 0;
	s2 += 1;
	}
	if(s2 == 6){
	s2 = 0;
	m1 += 1;
	}
	if(m1 == 10) {
	m1 = 0;
	m2 += 1;
	}
	T0IR = ( T0IR | (0x01) );
	VICVectAddr = 0x00;
}

void delay(){
	int i,j;
	long c = 0;
	for(i=0;i<10;i++){
		for(j=0;j<1000;j++){
			c++;
		}
	}
}

int dec_to_7seg(int number){
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

void Timer0_Init(void){
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