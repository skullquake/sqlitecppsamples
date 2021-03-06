#include<iostream>
#include<string>
#include<SQLiteCpp/SQLiteCpp.h>
#include<SQLiteCpp/VariadicBind.h>
int main(int argc,char** argv){
	SQLite::Database db("./db/a.db",SQLite::OPEN_READWRITE|SQLite::OPEN_CREATE);
	try{
		{
			SQLite::Transaction txn(db);
			db.exec(R"(DROP TABLE IF EXISTS A)");
			txn.commit();
		}
		{
			SQLite::Transaction txn(db);
			db.exec(R"(
				CREATE TABLE A
				(
					val		TEXT
				))");
			txn.commit();
		}
		{
			SQLite::Transaction txn(db);
			SQLite::Statement stmt(
				db,
				R"(
					INSERT INTO A
					(
						val
					)
					VALUES
					(
						?
					)
				)"
			);
			stmt.bind(1,std::string("a")+std::to_string(0));
			stmt.exec();
			txn.commit();
		}
		{
			SQLite::Statement q0(
				db,
				R"(
					SELECT 
					*
					FROM A
				)"
				
			);
			while(q0.executeStep()){
				std::cout<<"(";
				for(int colidx=0;colidx<q0.getColumnCount();colidx++){
					 std::cout<<q0.getColumn(colidx)<<",";
				}
				std::cout<<")"<<std::endl;
			}
		}
	}catch(std::exception e){
		std::cerr<<e.what()<<std::endl;
	}
	return 0;
}
