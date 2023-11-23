package account;
import java.util.Date;
public class Account {
private int id;
private double balance;
private Date dateCreated;
public Account() {
	id=0;
	balance=0;
	dateCreated=new Date();
}
public Account(int id,double balance) {
	this.id=id;
	this.balance=balance;	
}
public int getId() {
	return id;
}
public void setId(int id) {
	this.id = id;
}
public double getBalance() {
	return balance;
}
public void setBalance(double balance) {
	this.balance = balance;
}
public Date getDateCreated() {
	return dateCreated;
}
public void withdraw(double amount){
	balance-=amount;
	System.out.println("your balance is: "+this.balance+"now");
}
public void deposite(double amount) {
	balance+=amount;
	System.out.println("your balance is: "+this.balance+"now");
}
public String toString() {
	return "Account [id=" + id + ", balance=" + balance + ", dateCreated=" + dateCreated + "]";
}




}
