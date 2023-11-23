//AbdulRahmanHythamShaheen/1211753/labSec2/lectureSec2
package ass1_1211753_2;

public class Equation {
public static boolean isBalanced(int id) { // a method to check if the id number is balanced or not 
	int tempId=id,evenSum=0,oddSum=0;
	while (tempId!=0) {
		if (tempId%10==(tempId/10)%10)
			return false;
		tempId/=10;
	}	
while (id!=0){
	if ((id%10)%2==0)
		evenSum+=id%10;
	else 
		oddSum+=id%10;
	id/=10;
	}
	if (evenSum==oddSum)
	return true;
return false;
}// note that i used a temporary integer so I can use id number without affecting it 
	
	public static double	solveEquation(double x, double a, double b, double c) {
	return (a*x*x)+(b*x)+c;
}
public static void findEquationRoots(double a,double b,double c) {
	double n=(b*b)-(4*a*c); // here n indicates to (b^2-4ac) and in math if n is greater than 0 it has two roots and if its less than 0 it has imaginary roots and if its equal to 0 it has one root
	if (n==0)
		System.out.printf( "x=%.4f ",(Math.sqrt(n)-b)/(2*a));
	else if(n>0)
		System.out.printf("root 1 x=%.4f root 2 x=%.4f\n",(Math.sqrt(n)-b)/(2.0*a),(-1.0*(Math.sqrt(n))-b)/(2.0*a));
else 
		System.out.printf("imaginary numbers exist\ni:imaginary number\nroot 1 x=%.2f+%.2fi and root 2 x=%.2f-%.2fi\n",-b/(2*a),Math.sqrt(-n)/(2*a),-b/(2*a),Math.sqrt(-n)/(2*a));
}
public static void combineEquations (double a,double b,double c,double a2,double b2,double c2) {
	System.out.printf("%.1fx2%+.1fx%+.1f\n",a+a2,b+b2,c+c2);
}
}