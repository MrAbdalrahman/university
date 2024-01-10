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
int dec_to_7seg(int number)
{
switch(number){
case 0 : return 0x3F;
case 1 : return 0x06;
case 2 : return 0x5B;
case 3 : return 0x4F;
case 4 : return 0x66;
case 5 : return 0x6D;
case 6 : return 0x7D;
case 7 : return 0x07;
case 8 : return 0x7F;
case 9 : return 0x6F;
default : return 0x00;
}
}
int main(){
IO0DIR |= 0x000000FF; //P0.0 ...P0.7 output
IO1DIR &= ~(0x000F0000); //P1.16 ...P1.19 input
IO1DIR |= 0x00700000; // P1.20 ...P1.22 output
while(1){
//test first column
IO1CLR = bit(20);
IO1SET = bit(21)| bit(22);
if(!(IO1PIN & bit(16))){
msDelay(50);
IO0CLR = 0x000000FF;
IO0PIN |= dec_to_7seg(1);
}
else if (!(IO1PIN & bit(17))){
msDelay(50);
IO0CLR = 0x000000FF;
IO0PIN |= dec_to_7seg(4);
}
else if (!(IO1PIN & bit(18))){
msDelay(50);
IO0CLR = 0x000000FF;
	
	IO0PIN |= dec_to_7seg(7);
}
//test second column
 IO1CLR = bit(21);
IO1SET = bit(20)| bit(22);
if(!(IO1PIN & bit(16))){
msDelay(50);
IO0CLR = 0x000000FF;
IO0PIN |= dec_to_7seg(2);
}
else if (!(IO1PIN & bit(17))){
msDelay(50);
IO0CLR = 0x000000FF;
IO0PIN |= dec_to_7seg(5);
}
else if (!(IO1PIN & bit(18))){
msDelay(50);
IO0CLR = 0x000000FF;
IO0PIN |= dec_to_7seg(8);
}
else if (!(IO1PIN & bit(19))){
msDelay(50);
IO0CLR = 0x000000FF;
IO0PIN |= dec_to_7seg(0);
}
 // test third column
 
 IO1CLR = bit(22);
IO1SET = bit(20)| bit(21);
if(!(IO1PIN & bit(16))){
msDelay(50);
IO0CLR = 0x000000FF;
IO0PIN |= dec_to_7seg(3);
}
else if (!(IO1PIN & bit(17))){
msDelay(50);
IO0CLR = 0x000000FF;
IO0PIN |= dec_to_7seg(6);
}
else if (!(IO1PIN & bit(18))){
msDelay(50);
IO0CLR = 0x000000FF;
IO0PIN |= dec_to_7seg(9);
}
}
}
