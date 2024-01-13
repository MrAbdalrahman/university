#include<lpc213x.h>
#define bit(x) 1<<x
void msDelay(int d) {
int i,j;
long c=0;
d=d*2;
for(i=0;i<d;i++){
for(j=0;j<1000;j++){
c++;
}
}
}
int dec_to_bcd(int number)
{
switch(number){
case 0 : return 0;
case 1 : return 1;
case 2 : return 2;
case 3 : return 3;
case 4 : return 4;
case 5 : return 5;
case 6 : return 6;
case 7 : return 7;
case 8 : return 8;
case 9 : return 9;
default : return 0;
}
}
int main(){
IO0DIR |= 0x0000000F; // P0.0, P0.1, P0.2, and P0.3 as output
IO1DIR &= ~(0x000F0000); //P1.16 ...P1.19 input
IO1DIR |= 0x00700000; // P1.20 ...P1.22 output
while(1){
//test first column
IO1CLR = bit(20);
IO1SET = bit(21)| bit(22);
if(!(IO1PIN & bit(16))){
msDelay(50);
IO0CLR = 0x0000000F;
IO0PIN |= dec_to_bcd(1);
}
else if (!(IO1PIN & bit(17))){
msDelay(50);
IO0CLR = 0x0000000F;
IO0PIN |= dec_to_bcd(4);
}
else if (!(IO1PIN & bit(18))){
msDelay(50);
IO0CLR = 0x0000000F;
	
	IO0PIN |= dec_to_bcd(7);
}
//test second column
 IO1CLR = bit(21);
IO1SET = bit(20)| bit(22);
if(!(IO1PIN & bit(16))){
msDelay(50);
IO0CLR = 0x0000000F;
IO0PIN |= dec_to_bcd(2);
}
else if (!(IO1PIN & bit(17))){
msDelay(50);
IO0CLR = 0x0000000F;
IO0PIN |= dec_to_bcd(5);
}
else if (!(IO1PIN & bit(18))){
msDelay(50);
IO0CLR = 0x0000000F;
IO0PIN |= dec_to_bcd(8);
}
else if (!(IO1PIN & bit(19))){
msDelay(50);
IO0CLR = 0x0000000F;
IO0PIN |= dec_to_bcd(0);
}
 // test third column
 
 IO1CLR = bit(22);
IO1SET = bit(20)| bit(21);
if(!(IO1PIN & bit(16))){
msDelay(50);
IO0CLR = 0x0000000F;
IO0PIN |= dec_to_bcd(3);
}
else if (!(IO1PIN & bit(17))){
msDelay(50);
IO0CLR = 0x0000000F;
IO0PIN |= dec_to_bcd(6);
}
else if (!(IO1PIN & bit(18))){
msDelay(50);
IO0CLR = 0x0000000F;
IO0PIN |= dec_to_bcd(9);
}
}
}
