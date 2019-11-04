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
			db.exec(R"(DROP TABLE IF EXISTS A_B)");
			db.exec(R"(DROP TABLE IF EXISTS B)");
			txn.commit();
		}
		{
			SQLite::Transaction txn(db);
			db.exec(R"(
				CREATE TABLE A
				(
					id		INTEGER NOT NULL PRIMARY KEY,
					val		TEXT
				))");
			txn.commit();
		}
		{
			SQLite::Transaction txn(db);
			db.exec(R"(
				CREATE TABLE B
				(
					id		INTEGER NOT NULL PRIMARY KEY,
					val		TEXT
				))");
			txn.commit();
		}
		{
			SQLite::Transaction txn(db);
			db.exec(R"(
				CREATE TABLE A_B
				(
					id		INTEGER NOT NULL PRIMARY KEY,
					a_id		INTEGER NOT NULL,
					b_id		INTEGER NOT NULL,
					FOREIGN KEY (a_id) REFERENCES A(id),
					FOREIGN KEY (b_id) REFERENCES B(id)
				
				))");
			txn.commit();
		}
		{
			SQLite::Transaction txn(db);
			int na=4;
			int nb=2;
			int aid=0;
			int bid=0;
			for(int i=0;i<na;i++){
				{
					std::cout<<"Creating A"<<std::to_string(i)<<"...";
					SQLite::Statement stmt0(
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
					stmt0.bind( 1,aid);
					stmt0.bind( 2,std::string("A")+std::to_string(aid));
					stmt0.exec();
					std::cout<<"done"<<std::endl;
					for(int j=0;j<nb;j++){
						std::cout<<"Creating B"<<std::to_string(i)<<"...";
						SQLite::Statement stmt1(
							db,
							R"(
								INSERT INTO B
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
						stmt1.bind( 1,bid);
						stmt1.bind( 2,std::string("A")+std::to_string(bid));
						stmt1.exec();
						std::cout<<"done"<<std::endl;

						std::cout<<"Creating A_B"<<std::to_string(i)<<"...";
						SQLite::Statement stmt2(
							db,
							R"(
								INSERT INTO A_B
								(
									a_id,
									b_id
								)
								VALUES
								(
									?,
									?
								)
							)"
						);
						stmt2.bind( 1,aid);
						stmt2.bind( 2,bid);
						stmt2.exec();
						std::cout<<"done"<<std::endl;
						bid++;
					}


					aid++;
				}
			}
			txn.commit();
		}
		{
			SQLite::Statement q0(
				db,
				R"(
					SELECT 
						*
					FROM
						A
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
						FROM
							A_B
						WHERE
							a_id=?
					)"
					
				);
				q1.bind( 1,q0.getColumn("id").getInt());
				while(q1.executeStep()){
					{
						SQLite::Statement q2(
							db,
							R"(
								SELECT 
								*
								FROM B
								WHERE
									id=?
							)"
							
						);
						q2.bind( 1,q1.getColumn("b_id").getInt());
						while(q2.executeStep()){
							std::cout<<"\t(";
							for(int colidx=0;colidx<q2.getColumnCount();colidx++){
								 std::cout<<q2.getColumn(colidx)<<",";
							}
							std::cout<<")"<<std::endl;
						}
					}
				}
			}
			std::cout<<"----------------------------------------"<<std::endl;
		}
		{
			SQLite::Statement q0(
				db,
				R"(
					SELECT 
						*
					FROM
						B
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
						FROM
							A_B
						WHERE
							b_id=?
					)"
					
				);
				q1.bind( 1,q0.getColumn("id").getInt());
				while(q1.executeStep()){
					{
						SQLite::Statement q2(
							db,
							R"(
								SELECT 
								*
								FROM A
								WHERE
									id=?
							)"
							
						);
						q2.bind( 1,q1.getColumn("a_id").getInt());
						while(q2.executeStep()){
							std::cout<<"\t(";
							for(int colidx=0;colidx<q2.getColumnCount();colidx++){
								 std::cout<<q2.getColumn(colidx)<<",";
							}
							std::cout<<")"<<std::endl;
						}
					}
				}
			}
			std::cout<<"----------------------------------------"<<std::endl;
		}
		std::cout<<"done."<<std::endl;
	}catch(std::exception e){
		std::cerr<<e.what()<<std::endl;
	}
	return 0;
}
