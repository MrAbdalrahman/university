#define NEW_LINE 0x0D
#include <LPC213x.H>
void UART0_init();
void Delay(unsigned int times){
	int i, j;
	for (j = 0; j < times; j++)
	for (i = 0; i < 300; i++)
	;
}

void UART0_init(){
PINSEL0 = 0x5; /* Enable RxD0 and TxD0 */
U0LCR = 0x83; /* 8 bits, no Parity, 1 Stop bit */
U0DLL = 97; /* 9600 Baud Rate @ 12MHz VPB Clock */
U0LCR = 0x03; /* DLAB = 0 */
}
int UART0_read(void) {
 while (!(U0LSR & 0x1)); //wait until the receiver buffer is full
 return(U0RBR);
}



// slvae main
int main (){
	IO0DIR |= 0x3C;
	UART0_init();
	Delay(100);
	while (1){
		IO0PIN = UART0_read();
	}
	
}