#include<iostream>
#include<iostream>
#include<string>
#include<chrono>
#include<vector>
#include<SQLiteCpp/SQLiteCpp.h>
#include<SQLiteCpp/VariadicBind.h>
#define DEBUG
//------------------------------------------------------------------------------
SQLite::Database* db;
//------------------------------------------------------------------------------
void log(const std::string& msg);
void opendb();
void closedb();
void initdb();
void gendata();
void dropdb();
void vacuumdb();
void selectdata();
void selectjoindata();
std::vector<int> selectqs();
int  main(int argc,char** argv);
//------------------------------------------------------------------------------
void log(const std::string& msg){
#ifdef DEBUG
	std::cout
		<<std::chrono::duration_cast
		<std::chrono::milliseconds>(
			std::chrono::system_clock::now().time_since_epoch()
		).count()
		<<":log:"
		<<msg
		<<std::endl
	;
#endif
}
//------------------------------------------------------------------------------
void initdb(){
	if(db!=NULL){
		{
			log("Creating QS");
			SQLite::Transaction txn(*db);
			db->exec(R"(
CREATE TABLE IF NOT EXISTS QS
(
	id		INTEGER PRIMARY KEY NOT NULL,
	name		TEXT
)
			)");
			log("Creating Q");
			db->exec(R"(
CREATE TABLE IF NOT EXISTS Q
(
	id		INTEGER PRIMARY KEY NOT NULL,
	val		TEXT,
	q_qs		INTEGER,
	FOREIGN KEY(q_qs)
	REFERENCES QS(id)
)
			)");
			log("Creating A");
			db->exec(R"(
CREATE TABLE IF NOT EXISTS A
(
	id		INTEGER PRIMARY KEY NOT NULL,
	val		TEXT,
	correct		BOOL,
	a_q		INTEGER,
	FOREIGN KEY(a_q)
	REFERENCES A(id)
)
			)");
			txn.commit();
		}

	}else{
	}
}
//------------------------------------------------------------------------------
void vacuumdb(){
	if(db!=NULL){
		{
			db->exec(R"(VACUUM)");
		}
	}
}
//------------------------------------------------------------------------------
void gendata(){
	if(db!=NULL){
		log("Generating data");
		SQLite::Transaction txn(*db);
		int nqs=8;
		int nq=32;
		int na=8;
		for(int i=0;i<nqs;i++){
			{
				SQLite::Statement stmt_qs(
					*db,
					R"(
						INSERT INTO QS
						(
							name
						)
						VALUES
						(
							?
						)
					)"
				);
				stmt_qs.bind(
					1,
					"QS"+std::to_string(i)
				);
				stmt_qs.exec();
				for(int j=0;j<nq;j++){
					{
						SQLite::Statement stmt_q(
							*db,
							R"(
								INSERT INTO Q
								(
									val,
									q_qs
								)
								VALUES
								(
									?,
									?
								)
							)"
						);
						stmt_q.bind(
							1,
							"Q"+std::to_string(j)
						);
						stmt_q.bind(
							2,
							i+1
						);
						stmt_q.exec();
					}
					int idx_true=rand()%na;
					for(int k=0;k<na;k++){
						{
							SQLite::Statement stmt_a(
								*db,
								R"(
									INSERT INTO A
									(
										val,
										correct,
										a_q
									)
									VALUES
									(
										?,
										?,
										?
									)
								)"
							);
							stmt_a.bind(
								1,
								"A"+std::to_string(k)
							);
							stmt_a.bind(
								2,
								k==idx_true//==false
							);
							stmt_a.bind(
								3,
								i*nq+j+1
							);
							stmt_a.exec();
						}
					}
				}
			}
		}
		txn.commit();
	}else{
	}
}
//------------------------------------------------------------------------------
void selectdata(){
	if(db!=NULL){
		log("Retrieving data");
		SQLite::Statement q(
			*db,
			R"(
				SELECT 
				id,name
				FROM QS
			)"
			
		);
		while(q.executeStep()){
			std::cout<<q.getColumn(0).getInt()<<",";
			std::cout<<q.getColumn(1).getString()<<std::endl;
		}
	}else{
	}
}
void print_qs(){
	if(db!=NULL){
		log("Retrieving data");
		SQLite::Statement q(
			*db,
			R"(
				SELECT 
				id,name
				FROM QS
			)"
			
		);
		while(q.executeStep()){
			std::cout<<q.getColumn(0).getInt()<<":";
			std::cout<<q.getColumn(1).getString()<<std::endl;
		}
	}else{
	}
}
void selectjoindata(){
	if(db!=NULL){
		log("Retrieving data");
		SQLite::Statement q(
			*db,
			R"(
SELECT
	qs.id,
	qs.name,
	q.id,
	q.val,
	a.id,
	a.val,
	a.correct
FROM
	qs 
INNER JOIN
	q
INNER JOIN
	a
ON
	qs.id=q.q_qs
AND
	q.id=a.a_q
			)"
			
		);
		int i=0;
		while(q.executeStep()){
			std::cout<<(i++)<<",";
			std::cout<<q.getColumn(0)<<",";
			std::cout<<q.getColumn(1)<<",";
			std::cout<<q.getColumn(2)<<",";
			std::cout<<q.getColumn(3)<<",";
			std::cout<<q.getColumn(4)<<",";
			std::cout<<q.getColumn(5)<<",";
			std::cout<<q.getColumn(6)<<std::endl;
		}
	}else{
	}
}
std::vector<int> selectqs(){
	log("selectqs:start");
	std::vector<int> vr;
	if(db!=NULL){
		log("Retrieving data");
		SQLite::Statement q(
			*db,
			R"(
SELECT
	qs.id
FROM
	qs 
			)"
			
		);
		while(q.executeStep()){
			vr.push_back(q.getColumn(0).getInt());
		}
	}else{
	}
	log("selectqs:end");
	return vr;
}
int selectnumq(int idx_qs){
	log("selectnumq:start");
	int ret=-1;
	if(db!=NULL){
		log("Retrieving data");
		SQLite::Statement q(
			*db,
			R"(
SELECT
	count(*)
FROM
	qs 
INNER JOIN
	q
ON
	qs.id=q.q_qs
WHERE
	qs.id=?
			)"
		);
		q.bind(
			1,
			idx_qs
		);
		while(q.executeStep()){
			ret=q.getColumn(0);
		}
	}else{
	}
	log("selectnumq:end");
	return ret;
}

