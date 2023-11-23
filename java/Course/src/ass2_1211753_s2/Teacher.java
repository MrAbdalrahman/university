//AbdulRahmanHythamShaheen/1211753/labSec2/lectureSec2
package ass2_1211753_s2;
public class Teacher {
private String teacherName;
private int teacherId;
private double basicSalary;
private double extraPaymentRate;
private Course[] coursesTaught ;
public Teacher(){								//A non-parameterized (default) constructor
	teacherName="teacher";
	teacherId=0;
	basicSalary=0;
	extraPaymentRate=0;
}
/**
 * A parameterized(non-default) constructor that define and initialize a new teacher with given information  
 * @param teacherName teacher's name
 * @param teacherId teacher's id 
 * @param basicSalary teacher's basic salary
 * @param extraPaymentRate teacher's extra payment rate
 */
public Teacher(String teacherName,int teacherId,double basicSalary,double extraPaymentRate){
	this.teacherName=teacherName;
	this.teacherId=teacherId;
	this.basicSalary=basicSalary;
	this.extraPaymentRate=extraPaymentRate;
}
public void setCoursesTaught(Course courseTaught,int index) {
	 coursesTaught[index]=new Course();
	 coursesTaught[index]=courseTaught;
}
/**
this method is made to specify the number of courses taught to let program know the size of course's Array size to define it * 
 * @param size the size of course's array
 */
public void setCoursesSize(int size) {
	this.coursesTaught=new Course[size];
}
public int getTeacherId() {
	return this.teacherId;
}
public String getTeacherName() {
	return this.teacherName;
}
/**
 * a method to return the total salary by summing basic salary and extra payment
 * @return teacher's total salary
 */
	public double getTeacherTotalSalary() {
		return basicSalary+calculateExtraPayment();
	}
	public void setBasicSalary(double basicSalary) {
		this.basicSalary=basicSalary;
	}
	double getBasicSalary() {
		return basicSalary;
	}
	/**
	 * takes a level as an argument and returns the number of courses of that level 
	 * @param level the required level to count the number of courses of that level
	 * @return the numbers of courses of that level
	 */
public int countCourseLevel(int level) {
	int size,numOfCourses=0,tempid,tempid2;
	for (int i=0;i<coursesTaught.length;i++){
		size=0;
	 tempid=coursesTaught[i].getCourseId();
	 tempid2=tempid;
	while (tempid!=0) {
		tempid/=10;
		size++;
	}
	if(level==tempid2/(int)Math.pow(10,(size-1)))
		numOfCourses++;
	}
	return numOfCourses ;
}
/**
 * A method to calculate the extra payment for the teacher according to a specific equation
 * @return the extra payment for a teacher
 */
 public double calculateExtraPayment() {
	return (1*countCourseLevel(1)*extraPaymentRate)+(2*countCourseLevel(2)*extraPaymentRate)+(3*countCourseLevel(3)*extraPaymentRate)+(4*countCourseLevel(4)*extraPaymentRate);
}
 // A method to print all the information including course information
	public void printInfo() {
		System.out.println("Name: "+this.teacherName+" Id: "+this.teacherId+" basic salary: "+this.basicSalary+" Extra payment rate: "+this.extraPaymentRate+" totalPayment: "+this.getTeacherTotalSalary());
		System.out.print("courses taught: ");
		for (int i=0;i<this.coursesTaught.length;i++)
		{
			coursesTaught[i].printInfo();
		}
		System.out.println();
		}
}