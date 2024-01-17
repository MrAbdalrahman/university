#include<lpc213x.h>
#define RS 0x00020000 
#define RW 0X00040000 
#define EN 0X00080000 
#define CLR 0X00FE0000
#define FIRST_ROW 0x80
#define SECOND_ROW 0xC0
#define LCD_CLEAR 0x01
char os[] = "system is off";
char os2[] = "system is on";

void Delay(unsigned int times)
{
 int i, j;
 for (j = 0; j < times; j++)
 for (i = 0; i < 300; i++)
 ;
}

__irq void Timer0_IRQ(void);
void Timer0_Init(void);
int dec_to_7seg(int number);
void delay(void);
int s1, s2, m1, m2, ma, mb = 0;
char so[] = "system off"; 



void LCD_Command(char command)
{
 int Temp;
 IO1CLR = CLR;
 IO1SET = EN;
 IO1CLR = RS;
 IO1CLR = RW;
 Temp = (command & 0xF0) << 16;
 IO1SET = IO1SET | Temp;
 Delay(2);
 IO1CLR = EN;
}


void LCD_Command1(char command1)
{
 int Temp;
 IO1CLR = CLR; /* Clearing the port pins */
 IO1SET = EN; /* Enable pin high */
 IO1CLR = RS; /* RS=0 for command register */
 IO1CLR = RW; /* R/W=0 for write */
 /* Taking the first nibble of command */
 Temp = (command1 & 0xF0) << 16;
 IO1SET = IO1SET | Temp; /* Writing it to data line */
 Delay(2);
 IO1CLR = EN; /* Enable pin low to give H-L pulse */
 /* same as above for the second nibble */
 IO1CLR = CLR;
 IO1SET = EN;
 IO1CLR = RS;
 IO1CLR = RW;
 Temp = (command1 & 0x0F) << 20;
 IO1SET = IO1SET | Temp;
 Delay(2);
 IO1CLR = EN;
}




void LCD_Data(char data)
{
 int Temp;
 IO1CLR = CLR; /* Clearing the port pins */
 IO1SET = EN; /* Enable pin high */
 IO1SET = RS; /* RS=1 for data register */
 IO1CLR = RW; /* R/W=0 for write */
 Temp = (data & 0xF0) << 16; /* Taking the first nibble of command */
 IO1SET = IO1SET | Temp; /* Writing it to data line */
 Delay(2);
 IO1CLR = EN; /* Enable pin low to give H-L pulse */
 IO1CLR = CLR; /* Clearing the port pins */
 IO1SET = EN; /* Enable pin high */
 IO1SET = RS; /* RS=1 for data register */
 IO1CLR = RW; /* R/W=0 for write */
 Temp = (data & 0x0F) << 20; /* Taking the second nibble of command */
 IO1SET = IO1SET | Temp; /* Writing it to data line */
 Delay(2);
 IO1CLR = EN; /* Enable pin low to give H-L pulse */
}




void LCD_String(unsigned char *dat)
{
 /* Check for termination character */
 while (*dat != '\0')
 {
 /* Display the character on LCD */
 LCD_Data(*dat);
 /* Increment the pointer */
 dat++;
 }
}



void LCD_Init(void)
{
 Delay(15);
 LCD_Command(0x30);
 Delay(10);
 LCD_Command(0x30);
 Delay(5);
 LCD_Command(0x30);
	Delay(50);
 LCD_Command(0x20);
	Delay(50);
 LCD_Command1(0x28);
	Delay(50);
 LCD_Command1(0x01); /* Clear display */
	Delay(50);
 LCD_Command1(0x06); /* Auto increment */
	Delay(50);
 LCD_Command1(0x0C); /* Cursor off */
	Delay(50);
}


int main(void) {

	IO0DIR |= (1<<21);
	IO0SET = (1<< 21);
	IO1DIR = 0x00FE0000; // LCD pins 
	IO0DIR |= 0xF00; // P0.8 ... P0.11 output
	Timer0_Init(); //initialize timer0 with 1 sec delay
	T0TCR = 0x01; //start timer0
	 LCD_Init(); /* Initialise LCD */
				Delay(50);
	
	while (1){
		Delay(50);
		LCD_Command1(FIRST_ROW);
		Delay(50);
 LCD_String(os);
		Delay(50);
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
	ma += 1;
	mb +=1;
	}
	if (ma == 1){
		int i = 0;
			for (i = 0; i < 13; i++)
				os[i] = os2[i];
		os[i-1] = ' ';
	}
	
	
	if (mb == 5)
	{
	 Delay(50);
	IO0CLR = (1<< 21);
		
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
	T0MR0 = 50-1; // for 1000 ms = 1 sec
	T0MCR = 3;
	T0TC = 0x00;
	VICVectAddr4 = (unsigned) Timer0_IRQ;
	VICVectCntl4 = (0x20 | 4);
	VICIntEnable |= (1<<4);
}