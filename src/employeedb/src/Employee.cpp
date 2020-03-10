#include"Employee.h"
#include<iostream>
Employee::Employee(
	const std::string& firstName,
	const std::string& lastName
)
:mFirstName(firstName),
 mLastName(lastName)
{
}
Employee::~Employee(){
}

void Employee::promote(int raiseAmount){
	this->mSalary+=raiseAmount;
}
void Employee::demote(int demiritAmount){
	this->mSalary-=demiritAmount;
}
void Employee::hire(){
	this->mHired=true;
}
void Employee::fire(){
	this->mHired=false;
}
void Employee::display() const{
	std::cout
		<<"First Name:       "
		<<(this->mFirstName)
		<<std::endl
		<<"Last Name:        "
		<<(this->mLastName)
		<<std::endl
		<<"Employee Number:  "
		<<(this->mEmployeeNumber)
		<<std::endl
		<<"Salary:           "
		<<(this->mSalary)
		<<std::endl
		<<"Hired:            "
		<<(this->mHired?"true":"false")
		<<std::endl
		<<"----------------------------------------"
		<<std::endl

	;
}
void Employee::setFirstName(const std::string& firstName){
	this->mFirstName=firstName;
}
const std::string& Employee::getFirstName() const{
	return this->mFirstName;
}
void Employee::setLastName(const std::string& lastName){
	this->mLastName=lastName;
}
const std::string& Employee::getLastName() const{
	return this->mLastName;
}
void Employee::setEmployeeNumber(int employeeNumber){
	this->mEmployeeNumber=employeeNumber;
}
int Employee::getEmployeeNumber() const{
	return this->mEmployeeNumber;
}
void Employee::setSalary(int newSalary){
	this->mSalary=newSalary;
}
int Employee::getSalary() const{
	return this->mSalary;
}
bool Employee::isHired() const{
	return this->mHired;
}
