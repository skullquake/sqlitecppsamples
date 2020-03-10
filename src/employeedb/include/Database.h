#ifndef DATABASE_H
#define DATABASE_H
#include"Employee.h"
#include<vector>
#include<SQLiteCpp/SQLiteCpp.h>
#include<SQLiteCpp/VariadicBind.h>
namespace Records{
	const int kFirstEmployeeNumber=0;
	class Database{
		public:
			Database();
			~Database();
			Employee& addEmployee(
				const std::string& firstName,
				const std::string& lastName
			);
			Employee& getEmployee(
				int employeeNumber
			);
			Employee& getEmployee(
				const std::string& firstName,
				const std::string& lastName
			);
			void displayAll() const;
			void displayCurrent() const;
			void displayFormer() const;
			void initdb();
			void loaddb();
			void initnextemployeenumberdb();
			void updateEmployee(const Employee&);
			void dropEmployees();
		private:
			std::vector<Employee> mEmployees;
			int mNextEmployeeNumber=kFirstEmployeeNumber;
			SQLite::Database* db=nullptr;
		protected:
	};
}
#endif
