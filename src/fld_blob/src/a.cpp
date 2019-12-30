#include<iostream>
#include<fstream>
#include<string>
#include<SQLiteCpp/SQLiteCpp.h>
#include<SQLiteCpp/VariadicBind.h>
#include<chrono>
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
	log("db open:start");
	SQLite::Database db("./db/a.db",SQLite::OPEN_READWRITE|SQLite::OPEN_CREATE);
	log("db open:end");
	try{
		log("db create:start");
		{
			SQLite::Transaction txn(db);
			db.exec(R"(DROP TABLE IF EXISTS A)");
			txn.commit();
		}
		{
			db.exec(R"(
				VACUUM
				)");
		}
		{
			SQLite::Transaction txn(db);
			db.exec(R"(
				CREATE TABLE A
				(
					id		INTEGER PRIMARY KEY,
					name		TEXT,
					val		BLOB
				))");
			txn.commit();
		}
		log("db create:end");
		{
			log("db populate file:start");
			int nfiles=4096;
			int txnsz=512;
			SQLite::Transaction* txn=NULL;
			std::string fnam="./res/a.png";
			std::ifstream fl(fnam);
			fl.seekg(0,std::ios::end);
			size_t len=fl.tellg();
			char *blob=new char[len];
			int i=0;
			for(;i<nfiles;i++){
				if(i%txnsz==0){
					if(txn!=NULL){
						delete txn;
						txn=NULL;
					}
					txn=new SQLite::Transaction(db);
				}
				fl.seekg(0,std::ios::beg);
				fl.read(blob,len);
				fl.close();
				SQLite::Statement stmt(
					db,
					R"(
						INSERT INTO A
						(
							name,
							val
						)
						VALUES
						(
							?,
							?
						)
					)"
				);
				stmt.bind(1,"a.jpg");
				stmt.bind(2,blob,len);
				stmt.exec();
				if(i%txnsz==txnsz-1){
					if(txn!=NULL){
						txn->commit();
						delete txn;
						txn=NULL;
					}
				}
			}
			if(i%txnsz!=txnsz-1){
				if(txn!=NULL){
					txn->commit();
					delete txn;
				}
			}
			delete blob;
			log("db populate file:end");
		}
		{
			log("db populate generated:start");
			int ndata=4096;
			int txnsz=512;
			SQLite::Transaction* txn=NULL;
			int i=0;
			for(;i<ndata;i++){
				if(i%txnsz==0){
					if(txn!=NULL){
						delete txn;
						txn=NULL;
					}
					txn=new SQLite::Transaction(db);
				}
				size_t len=rand()%32+32;;
				char *blob=new char[len];
				for(int j=0;j<len;j++){
					blob[j]='a'+j;
				}
				SQLite::Statement stmt(
					db,
					R"(
						INSERT INTO A
						(
							name,
							val
						)
						VALUES
						(
							?,
							?
						)
					)"
				);
				stmt.bind(1,"generated");
				stmt.bind(2,blob,len);
				stmt.exec();
				if(i%txnsz==txnsz-1){
					if(txn!=NULL){
						txn->commit();
						delete txn;
						txn=NULL;
					}
				}
				delete blob;
			}
			if(i%txnsz!=txnsz-1){
				if(txn!=NULL){
					txn->commit();
					delete txn;
				}
			}
			log("db populate generated:end");
		}
		{
			log("db select:start");
			SQLite::Statement q0(
				db,
				R"(
					SELECT 
					id,
					name,
					val
					FROM A
				)"
				
			);
			std::cout<<"id,name,size"<<std::endl;
			while(q0.executeStep()){
				const void* blob=NULL;
				size_t size;
				blob=q0.getColumn(2).getBlob();
				size=q0.getColumn(2).getBytes();
				std::cout
					<<q0.getColumn(0).getInt()<<","
				 	<<q0.getColumn(1).getString()<<","
				 	<<size
					<<std::endl
				;
			}
			log("db select:end");
		}
	}catch(std::exception e){
		std::cerr<<e.what()<<std::endl;
	}
	return 0;
}
