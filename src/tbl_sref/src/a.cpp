#include<iostream>
#include<string>
#include<chrono>
#include<SQLiteCpp/SQLiteCpp.h>
#include<SQLiteCpp/VariadicBind.h>
//------------------------------------------------------------------------------
SQLite::Database* db;
//------------------------------------------------------------------------------
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
//------------------------------------------------------------------------------
void opendb(){
	db=new SQLite::Database("./db/a.db",SQLite::OPEN_READWRITE|SQLite::OPEN_CREATE);
}
void closedb(){
	if(db==NULL)
		delete db;
}
void initdb(){
	if(db!=NULL){
		{
			SQLite::Transaction txn(*db);
			db->exec(R"(DROP TABLE IF EXISTS A)");
			txn.commit();
		}
		{
			db->exec(R"(VACUUM)");
		}
		{
			SQLite::Transaction txn(*db);
			db->exec(R"(
				CREATE TABLE A
				(
					id		INTEGER PRIMARY KEY AUTOINCREMENT,
					val		TEXT,
					parent		INTEGER,
					FOREIGN KEY(parent) REFERENCES A(id)
				))");
			txn.commit();
		}

	}else{
	}
}
void gendata(){
	if(db!=NULL){
		SQLite::Transaction txn(*db);
		int na=8;
		for(int i=0;i<na;i++){
			{

				SQLite::Statement stmt(
					*db,
					R"(
						INSERT INTO A
						(
							id,
							val,
							parent
						)
						VALUES
						(
							NULL,
							?,
							NULL
						)
					)"
				);
				stmt.bind(
					1,
					i
				);
				stmt.exec();
				for(int j=0;j<na;j++){
					{

						SQLite::Statement stmt(
							*db,
							R"(
								INSERT INTO A
								(
									id,
									val,
									parent
								)
								VALUES
								(
									NULL,
									?,
									?
								)
							)"
						);
						stmt.bind(
							1,
							std::to_string(i)+":"+
							std::to_string(j)+":"
						);
						stmt.bind(
							2,
							i
						);
						stmt.exec();
					}
				}

			}
		}
		txn.commit();
	}else{
	}
}
void selectdata(){
	if(db!=NULL){
		SQLite::Statement q0(
			*db,
			R"(
				SELECT 
				id,val,parent
				FROM A
				WHERE
				parent IS NOT NULL
			)"
			
		);
		while(q0.executeStep()){
			std::cout<<q0.getColumn(0).getInt()<<",";
			std::cout<<q0.getColumn(1).getString()<<",";
			std::cout<<q0.getColumn(2).getInt()<<std::endl;
			{
				SQLite::Statement q1(
					*db,
					R"(
						SELECT 
						id,val,parent
						FROM A
						WHERE
						parent=?
					)"
					
				);
				q1.bind(
					1,
					q0.getColumn(1).getInt()
				);
				while(q1.executeStep()){
					std::cout<<"\t"<<q1.getColumn(0).getInt()<<",";
					std::cout<<q1.getColumn(1).getString()<<",";
					std::cout<<q1.getColumn(2).getInt()<<std::endl;
				}
				std::cout<<"----------------------------------------"<<std::endl;
			}

		}
	}else{
	}
}
//------------------------------------------------------------------------------
int main(int argc,char** argv){
	try{
		log("Opening database:start");
		opendb();
		log("Opening database:end");
		log("Creating tables:start");
		initdb();
		log("Creating tables:end");
		log("Populating:start");
		gendata();
		log("Populating:end");
		log("Selecting:start");
		selectdata();
		log("Selecting:end");
		log("Closgin database:start");
		closedb();
		log("Closgin database:end");
	}catch(std::exception e){
		std::cerr<<e.what()<<std::endl;
	}
	return 0;
}
