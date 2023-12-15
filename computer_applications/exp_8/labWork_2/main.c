#include <lpc213x.H>
#define NEW_LINE 0x0D
#define RS 0x00020000 /* RS - P1.17 */
#define RW 0X00040000 /* R/W - P1.18 */
#define EN 0X00080000 /* E - P1.19 */
#define CLR 0X00FE0000
#define FIRST_ROW 0x80
#define SECOND_ROW 0xC0
#define LCD_CLEAR 0x01
	
void Delay(unsigned int times){
	int i, j;
	for (j = 0; j < times; j++)
	for (i = 0; i < 300; i++)
	;
}

void LCD_Command(char command){
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

void LCD_Command1(char command1){
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


void LCD_Init(void){
	Delay(50);
	LCD_Command(0x30);
	Delay(50);
	LCD_Command(0x30);
	Delay(50);
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

void LCD_Data(char data){
	int Temp;
	IO1CLR = CLR; /* Clearing the port pins */
	IO1SET = EN; /* Enable pin high */
	IO1SET = RS; /* RS=1 for data register */
	IO1CLR = RW; /* R/W=0 for write */
	Temp = (data & 0xF0) << 16; /* Taking the first nibble of command */
	IO1SET = IO1SET | Temp; /* Writing it to data line */
	Delay(2);
	IO1CLR = EN; /* Enable pin low to give H L p ulse */
	IO1CLR = CLR; /* Clearing the port pins */
	IO1SET = EN; /* Enable pin high */
	IO1SET = RS; /* RS=1 for data register */
	IO1CLR = RW; /* R/W=0 for write */
	Temp = (data & 0x0F) << 20; /* Taking the second nibble of command */
	IO1SET = IO1SET | Temp; /* Writing it to data line */
	Delay(2);
	IO1CLR = EN; /* Enable pin low to give H L pulse */
}

char UART0_read(void) {
	while (!(U0LSR & 0x1)); //wait until the receiver buffer is full
	return(U0RBR);
}
	
void UART0_init(void){
	PINSEL0 = 0x5; /* Enable RxD0 and TxD0 */
	U0LCR = 0x83; /* 8 bits, no Parity, 1 Stop bit */
	U0DLL = 97; /* 9600 Baud Rate @ 12MHz VPB Clock */
	U0LCR = 0x03; /* DLAB = 0 */
}
int putchar (int ch) /* Write single character to Serial Port */
{
	while (!(U0LSR & 0x20));
	return (U0THR = ch);
}

void UART0_write_str(char* str){ /* send string to the serial port */
	int i=0;
	while(str[i]!='\0'){
		putchar(str[i]);
		i++;
	}
}
	
int main() {
		unsigned char name[20] = "ENCS4110";
		unsigned char msg1[50] = "Welcome to Serial Communication Lab:)";
		unsigned char msg2[20] = "write here:";
		IO1DIR = 0x00FE0000; /* LCD pins set as output P1.16 ..P1.19 */
		UART0_init();
		Delay(100);
		UART0_write_str(msg1);
		putchar(NEW_LINE);
		UART0_write_str(msg2);
		LCD_Init(); /* Initialise LCD */
		Delay(100);
		while(1)
		{
			LCD_Data(UART0_read());
			Delay(50);
		}
}