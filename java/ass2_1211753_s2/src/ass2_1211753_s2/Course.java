//AbdulRahmanHythamShaheen/1211753/labSec2/lectureSec2
package ass2_1211753_s2;
public class Course {
	private String courseName;            // this is read only 
	private int courseId;                // this is read only
	public Course(){                    //default constructor
		courseName="JAVA";
		courseId=0;
	}
	/**
	 *  A non default constructor
	 * @param courseName course's name
	 * @param courseId course's id
	 */
	public Course(String courseName,int courseId){
		if(courseName.length()<=4)
			this.courseName=courseName;
		this.courseId=courseId;
	}
	public	String getCourseName() {
	return this.courseName;
}
	public	int getCourseId() {
	return this.courseId;
}
	// a method to print all course information
	public void printInfo() {
	System.out.print("Course name : "+courseName+" course id:"+courseId+" ");
}
}