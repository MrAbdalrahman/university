#define NEW_LINE 0x0D
#include <LPC213x.H>
void UART0_init(void);
int putchar (int ch);
void UART0_write_str(char *msg);
int main() {
char msg1[]="Welcome to Serial Communication Lab :)";
char msg2[] = "End";
UART0_init();
UART0_write_str(msg1);
putchar(NEW_LINE);
UART0_write_str(msg2);
while(1);
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
void UART0_write_str(char *str){ /* send string to the serial port */
int i=0;
while(str[i]!='\0'){
putchar(str[i]);
i++;
}
}