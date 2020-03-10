#include<iostream>
#include"Test.h"
#include"Database.h"
const unsigned int opt_doQuit       =0;
const unsigned int opt_doHire       =1;
const unsigned int opt_doFire       =2;
const unsigned int opt_doPromote    =3;
const unsigned int opt_doDemote     =4;
const unsigned int opt_doListAll    =5;
const unsigned int opt_doListCurrent=6;
const unsigned int opt_doListFormer =7;
const unsigned int opt_doDrop       =8;
int  main(int,char**);
int  displayMenu();
void doHire(Records::Database&);
void doFire(Records::Database&);
void doPromote(Records::Database&);
void doDemote(Records::Database&);
void doListAll(Records::Database&);
void doListCurrent(Records::Database&);
void doListFormer(Records::Database&);
void doDrop(Records::Database&);
int  main(int argc,char** argv){
	Records::Database db;
	bool done=false;
	std::cout<<"----------------------------------------"<<std::endl;
	while(!done){
		int s=displayMenu();
		switch(s){
			case opt_doHire:
				doHire(db);
				break;
			case opt_doFire:
				doFire(db);
				break;
			case opt_doPromote:
				doPromote(db);
				break;
			case opt_doDemote:
				doDemote(db);
				break;
			case opt_doListAll:
				doListAll(db);
				break;
			case opt_doListCurrent:
				doListCurrent(db);
				break;
			case opt_doListFormer:
				doListFormer(db);
				break;
			case opt_doDrop:
				doDrop(db);
				break;
			case opt_doQuit:
				done=true;
				break;
			default:
				std::cerr<<"Invalid selection"<<std::endl;
				break;
		}
	}
	return 0;
}
int  displayMenu(){
	std::cout
		<<"Menu:"<<std::endl
		<<"----------------------------------------"<<std::endl
		<<"1) Hire"<<std::endl
		<<"2) Fire"<<std::endl
		<<"3) Promote"<<std::endl
		<<"4) Demote"<<std::endl
		<<"5) List All"<<std::endl
		<<"6) List Current"<<std::endl
		<<"7) List Former"<<std::endl
		<<"8) Drop Employees"<<std::endl
		<<"0) Quit"<<std::endl
		<<"----------------------------------------"<<std::endl
	;
	int s;
	std::cin>>s;
	return s;
}
void doHire(Records::Database& db){
	std::string a;
	std::string b;
	std::cout<<"Name:    ";
	std::cin>>a;
	std::cout<<"Surname: ";
	std::cin>>b;
	db.addEmployee(a,b);

}
void doFire(Records::Database& db){
	try{
		int a;
		std::cout<<"Employee Number: ";
		std::cin>>a;
		db.getEmployee(a).fire();
		db.updateEmployee(db.getEmployee(a));
	}catch(std::exception e){
		std::cerr<<e.what()<<std::endl;
	}
	std::cout<<"----------------------------------------"<<std::endl;
}
void doPromote(Records::Database& db){
	try{
		int a;
		std::cout<<"Employee Number: ";
		std::cin>>a;
		int b;
		std::cout<<"Amount:           ";
		std::cin>>b;
		db.getEmployee(a).promote(b);
		db.updateEmployee(db.getEmployee(a));
	}catch(std::exception e){
		std::cerr<<e.what()<<std::endl;
	}
	std::cout<<"----------------------------------------"<<std::endl;
}
void doDemote(Records::Database& db){
	try{
		int a;
		std::cout<<"Employee Number: ";
		std::cin>>a;
		int b;
		std::cout<<"Amount:           ";
		std::cin>>b;
		db.getEmployee(a).demote(b);
	}catch(std::exception e){
		std::cerr<<e.what()<<std::endl;
	}
	std::cout<<"----------------------------------------"<<std::endl;
}
void doListAll(Records::Database& db){
	db.displayAll();
}
void doDrop(Records::Database& db){
	db.dropEmployees();
}
void doListCurrent(Records::Database& db){
	db.displayCurrent();
}
void doListFormer(Records::Database& db){
	db.displayFormer();
}

