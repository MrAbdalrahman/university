#include <lpc213x.h>
#define RS 0x00020000 /* RS - P1.17 */
#define RW 0X00040000 /* R/W - P1.18 */
#define EN 0X00080000 /* E - P1.19 */
#define CLR 0X00FE0000
#define FIRST_ROW 0x80
#define SECOND_ROW 0xC0
#define LCD_CLEAR 0x01
void Delay(unsigned int times)
{
 int i, j;
 for (j = 0; j < times; j++)
 for (i = 0; i < 300; i++)
 ;
}
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
 LCD_Command(0x20);
 LCD_Command1(0x28);
 LCD_Command1(0x01); /* Clear display */
 LCD_Command1(0x06); /* Auto increment */
 LCD_Command1(0x0C); /* Cursor off */
}


void LCD_set_cursor_pos(int row, int col)
{
 unsigned char cp;
 if (row == 0)
 cp = FIRST_ROW;
 else
 cp = SECOND_ROW;
 cp |= col;
 LCD_Command1(cp);
}
int main()
{
 unsigned char name[] = "abdel rahman shahen" ;
	unsigned char id[] = "1211753" ;
 int col1 = 0, row1 = 0,col2 = 0 ,row2 = 0;
 IO1DIR = 0x00FE0000; /* LCD pins set as output P1.16 ..P1.19 */
 IO0DIR &= (0xFFFFFFF0); //Push button keys as inputs P0.0 ..P0.3
 LCD_Init(); /*Initialise LCD */\
	
while(1) {
if (!(IO0PIN & (1 << 0))) {
 LCD_Command1(FIRST_ROW);
	LCD_String(name);
	Delay(20);
	
	LCD_Command1(SECOND_ROW);
	LCD_String(id);
	Delay(20);
	
	while(1){
	if (!(IO0PIN & (1<<0))){
		col1--;
	if (col1 < 0)
		col1 = 15;
	LCD_Command1(LCD_CLEAR);
	 LCD_set_cursor_pos(row1, col1);
 LCD_String(name);
 Delay(100);
	col2++;
	 col2%=16;
	 Delay(100);
	LCD_set_cursor_pos(SECOND_ROW, col2);
	LCD_String(id);
	Delay(100);
}
 }
 }
}
}