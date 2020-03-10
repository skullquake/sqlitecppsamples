#include"Test.h"
#include"Employee.h"
#include"Database.h"
#include<iostream>
#include<chrono>
void log(const std::string& msg){
	std::cout
		<<std::chrono::duration_cast
		<std::chrono::milliseconds>(
			std::chrono::system_clock::now().time_since_epoch()
		).count()
		<<":log:"
		<<msg
		<<std::endl
	;

}
void test_employee(){
	{
		Employee a;
		a.display();
	}
	{
		Employee a("John","Doe");
		a.display();
	}
	{
		Employee a("John","Doe");
		a.hire();
		a.display();
		a.fire();
		a.display();
	}
	{
		Employee a("John","Doe");
		a.display();
		a.promote();
		a.display();
		a.promote(100);
		a.display();
		a.demote();
		a.display();
		a.demote(100);
		a.display();
	}
}
/*
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
		void setLastName(const std::string& lastName);
		void setEmployeeNumber(int employeeNumber);
		int getEmployeeNumber() const;
		void setSalary(int newSalary);
		int getSalary() const;
		bool isHired() const;
*/
void test_database(){
	log("test_database:start");
	{
		Records::Database db;
		std::cout<<"----------------------------------------"<<std::endl;
		try{
			db.addEmployee("John","Doe");
			db.getEmployee(Records::kFirstEmployeeNumber).display();
		}catch(std::exception e){
			std::cerr<<e.what()<<std::endl;
		}
		try{
			db.getEmployee("John","Doe").display();
		}catch(std::exception e){
			std::cerr<<e.what()<<std::endl;
		}
		try{
			db.getEmployee("_John","_Doe").display();
		}catch(std::exception e){
			std::cerr<<e.what()<<std::endl;
		}
		try{
			db.displayAll();
			db.addEmployee("Name0","Surname0");
			db.addEmployee("Name1","Surname1");
			db.addEmployee("Name2","Surname2");
			db.displayAll();
		}catch(std::exception e){
			std::cerr<<e.what()<<std::endl;
		}
		try{
			std::cout<<"db.displayCurrent():"<<std::endl;
			db.displayCurrent();
		}catch(std::exception e){
		}
	}
	{
		Records::Database db;
		try{
			db.displayCurrent();
		}catch(std::exception e){
			std::cerr<<e.what()<<std::endl;
		}
		try{
			db.addEmployee("Name0","Surname0");
			db.displayCurrent();
		}catch(std::exception e){
			std::cerr<<e.what()<<std::endl;
		}
		try{
			db.addEmployee("Name1","Surname1");
			db.displayCurrent();
		}catch(std::exception e){
			std::cerr<<e.what()<<std::endl;
		}
		try{
			db.displayFormer();
		}catch(std::exception e){
			std::cerr<<e.what()<<std::endl;
		}
		try{
			db.addEmployee("Name2","Surname2");
			db.getEmployee("Name2","Surname2").fire();
			db.displayFormer();
		}catch(std::exception e){
			std::cerr<<e.what()<<std::endl;
		}


	}
	log("test_database:end");
}