//------------------------------------------------------------------------------
void opendb(){
	db=new SQLite::Database("./db/a.db",SQLite::OPEN_READWRITE|SQLite::OPEN_CREATE);
}
//------------------------------------------------------------------------------
void closedb(){
	if(db==NULL)
		delete db;
}
//------------------------------------------------------------------------------
void dropdb(){
	if(db!=NULL){
		{
			SQLite::Transaction txn(*db);
			log("Dropping QS");
			db->exec(R"(DROP TABLE IF EXISTS QS)");
			log("Dropping Q");
			db->exec(R"(DROP TABLE IF EXISTS Q)");
			log("Dropping A");
			db->exec(R"(DROP TABLE IF EXISTS A)");
			txn.commit();
		}
	}
}
//------------------------------------------------------------------------------
int  main(int argc,char** argv){
	bool done=false;
	int idx_qs=-1;
	int idx_q=-1;
	int idx_a=-1;
	int num_q=-1;
	std::string input;
	opendb();
	initdb();
	while(!done){
		if(idx_qs==-1){
			print_qs();
			std::cout<<"Select Question Set"<<std::endl;
		}else{
		}
		std::cin>>input;
		if(input=="quit"||input=="q"){
			done=true;
		}else{
			try{
				int option;
				option=std::stoi(input);
				if(idx_qs==-1){
					bool idx_qs_found=false;
					idx_qs=option;
					auto vqsid=selectqs();
					for(auto qsid:vqsid){
						if(idx_qs==qsid){
							idx_qs_found=true;
							break;
						}
					}
					if(idx_qs_found){
						num_q=selectnumq(idx_qs);
						std::cout<<"Question set "<<idx_qs<<" selected ["<<numq<<" questions]"<<std::endl;
					}else{
						std::cout<<"Invalid Selection"<<std::endl;
						idx_qs=-1;
					}
				}else{
					switch(option){
						case 0:
							std::cout<<"Option 0"<<std::endl;
							break;
						case 1:
							std::cout<<"Option 1"<<std::endl;
							break;
						case 2:
							std::cout<<"Option 2"<<std::endl;
							break;
						case 3:
							std::cout<<"Option 3"<<std::endl;
							break;
						default:
							break;
					}

				}
			}catch(std::exception e){
				std::cerr<<"Invalid input"<<std::endl;
			}
		}
	}
	dropdb();
	initdb();
	gendata();
	selectdata();
	selectjoindata();
	closedb();
	return 0;
}
