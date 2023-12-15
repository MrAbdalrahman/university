#define NEW_LINE 0x0D
#include <LPC213x.H>
void UART0_init(void);
int putchar (int ch);


void UART0_init(){
PINSEL0 = 0x5; /* Enable RxD0 and TxD0 */
U0LCR = 0x83; /* 8 bits, no Parity, 1 Stop bit */
U0DLL = 97; /* 9600 Baud Rate @ 12MHz VPB Clock */
U0LCR = 0x03; /* DLAB = 0 */
}
int UART0_write(int data){ /* send data to the serial port */

while (!(U0LSR & 0x20));
 return U0THR = data;
}



// master main
int main (){
	IO0DIR &= (~0x3C);
	UART0_init();
	while (1){
		UART0_write(IO0PIN);
	}
	
	
	
	
}