//AbdulRahmanHythamShaheen/1211753/labSec2/lectureSec2
package ass1_1211753_2;
import java.util.Scanner;
public class CheckEquation {
	public static void main(String[] args) {
		Scanner scan=new Scanner(System.in);
int idNumber,choice=0;
double x,a,b,c,a2,b2,c2;	
String firstName;		
		do {
		System.out.println("please enter your first name");
firstName=scan.next();
 scan.nextLine(); // this scan helps to get rid of the rest of the first name line so id could be read
System.out.println("please enter your Id numebr or -1 to exit program");
do {                     // a do while loop to check the validity of an id number
	idNumber=scan.nextInt();
if (idNumber!=-1)
	if (Equation.isBalanced(idNumber)==false)
	System.out.printf("%d is invalid (the id number should be balanced) please try again or enter -1 to exist \n",idNumber);
else
	break;
}
while (idNumber!=-1);	
if (idNumber!=-1) {  //if id was -1 it skips to the end of the program else it will continue
System.out.println("Hello "+firstName);
	do {
	System.out.println("please choose the operation you want to run\n1- Solve a quadratic equation\n2- Find the root values of a quadratic equation\n3- Combine two quadratic equations\n4- Exit menu ");
	choice=scan.nextInt();    // scanning choice
	switch(choice) { // switch statement to run  operation
		case 1:System.out.println("please enter x and (coefficients) a,b and c in order");  // Solve a quadratic equation
x=scan.nextDouble();
	a=scan.nextDouble();
		b=scan.nextDouble();
			c=scan.nextDouble();
	System.out.println("the quadratic solution is "+Equation.solveEquation(x,a,b,c));
				break;
		case 2:System.out.println("please enter (coefficients) a,b and c in order");   // Find the root values of a quadratic equation
		 a=scan.nextDouble();
				 b=scan.nextDouble();
				 	c=scan.nextDouble();
		Equation.findEquationRoots(a,b,c);
			break;
		case 3:                                                                            
  			System.out.println("please enter (coefficients) a,b and c for the first equation in order");    // Combine two quadratic equations
				a=scan.nextDouble();
					b=scan.nextDouble();
						c=scan.nextDouble();
		System.out.println("please enter (coefficients) a,b and c for the seconde equation in order");
			 a2=scan.nextDouble();
			 	b2=scan.nextDouble();
			 		c2=scan.nextDouble();
			 Equation.combineEquations(a,b,c,a2,b2,c2);
			break;
		case 4: System.out.println("Exiting menu"); // Exit menu
			break;
		default: System.out.println("invalid operation,please enter a valid operation from 1-4"); // if user didn't choose 1-4
	}
}
while(choice!=4);  // a do while loop to keep viewing menu until user enters 4
}
		}
		while (idNumber!=-1); //a do while loop to rerun program until user enters an id of value -1
	System.out.println("thank you Mr "+firstName+" for using my program");  // a sign that program finished
	scan.close();  // closing the scanner because it gives me a warning if i didn't
	}
}