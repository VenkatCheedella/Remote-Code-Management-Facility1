#ifndef STORE_H
#define STORE_H

/////////////////////////////////////////////////////////////////////
// Store.h - Storage class                                       //
//                                                                 //
// Application: CSE687 - Object Oriented Design-Cataloguer         //
// Platform:    Dell Inspiron, Win 8.1, Visual Studio 2013         //
// Author:      Venkata Subba Rao Cheedella, Syracuse Univesity    //
//              vcheedel@syr.edu                                   //
/////////////////////////////////////////////////////////////////////

/*
Here we define a containers, Map and Set to store the directories and files 
and the corresponding directory location iterators. 


Required Files
======================
Display.h, Display.cpp. FileMgr.h, FileMgr.cpp, Store.h, Store.cpp, FileSystem.h,
FileSystem.cpp, TestCatalogue.h, TestCatalogue.cpp, TextSearch.h and TextSearch.cpp.

Publiv Interfaces
=======================

void savePath(std::string dir);  --> To save directory in the set
void saveFile(key k, value v);  --->  To save a file and a list to a map
diriter getDirsBegin();         ---> To retrive first iterator of the set
diriter getDirsEnd();			---> To get the last iterator of the set
fileiter getFileBegin();		---> To get the first iterator of the map
fileiter getFileEnd();			---> To get the last iterator of the map
unsigned int lenOfDirs();		---> To get the length of set(directory list)
unsigned int filesSize();		---> To get the current lenght of map( files list)

Build Process
==============

From the Visual Studio Developer's Command Prompt:
devenv Sockets-Server.sln /rebuild debug


*/

/*

Maintenance history
=======================

	ver 1.0  : 11 feb 2015
	- First Release

*/


#include <iostream>
#include <set>
#include <map>
#include <list>
#include <set>

using key = std::string;
using diriter = std::set<std::string>::iterator;
using value = std::list<std::set<std::string>::iterator>;
using fileiter = std::map<key, value>::iterator;

class Store
{
public:
	Store();
	~Store();		
	void savePath(std::string dir);
	void saveFile(key k, value v);
	diriter getDirsBegin();
	diriter getDirsEnd();
	fileiter getFileBegin();
	fileiter getFileEnd();
	unsigned int lenOfDirs();
	unsigned int filesSize();

private:
	std::set<std::string> listOfDirs;
	std::map<key, value> catalogue;	
};


#endif
