#include "lpc213x.h"


void Delay(unsigned int times){
	int i, j;
	for (j = 0; j < times; j++)
	for (i = 0; i < 300; i++)
	;
}


void UART0_init(void){
	PINSEL0 |= 0x5; /* Enable RxD0 and TxD0 */
	U0LCR = 0x83; /* 8 bits, no Parity, 1 Stop bit */
	U0DLL = 97; /* 9600 Baud Rate @ 12MHz VPB Clock */
	U0LCR = 0x03; /* DLAB = 0 */
}

void UART1_init(void){
	PINSEL0 |= 0x50000; /* Enable RxD0 and TxD0 */
	U1LCR = 0x83; /* 8 bits, no Parity, 1 Stop bit */
	U1DLL = 97; /* 9600 Baud Rate @ 12MHz VPB Clock */
	U1LCR = 0x03; /* DLAB = 0 */
}

int putchar0 (int ch) /* Write single character to Serial Port */
{
	while (!(U0LSR & 0x20));
	return (U0THR = ch);
}

int putchar1 (int ch) /* Write single character to Serial Port */
{
	while (!(U1LSR & 0x20));
	return (U1THR = ch);
}



int main() {
    UART0_init();
		Delay(100);
    UART1_init();
	Delay(100);

    while(1) {
			
        putchar0(96); 
			
        putchar1(97); 
				Delay(100);

    }
}
