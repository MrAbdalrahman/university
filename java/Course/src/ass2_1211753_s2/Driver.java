//AbdulRahmanHythamShaheen/1211753/labSec2/lectureSec2
package ass2_1211753_s2;
import java.util.Scanner;
public class Driver {
	public static void main(String[] args) {
	int tempId,tempId2,size=0 ;
		Scanner scan=new Scanner(System.in);
		System.out.println("Enter number of teachers");
	int numOfTeacher,numOfCourses;  
	numOfTeacher=scan.nextInt();
	scan.nextLine();
	Teacher[] teachers=new Teacher[numOfTeacher];    // Array of type Teacher to save each teacher's information
	String tempCourseName; // A temp String to save each teacher's course's information
	boolean validity; // A boolean variable to check if the the course's id valid
	for ( int i=0;i<numOfTeacher;i++) {	//A for loop for every teacher entered
		System.out.println("Enter teacher’s name, id, basic salary, and extra payment rate in order (each info in line)");
		teachers[i]=new Teacher(scan.next(),scan.nextInt(),scan.nextDouble(),scan.nextDouble());
		System.out.println("Enter number of courses taught");
		scan.nextLine();
		numOfCourses=scan.nextInt();
		teachers[i].setCoursesSize(numOfCourses);
		scan.nextLine();
		for (int j=0;j<numOfCourses;j++) //A for loop for every course entered for each teacher
		{          validity=false; 
			do {                                     //A do while loop for the validity for each course name		
				System.out.println("Enter Course name");
				 tempCourseName=scan.nextLine();
				if (tempCourseName.length()>4 || tempCourseName.length()<1) {
				System.out.println("invalid course name (course name should consist of 4 charachters or less and cant be empty)");
				}
				else
				validity=true;
			}while (!validity);
			validity=false;
			do {
			size=0;
				System.out.println("please enter the id of the course");
			 tempId=scan.nextInt();
			 tempId2=tempId;
			while (tempId2!=0) {
				tempId2/=10;
				size++;
			}
			if((tempId/(int)Math.pow(10,(size-1)))>4)
				System.out.println("invalid course id (first digit of the id should be between(1-4))");
			else
				validity=true;
			}while (!validity);
			Course tempCourse= new Course(tempCourseName,tempId);
			teachers[i].setCoursesTaught(tempCourse,j);
			scan.nextLine();
		}//end of courses for loop
	}// end of teachers for loop
	int choice,k;
	double tempSalary;
	double sum=0;
	do { // A do while loop to view the menu after completing each task
	System.out.println("Enter your choice of tasks ( 1-5 ):\n1-Print teacher Info\n2-Display all teachers names and total Salaries\n3-Change basic salary for a teacher\n4-Display sum of total salaries for all teachers\n5-Exit");
	choice=scan.nextInt();
	scan.nextLine();
	switch(choice) {
	case 1:System.out.println("please enter teacher's id"); // task 1 case, that prints the information for the teacher whom id is entered
	tempId=scan.nextInt();
	scan.nextLine();
	boolean exists=false;
	for ( k=0;k<teachers.length;k++) {
		if (tempId==teachers[k].getTeacherId()) {
			exists=true;
			break;	
		}
	}
	if (exists)
		teachers[k].printInfo();
	else System.out.println("teacher whom id is: "+tempId+" does not exist");
		break;
	case 2:for (int i=0;i<numOfTeacher;i++)                              // task 2 case, to print all teachers names and total Salaries 
	{
	System.out.println("Name: "+teachers[i].getTeacherName()+" Total Salary: "+teachers[i].getTeacherTotalSalary());
	}
		break;
	case 3:System.out.println("please enter teacher's id");             // task 3 case, to change basic salary for the teacher whom id is entered
	tempId=scan.nextInt();
	scan.nextLine();
	 exists=false;
	for ( k=0;k<teachers.length;k++) {
		if (tempId==teachers[k].getTeacherId()) {
			exists=true;
			break;	
		}
	}
	if (exists) {
		System.out.println("please enter teacher's new basic Salary");
		tempSalary=scan.nextDouble();
		scan.nextLine();
		teachers[k].setBasicSalary(tempSalary);
		}
	else System.out.println("teacher whom id is: "+tempId+" does not exist");
		break;
	case 4:for (int i=0;i<numOfTeacher;i++) {  							 //task 4 case, to print the sum of total salaries for all teachers
		sum+=teachers[i].getTeacherTotalSalary();
	}
		System.out.println("the sum of total salaries for all teachers is: "+sum);
		break;
	case 5:System.out.println("exiting program...");					 // Exit case
		break;				
		default :                                    						// in case that the user entered number that is not {1,2,3,4,5}
			System.out.println("invalid choice please choose between(1-5)");
	}
	}while(choice!=5);
	scan.close();
	System.out.println("thanks for using my program");
	}
}