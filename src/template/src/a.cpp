#include<iostream>
#include<SQLiteCpp/SQLiteCpp.h>
#include<SQLiteCpp/VariadicBind.h>
int main(int argc,char** argv){
	SQLite::Database db("./db/a.db",SQLite::OPEN_READWRITE|SQLite::OPEN_CREATE);
	std::cout<<"Opened "<<db.getFilename()<<std::endl;
	if(!db.exec("CREATE TABLE IF NOT EXISTS vec2f (x FLOAT, y FLOAT)")){
		std::cout<<"Table vec2f created"<<std::endl;
	}else{
		std::cerr<<"Failed to create table vec2f"<<std::endl;
	}
	if(!db.exec("DELETE FROM vec2f")){
		std::cout<<"Table vec2f cleared"<<std::endl;
	}else{
		std::cerr<<"Failed to clear table vec2f"<<std::endl;
	}
	SQLite::Transaction txn(db);
	for(int i=0;i<8;i++){
		SQLite::Statement stmt(db,"INSERT INTO vec2f (x,y) VALUES(?,?)");
		stmt.bind(1,i);
		stmt.bind(2,i);
		if(!stmt.exec()){
			std::cout<<"Row created"<<std::endl;
		}else{
			std::cout<<"Failed to create row"<<std::endl;
		}
	}
	txn.commit();
	SQLite::Statement q(db,"SELECT * FROM vec2f");
	while(q.executeStep()){
		std::cout<<"("
		         <<q.getColumn(0)
		         <<","
		         <<q.getColumn(1)
		         <<")"
		         <<std::endl;
	}
	return 0;
}
