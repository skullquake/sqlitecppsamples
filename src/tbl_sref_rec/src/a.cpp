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
void gendata_rec(int depth,int width,int maxlvl,int& id){
	//log("gendata["+std::to_string(depth)+"]:start");
	int parent=id-1;
	if(db!=NULL){
		if(depth<=maxlvl){
			for(int i=0;i<width;i++){
				SQLite::Statement stmt(
					*db,
					R"(INSERT INTO A(id,val,parent)VALUES(?,?,?))"
				);
				stmt.bind(
					1,
					id
				);
				stmt.bind(
					2,
					"userdata"
				);
				if(depth==0){
				}else{
					stmt.bind(
						3,
						parent//id-1
					);
				}
				stmt.exec();
				//log(stmt.getQuery());
				//for(int j=0;j<depth;j++)std::cout<<" ";
				//std::cout<<stmt.getExpandedSQL();
				//std::cout<<std::endl;
				id+=1;
				gendata_rec(depth+1,width,maxlvl,id);
			}
		}else{
		}
	}else{
		log("db null");
	}
	//log("gendata["+std::to_string(depth)+"]:end");
}
void selectdata_rec(int idx=0,int lvl=0){
	if(db!=NULL){
		SQLite::Statement* q=NULL;
		if(idx==0){
			q=new SQLite::Statement(
				*db,
				R"(
					SELECT 
					id,val,parent
					FROM A
					WHERE
					parent IS NULL
				)"
				
			);
		}else{
			q=new SQLite::Statement(
				*db,
				R"(
					SELECT 
					id,val,parent
					FROM A
					WHERE
					parent=?
					AND
					parent IS NOT NULL
				)"
				
			);
			q->bind(
				1,
				idx
			);
		}
		while(q->executeStep()){
			for(int i=0;i<lvl;i++)std::cout<<" ";
			std::cout<<q->getColumn(0).getInt()<<",";
			std::cout<<q->getColumn(1).getString()<<",";
			std::cout<<q->getColumn(2).getInt()<<std::endl;
			selectdata_rec(q->getColumn(0),lvl+1);
		}
		delete q;
	}else{
		log("database null");
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
		int id=1;
		SQLite::Transaction txn(*db);
		gendata_rec(0,2,8,id);
		txn.commit();
		log("Populating:end");
		log("SelectingRec:start");
		selectdata_rec();
		log("SelectingRec:end");
		log("Closing database:start");
		closedb();
		log("Closing database:end");
	}catch(std::exception e){
		std::cerr<<e.what()<<std::endl;
	}
	return 0;
}
