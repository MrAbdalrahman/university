#include<lpc213x.h>
#define bit(x) 1<<x
int num[] = {0,0};
int Index = 0;
int i = 0;
int current = 0;
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
IO0DIR |= 0x000000EF; //P0.0 ...P0.7 output
IO0DIR |= (1<<22)|(1<<23);
	IO0SET = (1 << 22)|(1 << 23);
IO1DIR &= ~(0x000F0000); //P1.16 ...P1.19 input
IO1DIR |= 0x00700000; // P1.20 ...P1.22 output
	IO0DIR |= (1<<21);
	IO0SET = (1<< 21);
	IO1DIR &= (~(1<<23));
	
while(1){
	if (current == 2){
		current = 0; 
	}
	
	if (current == 0){
		Index = 0;
	}
	
	else if (current == 1)
		Index = 1;
	
	
	
//test first column
IO1CLR = bit(20);
IO1SET = bit(21)| bit(22);
if(!(IO1PIN & bit(16))){
msDelay(50);
IO0CLR = 0x000000FF;
	num[Index] = 1;
//IO0PIN |= dec_to_7seg(1);
	current += 1;
}
else if (!(IO1PIN & bit(17))){
msDelay(50);
IO0CLR = 0x000000FF;
//IO0PIN |= dec_to_7seg(4);
	num[Index] = 4;
	current += 1;
}
else if (!(IO1PIN & bit(18))){
msDelay(50);
IO0CLR = 0x000000FF;
	
	//IO0PIN |= dec_to_7seg(7);
	num[Index] = 7;
	current += 1;
}
//test second column
 IO1CLR = bit(21);
IO1SET = bit(20)| bit(22);
if(!(IO1PIN & bit(16))){
msDelay(50);
IO0CLR = 0x000000FF;
	num[Index] = 2;
//IO0PIN |= dec_to_7seg(2);
	current += 1;
}
else if (!(IO1PIN & bit(17))){
msDelay(50);
IO0CLR = 0x000000FF;
	num[Index] = 5;
//IO0PIN |= dec_to_7seg(5);
	current += 1;
}
else if (!(IO1PIN & bit(18))){
msDelay(50);
IO0CLR = 0x000000FF;
	num[Index] = 8;
//IO0PIN |= dec_to_7seg(8);
	current += 1;
}
else if (!(IO1PIN & bit(19))){
msDelay(50);
IO0CLR = 0x000000FF;
//IO0PIN |= dec_to_7seg(0);
	num[Index] = 0;
	current += 1;
}
 // test third column
 
 IO1CLR = bit(22);
IO1SET = bit(20)| bit(21);
if(!(IO1PIN & bit(16))){
msDelay(50);
IO0CLR = 0x000000FF;
//IO0PIN |= dec_to_7seg(3);
	num[Index] = 3;
	current += 1;
}
else if (!(IO1PIN & bit(17))){
msDelay(50);
IO0CLR = 0x000000FF;
	num[Index] = 6;
//IO0PIN |= dec_to_7seg(6);
	current += 1;
}
else if (!(IO1PIN & bit(18))){
msDelay(50);
IO0CLR = 0x000000FF;
	num[Index] = 9;
//IO0PIN |= dec_to_7seg(9);
	current += 1;
}

if (!(IO1PIN & (1<<23))){
	
	int whole_num = num[1] + (num[0]*10);
	msDelay(50);
	if (whole_num % 2 == 0)
	{
		IO0CLR = (1<< 21);
		for (i = 0 ; i < 1000 ; i++){
		
		
		
			IO0CLR = (1<<22);
			IO0CLR = 0x0000007F;
			IO0PIN |= dec_to_7seg(num[0]);
			msDelay(10);
			IO0SET = (1<<22);
		
			IO0CLR = (1<<23);
			IO0CLR = 0x0000007F;
			IO0PIN |= dec_to_7seg(num[1]);
			msDelay(10);
			IO0SET = (1<<23);
		}
	}
		else{
			IO0SET = (1<<21);
		}
	
	

}
}
}