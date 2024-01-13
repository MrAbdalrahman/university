#include<lpc213x.h>
#define bit(x) 1<<x
const int password[] = {1, 2, 3, 4}; 

int currentPassword[4];
int passwordIndex = 0;
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
return number;

}
int main(){
IO0DIR |= 0x00000001;
IO1DIR &= ~(0x000F0000); //P1.16 ...P1.19 input
IO1DIR |= 0x00700000; // P1.20 ...P1.22 output
 

while(1){
//test first column
IO1CLR = bit(20);
IO1SET = bit(21)| bit(22);
	
if(!(IO1PIN & bit(16))){
msDelay(50);
	
IO0CLR = 0x00000001;
	
currentPassword[passwordIndex++] = dec_to_bcd(1);
}

else if (!(IO1PIN & bit(17))){
msDelay(50);
IO0CLR = 0x00000001;
	
currentPassword[passwordIndex++] = dec_to_bcd(4);
}

else if (!(IO1PIN & bit(19))){

	
	int correct = 1;
	int i = 0;
	
	passwordIndex = 0;
	
		for (i = 0 ;i < 4;i= i+1){
		if (currentPassword[i] != password[i]){
			correct = 0;
			break;
		}
	}
	
	if (correct) 
		
    IO0SET = 0x00000001 ; // Set P0.0 high to turn on the LED
}


else if (!(IO1PIN & bit(18))){
msDelay(50);
IO0CLR = 0x00000001;
	
currentPassword[passwordIndex++]= dec_to_bcd(7);
}
//test second column
 IO1CLR = bit(21);
IO1SET = bit(20)| bit(22);
if(!(IO1PIN & bit(16))){
msDelay(50);
IO0CLR = 0x00000001;
currentPassword[passwordIndex++] = dec_to_bcd(2);
}
else if (!(IO1PIN & bit(17))){
msDelay(50);
IO0CLR = 0x00000001;
currentPassword[passwordIndex++] = dec_to_bcd(5);
}
else if (!(IO1PIN & bit(18))){
msDelay(50);
IO0CLR = 0x00000001;
currentPassword[passwordIndex++] = dec_to_bcd(8);
}
else if (!(IO1PIN & bit(19))){
msDelay(50);
IO0CLR = 0x00000001;
currentPassword[passwordIndex++] = dec_to_bcd(0);
}
 // test third column
 
 IO1CLR = bit(22);
IO1SET = bit(20)| bit(21);
if(!(IO1PIN & bit(16))){
msDelay(50);
IO0CLR = 0x00000001;
currentPassword[passwordIndex++] = dec_to_bcd(3);
}
else if (!(IO1PIN & bit(17))){
msDelay(50);
IO0CLR = 0x00000001;
currentPassword[passwordIndex++] = dec_to_bcd(6);
}
else if (!(IO1PIN & bit(18))){
msDelay(50);
IO0CLR = 0x00000001;
currentPassword[passwordIndex++] = dec_to_bcd(9);

}


}
}


