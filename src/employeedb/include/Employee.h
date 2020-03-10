#ifndef EMPLOYEE_H
#define EMPLOYEE_H
#include<string>
const int kDefaultStartingSalary=1000;
class Employee{
	public:
		Employee()=default;
		Employee(
			const std::string& firstName,
			const std::string& lastName
		);
		~Employee();
		void promote(int raiseAmount=1000);
		void demote(int demiritAmount=1000);
		void hire();
		void fire();
		void display() const;
		void setFirstName(const std::string& firstName);
		const std::string& getFirstName() const;
		void setLastName(const std::string& lastName);
		const std::string& getLastName() const;
		void setEmployeeNumber(int employeeNumber);
		int getEmployeeNumber() const;
		void setSalary(int newSalary);
		int getSalary() const;
		bool isHired() const;
	private:
		std::string mFirstName;
		std::string mLastName;
		int mEmployeeNumber=-1;
		int mSalary=kDefaultStartingSalary;
		bool mHired=false;
	protected:
};
#endif
