/////////////////////////////////////////////////////////////////////
// Store.cpp - Implementation of store class                       //
//                                                                 //
// Application: CSE687 - Object Oriented Design-Cataloguer         //
// Platform:    Dell Inspiron, Win 8.1, Visual Studio 2013         //
// Author:      Venkata Subba Rao Cheedella, Syracuse Univesity    //
//              vcheedel@syr.edu                                   //
/////////////////////////////////////////////////////////////////////



#include "Store.h"
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <list>

//--------------------<Default Constructor>----------------------

Store::Store()
{
}

//-------------------<Default destructor>--------------------------

Store::~Store()
{
}

//--------------------<Save list of paths>---------------------------

void Store::savePath(std::string dir)
{
	listOfDirs.insert(dir);
}

//---------------------<Save filenames with their locations>---------------------

void Store::saveFile(key k, value v)
{
	catalogue[k] = v;
}

//--------------------<Get the begin iterator of the list of files container>----------------

diriter Store::getDirsBegin()
{
	return listOfDirs.begin();
}

//---------------------<Get the end iterator of the list of files container>-------------

diriter Store::getDirsEnd()
{
	return listOfDirs.end();
}

//-------------------<Get the catalgouer being iterator>----------------

fileiter Store::getFileBegin()
{
	return catalogue.begin();
}

//--------------------<Get the cataloguer end iterator>-----------------

fileiter Store::getFileEnd()
{
	return catalogue.end();
}

//---------------------<return the length of directories for a search>----------------

unsigned int Store::lenOfDirs()
{
	return listOfDirs.size();
}

//---------------------<get list of files for a given search>-------------------

unsigned int Store::filesSize()
{
	return catalogue.size();
}

//----------------<Test Stub>----------------------------

#ifdef TEST_STORE
int main()
{
	Store s;	
	std::set<std::string> vstring;
	vstring.insert(".");
	vstring.insert("e:/sample");
	int count = 0;	
	value v1;
	auto viter = vstring.begin();
	v1.push_back(viter);
	s.saveFile("venk1.txt", v1);
	value v2;
	v1.push_back(viter++);
	s.saveFile("venkat1", v2);

	std::cout << "\n" <<"Filenames : ";
	for (auto miter = s.getFileBegin(); miter != s.getFileEnd(); miter++)
	{
		std::cout << "\n" <<miter->first;
	}
	std::cout << "\n" << "Paths : ";
	for (auto siter = vstring.begin(); siter != vstring.end(); siter++)
	{
		std::cout << "\n" << *siter;
	}
}
#endif
