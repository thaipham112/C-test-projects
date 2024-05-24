//Name: Thai Pham
//I work alone in this project on 04/30/24.

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
//define the color to change the text colors
#define RESET "\033[0m" //reset to the normal color
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define CYAN "\033[36m"
using namespace std;

//declare class Account
class Account {
protected:
	double balance;
	int numOfDeposits = 0;
	int numOfWithdrawals = 0;
	double annualRate;
	double mServiceCharge = 0;
public:
	Account(double balance, double annualRate) { //constructor
		this->balance = balance;
		this->annualRate = annualRate;
	}
	Account() { //default constructor
		balance = 0;
		annualRate = 0;
	}
	virtual void deposit(double amount) { //function to deposit
		balance = balance + amount;
		numOfDeposits++;
	}
	virtual void withdraw(double amount) { //function to withdraw
		balance = balance - amount;
		numOfWithdrawals++;
	}
	virtual void calcInt() { //function to calculate and add the interest
		balance = balance + (balance * (annualRate / 12));
		//annualRate / 12 is the monthly interest. Balance * (annualRate/12) is the amount of interest.
	}

	int times = 0; //declare a variable solely for the function monthlyProc
	//if I combine all the codes in the monthlyProc function below, I would reset all the service charges value to 0 before printing it out
	virtual void monthlyProc() { //minus the amount of service fee to the balance
		if (times % 2 == 0) { //if this is the first time calling the function, minus the service fee and add the interest amount 
			balance = balance - mServiceCharge;
			if (balance > 0) { //add interest to the balance only and only if the balance is over 0
				calcInt();
			}
			times++;
		}
		else { //the second time the function is called, reset all the amount to 0
			mServiceCharge = 0;
			numOfDeposits = 0;
			numOfWithdrawals = 0;
			times++;
		}
	}
	//accessor functions
	int getNumOfWith() {
		return numOfWithdrawals;
	}
	int getNumOfDep() {
		return numOfDeposits;
	}
	double getServiceCharge() {
		return mServiceCharge;
	}
	double getBalance() {
		return balance;
	}
};

class SavingAccounts : public Account {
private:
	bool status; //declare a bool variable to check for the status of the account
public:
	SavingAccounts() {
		balance = 0;
		annualRate = 0;
		status = 0;
	}
	SavingAccounts(double balance, double annualRate) { //constructor
		this->balance = balance;
		this->annualRate = annualRate;
		isActive(); //check if the balance amount would make the account active or inactive
	}
	void isActive() { //function to check for the status of the bank account
		if (balance < 25) {
			status = 0;
		}
		else status = 1;
	}
	void withdraw(double amount) { //function to withdraw
		if (status == 1) { //if the account is active, withdraw the amount then check for status
			Account::withdraw(amount);
			isActive();
		}
		//if the account is not active, don't check for it
		else cout << RED << "The balance is below $25. The account is inactive. " << RESET << endl; 
	}
	void deposit(double amount) {
		Account::deposit(amount);
		if (balance >= 25) isActive(); 
		//in case the account is not active, if the balance is more than 25 after deposit, the account will be active
	}
	void monthlyProc() {
		if (numOfWithdrawals > 4) { //if the number of withdrawals is more than 4, add a fee $1 for each withdrawals
			mServiceCharge = mServiceCharge + 1 * (numOfWithdrawals - 4);
		}
		Account::monthlyProc(); //then call the base class's function
		isActive(); //check if the account is active when the service fee is charged
	}
};

class checkingAccount : public Account {
private:
	int invalidWithdrawal = 0; //variable for invalid withdrawal (not enough balance)
public:
	checkingAccount(double balance, double annualRate) { //constructor
		this->balance = balance;
		this->annualRate = annualRate;
	}
	checkingAccount() { //default constructor
		balance = 0;
		annualRate = 0;
	}
	void withdraw(double amount) { 
		if (balance - amount < 0) {// if the amount of withdrawal is larger than the balance
			cout << "Not enough balance. A $15 service charge will be withdrawn at the end of the month." << endl;
			invalidWithdrawal++; //increase the time of invalid withdrawal by 1
		}
		else Account::withdraw(amount); //else do a normal withdrawal
	}
	void monthlyProc() { //service charge
		mServiceCharge = 5 + numOfWithdrawals * 0.1 + invalidWithdrawal * 15; //monthly $5 plus $0.10 for each withdrawal
		invalidWithdrawal = 0; //reset the amount of invalid withdrawal (not enough balance withdrawal)
		Account::monthlyProc(); //then call the base class's function
	}
};

