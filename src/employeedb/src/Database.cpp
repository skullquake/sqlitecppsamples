#include"Database.h"
#include<iostream>
namespace Records{
	Database::Database(){
		this->initdb();
		this->loaddb();
		this->initnextemployeenumberdb();
	}
	Database::~Database(){
	}
	Employee& Database::addEmployee(
		const std::string& firstName,
		const std::string& lastName
	){
		Employee a(firstName,lastName);
		a.setEmployeeNumber(mNextEmployeeNumber);
		a.hire();
		mEmployees.push_back(a);
		if(this->db!=nullptr){
			SQLite::Transaction txn(*db);
			SQLite::Statement stmt(
				*db,
				R"(
					INSERT INTO EMPLOYEE 
					(
						id,
						fname,
						lname,
						salary,
						hired
					)
					VALUES
					(
						?,
						?,
						?,
						?,
						?
					)
				)"
			);
			stmt.bind(
				1,
				mNextEmployeeNumber
			);
			stmt.bind(
				2,
				firstName
			);
			stmt.bind(
				3,
				lastName
			);
			stmt.bind(
				4,
				a.getSalary()
			);
			stmt.bind(
				5,
				a.isHired()
			);
			stmt.exec();
			txn.commit();
		}else{
		}
		mNextEmployeeNumber++;
		return mEmployees[mEmployees.size()-1];
	}
	Employee& Database::getEmployee(
		int employeeNumber
	){
		for(auto& a:mEmployees){
			if(a.getEmployeeNumber()==employeeNumber){
				return a;
			}
		}
		throw std::logic_error("No Employee found");
	}
	Employee& Database::getEmployee(
		const std::string& firstName,
		const std::string& lastName
	){
		for(auto& a:mEmployees){
			if(
				a.getFirstName().compare(firstName)==0&&
				a.getLastName().compare(lastName)==0
			){
				return a;
			}
		}
		throw std::logic_error("No Employee found");
	}
	void Database::displayAll() const{
		for(auto& a:mEmployees){
			a.display();
		}
	}
	void Database::displayCurrent() const{
		if(mEmployees.size()>0){
			for(auto& a:mEmployees){
				if(a.isHired()){
					a.display();
				}else{
				}
			}
		}else{
			throw std::logic_error("No Employees in Database");
		}
	}
	void Database::displayFormer() const{
		if(mEmployees.size()>0){
			for(auto& a:mEmployees){
				if(!a.isHired()){
					a.display();
				}else{
				}
			}
		}else{
			throw std::logic_error("No Employees in Database");
		}
	}
	void Database::initdb(){
		db=new SQLite::Database("./db/a.db",SQLite::OPEN_READWRITE|SQLite::OPEN_CREATE);
		if(db!=NULL){
			{
				db->exec(R"(VACUUM)");
			}
			{
				SQLite::Transaction txn(*db);
				db->exec(R"(
					CREATE TABLE IF NOT EXISTS EMPLOYEE
					(
						id		INTEGER PRIMARY KEY AUTOINCREMENT,
						fname           TEXT,
						lname           TEXT,
						salary		INTEGER,
						hired		BOOL
					))");
				txn.commit();
			}

		}else{
		}
	}
	void Database::loaddb(){
		std::cout<<"loaddb()"<<std::endl;
		if(this->db!=nullptr){
			std::cout<<"0"<<std::endl;
			SQLite::Statement q(
				*(this->db),
				R"(
					SELECT
						id,
						fname,
						lname,
						salary,
						hired
					FROM
						EMPLOYEE
				)"
				
			);
			while(q.executeStep()){
				Employee a(
					q.getColumn(1).getString(),
					q.getColumn(2).getString()
				);
				a.setEmployeeNumber(q.getColumn(0).getInt());
				a.setSalary(q.getColumn(3).getInt());
				q.getColumn(4).getInt()==0?a.fire():a.hire();
				this->mEmployees.push_back(a);
			}
		}else{
			std::cout<<"1"<<std::endl;
		}
	}
	void Database::initnextemployeenumberdb(){
		this->mNextEmployeeNumber=kFirstEmployeeNumber;
		if(this->db!=nullptr){
			SQLite::Statement q(
				*(this->db),
				R"(
					SELECT
						id
					FROM
						EMPLOYEE
					ORDER BY
						id
					DESC
					LIMIT
						1
				)"
				
			);
			while(q.executeStep()){
				std::cout<<q.getColumn(0).getInt()<<std::endl;
				this->mNextEmployeeNumber=q.getColumn(0).getInt()+1;
			}
		}else{
			std::cout<<"1"<<std::endl;
		}
	}
	void Database::updateEmployee(const Employee& e){
		if(this->db!=nullptr){
			SQLite::Transaction txn(*db);
			SQLite::Statement stmt(
				*db,
				R"(
					UPDATE EMPLOYEE SET
					fname=?,
					lname=?,
					salary=?,
					hired=?
					WHERE id=?
				)"
			);
			stmt.bind(
				1,
				e.getFirstName()
			);
			stmt.bind(
				2,
				e.getLastName()
			);
			stmt.bind(
				3,
				e.getSalary()
			);
			stmt.bind(
				4,
				e.isHired()
			);
			stmt.bind(
				5,
				e.getEmployeeNumber()
			);
			stmt.exec();
			txn.commit();
		}else{
		}
	}
	void Database::dropEmployees(){
		this->mEmployees.clear();
		if(this->db!=nullptr){
			SQLite::Transaction txn(*db);
			SQLite::Statement stmt(
				*db,
				R"(
					DELETE FROM EMPLOYEE
				)"
			);
			stmt.exec();
			txn.commit();
		}else{
		}
		this->mNextEmployeeNumber=kFirstEmployeeNumber;
	}
}
