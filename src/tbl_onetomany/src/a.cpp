#include<iostream>
#include<string>
#include<chrono>
#include<SQLiteCpp/SQLiteCpp.h>
#include<SQLiteCpp/VariadicBind.h>
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
int main(int argc,char** argv){
	log("Opening database:start");
	SQLite::Database db("./db/a.db",SQLite::OPEN_READWRITE|SQLite::OPEN_CREATE);
	log("Opening database:end");
	try{
		log("Creating tables:start");
		{
			SQLite::Transaction txn(db);
			db.exec(R"(DROP TABLE IF EXISTS A)");
			db.exec(R"(DROP TABLE IF EXISTS B)");
			txn.commit();
		}
		{
			db.exec(R"(VACUUM)");
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
		log("Creating tables:end");
		{
			log("Populating:start");
			SQLite::Transaction txn(db);
			int na=512;
			int nb=128;
			int aid=0;
			int bid=0;
			for(int i=0;i<na;i++){
				{

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
				}
				for(int j=0;j<nb;j++){
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
					bid++;
				}
				aid++;
			}
			txn.commit();
			log("Populating:end");
		}
		{
			log("Selecting:start");
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
			log("Selecting:end");
		}
		{
			log("Selecting JOIN:start");
			SQLite::Statement q0(
				db,
				R"(
					SELECT 
					A.id,
					A.val,
					B.id,
					B.val
					FROM A
					INNER JOIN
					B
					ON
					A.id=B.a_id
					ORDER BY
					A.id ASC
				)"
				
			);
			while(q0.executeStep()){
				std::cout<<"(";
				for(int colidx=0;colidx<q0.getColumnCount();colidx++){
					 std::cout<<q0.getColumn(colidx)<<",";
				}
				std::cout<<")"<<std::endl;
			}
			log("Selecting JOIN:end");
			std::cout<<"----------------------------------------"<<std::endl;
		}
		std::cout<<"done."<<std::endl;
	}catch(std::exception e){
		std::cerr<<e.what()<<std::endl;
	}
	return 0;
}
