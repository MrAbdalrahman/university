#include <lpc213x.h>
#define RS 0x00020000 
#define RW 0X00040000 
#define EN 0X00080000 
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




int main()
{
 unsigned char name[] = "abdel rahman";
 IO1DIR = 0x00FE0000; /* LCD pins set as o/p????????? */
 LCD_Init(); /* Initialise LCD */
while(1) {
 LCD_Command1(FIRST_ROW);
 LCD_String(name);
 Delay(50);
 LCD_Command1(SECOND_ROW);
 LCD_String("1211753");
 Delay(50);

 //LCD_Command1(LCD_CLEAR); /* Clear screen */
 //Delay(80);
}

}


