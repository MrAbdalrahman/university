#include <LPC213X.H>
int main ()
	{
	IO0DIR &= ~(0x000F0000); // p.0.16-p.0.19 inputs
	IO1DIR |=  (0x000F0000); // p.1.16-p.0.19 outputs
	while(1) // run forever
	IO1PIN = IO0PIN;  // write port0 data on port1
}
	