int main() {
	cout << RED << "WELCOME TO OUR BANKING APPLICATION" << RESET;
	SavingAccounts a(100, 0.1); //set the balance of saving and checking accounts to 100 and interest rate to 0.1
	checkingAccount b(100, 0.1);

	for (;;) { //infinite loop
		int choice; 
		//print out the menu, then tell the user to input their choice from 1 to 6
		cout << BLUE << "\n\n******** BANK ACCOUNT MENU ********\n\n\n" << CYAN << "1. Savings Account Deposit\n2. Checking Account Deposit\n3. Savings Account Withdrawal"
			<< "\n4. Checking Account Withdrawal\n5. Update and Display Account Statistics\n6. Exit\n\n\n" << RESET;
		cout << "Please enter your choice from 1 to 6: "; cin >> choice;

		while (choice > 6 || choice < 1) { //while loop in case the user not enter the correct choice
			cout << "Invalid choice. Please enter again: "; cin >> choice;
		}

		if (choice == 1) { //if the choice is to deposit into saving account
			double amount;
			//tell the user to enter the amount deposit
			cout << GREEN << "Enter amount to deposit: " << RESET; cin >> amount;
			a.deposit(amount); //activate the function to deposit
		}
		if (choice == 2) { //if the choice is to deposit into checking account
			double amount; 
			//tell the user to enter the amount deposit
			cout << GREEN << "Enter amount to deposit: " << RESET; cin >> amount;
			b.deposit(amount); //activate the function to deposit
		}
		if (choice == 3) { //if the choice is to withdraw from saving account
			double amount;
			//tell the user to enter the amount withdrawal
			cout << RED << "Enter amount to withdrawal: " << RESET; cin >> amount;
			a.withdraw(amount); //activate the function to withdraw
		}
		if (choice == 4) { //if the choice is to withdraw from checking account
			double amount;
			//tell the user to enter the amount withdrawal
			cout << RED << "Enter amount to withdrawal: " << RESET; cin >> amount;
			b.withdraw(amount); //activate the function to withdraw
		}

		if (choice == 5) {
			ofstream file("output.txt"); // create an ofstream object

			a.monthlyProc(); //update the account by calling the function for the first time
			//print out the results
			string output = "\n\nSAVINGS ACCOUNT MONTHLY STATISTICS:\nWithdrawals: " + to_string(a.getNumOfWith()) + 
				"\nDeposits: " + to_string(a.getNumOfDep()) + "\nService Charges: $" + to_string(a.getServiceCharge()) + 
				"\nEnding Balance: $" + to_string(a.getBalance()) + "\n"; //to_string to convert from int or double to string

			cout << YELLOW << output << RESET; //write to the console
			file << output; // write to the file
			a.monthlyProc(); //update the account by calling the function for the second time (reset some amounts to 0)


			b.monthlyProc(); //update the account by calling the function for the first time
			//print out the results
			output = "\nCHECKING ACCOUNT MONTHLY STATISTICS:\nWithdrawals: " + to_string(b.getNumOfWith()) + 
				"\nDeposits: " + to_string(b.getNumOfDep()) + "\nService Charges: $" + to_string(b.getServiceCharge()) + 
				"\nEnding Balance: $" + to_string(b.getBalance()) + "\n"; //to_string to convert from int or double to string

			cout << output; //write to the console
			file << output; // write to the file
			b.monthlyProc(); //update the account by calling the function for the second time (reset some amounts to 0)

			file.close(); //close the file
		}
		if (choice == 6) break; //if choice is exit, exit the loop, thus exiting the program
	}
	cout << "\nExiting the program..." << endl;
	return 0;
}