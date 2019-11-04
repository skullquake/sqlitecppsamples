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
					id	INTEGER PRIMARY KEY NOT NULL,
					val	TEXT
				))");
			txn.commit();
		}
		{
			SQLite::Transaction txn(db);
			for(int i=0;i<8;i++){
				SQLite::Statement stmt(
					db,
					R"(
						INSERT INTO A
						(
							id,
							val
						)
						VALUES
						(
							?,
							?
						)
					)"
				);
				stmt.bind(1,i);//try stmt.bind(1,0); to see it fail
				stmt.bind(2,std::string("a")+std::to_string(i));
				stmt.exec();
			}
			txn.commit();
		}
		{
			SQLite::Statement q(
				db,
				R"(
					SELECT 
					*
					FROM A
				)"
				
			);
			while(q.executeStep()){
				std::cout<<"(";
				for(int colidx=0;colidx<q.getColumnCount();colidx++){
					 std::cout<<q.getColumn(colidx)<<",";
				}
				std::cout<<")"<<std::endl;
			}
		}
	}catch(std::exception e){
		std::cerr<<e.what()<<std::endl;
	}
	return 0;
}
