#include <lpc213x.H>
#define NEW_LINE 0x0D
#define RS 0x00020000 /* RS - P1.17 */
#define RW 0X00040000 /* R/W - P1.18 */
#define EN 0X00080000 /* E - P1.19 */
#define CLR 0X00FE0000



	
void Delay(unsigned int times){
	int i, j;
	for (j = 0; j < times; j++)
	for (i = 0; i < 300; i++)
	;
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

int main() {
	
		char temp;
	char keyWord[] = "hi";
	int index = 0;
		IO0DIR |= (1 << 4); 
		UART0_init();
		Delay(100);
		
		
		while(1)
		{
			temp = UART0_read();
			if (temp == keyWord[index]) 
            index++; 
			else {
				 IO0CLR = (1 << 4);
				index = 0;
			}
			
			if (index == 2) {
            IO0SET = (1 << 4); 
            index = 0; 
        }
			Delay(50);
			
		}
}