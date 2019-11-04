#include<iostream>
#include<string>
#include<SQLiteCpp/SQLiteCpp.h>
#include<SQLiteCpp/VariadicBind.h>
int main(int argc,char** argv){
	SQLite::Database db("./db/a.db",SQLite::OPEN_READWRITE|SQLite::OPEN_CREATE);
	std::cout<<"Opened "<<db.getFilename()<<std::endl;
	std::cout<<"Creating table...";
	try{
		{
			SQLite::Transaction txn(db);
			db.exec(R"(DROP TABLE IF EXISTS A)");
			db.exec(R"(DROP TABLE IF EXISTS B)");
			txn.commit();
		}
		{
			SQLite::Transaction txn(db);
			db.exec(R"(
				CREATE TABLE A
				(
					id		INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
					val		TEXT
				))");
			txn.commit();
		}
		{
			SQLite::Transaction txn(db);
			db.exec(R"(
				CREATE TABLE B
				(
					id		INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
					val		TEXT,
					a_id		INTEGER,
					FOREIGN KEY(a_id) REFERENCES A(id)
				))");
			txn.commit();
		}
		{
			SQLite::Transaction txn(db);
			int na=4;
			int nb=4;
			int aid=0;
			int bid=0;
			for(int i=0;i<na;i++){
				{
					std::cout<<"Creating A"<<std::to_string(i)<<"...";
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
					stmt.bind( 1,aid);
					stmt.bind( 2,std::string("A")+std::to_string(aid));
					stmt.exec();
					std::cout<<"done"<<std::endl;
				}
				for(int j=0;j<nb;j++){
					std::cout<<"Creating B"<<std::to_string(i)<<"...";
					SQLite::Statement stmt(
						db,
						R"(
							INSERT INTO B
							(
								id,
								val,
								a_id
							)
							VALUES
							(
								?,
								?,
								?
							)
						)"
					);
					stmt.bind( 1,bid);
					stmt.bind( 2,std::string("B")+std::to_string(bid));
					stmt.bind( 3,aid);
					stmt.exec();
					std::cout<<"done"<<std::endl;
					bid++;
				}
				aid++;
			}
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
				SQLite::Statement q1(
					db,
					R"(
						SELECT 
						*
						FROM B
						WHERE
						a_id=?
					)"
					
				);
				q1.bind(1,q0.getColumn("id").getInt());
				while(q1.executeStep()){
					std::cout<<"\t(";
					for(int colidx=0;colidx<q1.getColumnCount();colidx++){
						 std::cout<<q1.getColumn(colidx)<<",";
					}
					std::cout<<")"<<std::endl;
				}
				std::cout<<"----------------------------------------"<<std::endl;
			}
		}

		std::cout<<"done."<<std::endl;
	}catch(std::exception e){
		std::cerr<<e.what()<<std::endl;
	}
	return 0;
}
