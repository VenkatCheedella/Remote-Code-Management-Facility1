#ifndef TESTCATALOGUE_H
#define TESTCATALOGUE_H

/////////////////////////////////////////////////////////////////////
// TestCatalogue.h - Controller class                              //
//                                                                 //
// Application: CSE687 - Object Oriented Design-Cataloguer         //
// Platform:    Dell Inspiron, Win 8.1, Visual Studio 2013         //
// Author:      Venkata Subba Rao Cheedella, Syracuse Univesity    //
//              vcheedel@syr.edu                                   //
/////////////////////////////////////////////////////////////////////

/*

Test Catalogue class drives the application-Cataloguer. It creates Map with
the help of directories retrieved from the file manager. It decodes the user 
input and drives the flow with right logical conclusion. It can notify the display
package to print the catalogue. After the catalogue is created, it send a request for
Text search to search the text in the file set.

Public Interfaces
==================

void createCatalogue(Store& store);		-->It creates a set and a map for a store 
send to it.
void controller();						-->File Cataloger is controlled here, it makes
calls to otehr functions. Ex: creatCataloguer, TextSearch functions to search a text in
a given file set.
std::string getText(std::string arg);	--> It reads the command line arguemnts and 
pass the text string to search.
std::vector<std::string> getPatterns(std::string patts); --> It reads the command line arguement
and pass the list of patterns to search on.
void recSearch(TextSearch& txtSearch); --> Here it provides an option for client to search
differnt text agaist a set of patterns for a given map.

Required Files
======================
Display.h, Display.cpp. FileMgr.h, FileMgr.cpp, Store.h, Store.cpp, FileSystem.h,
FileSystem.cpp, TextSearch.h and TextSearch.cpp

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
#include <string>
#include "../Store/Store.h"
#include "../FileMgr/FileMgr.h"
#include "../TextSearch/TextSearch.h"




class TestCatalogue
{
public:
	TestCatalogue(std::vector<std::string> cmdargs);
	~TestCatalogue();	
	void createCatalogue(Store& store);
	void controller();
	std::string getText(std::string arg);
	std::vector<std::string> getPatterns(std::string patts);	
	void recSearch(TextSearch& txtSearch);
	Store& getStore(){ return store; }
	std::vector<std::map<key, value>::iterator> getTextSearchFiles(){ return textSearchFilenames;}
		
private:
	std::string path;
	std::string pattern;
	std::string text;
	std::vector<std::string> arguments;
	std::vector<std::string> patterns;
	std::vector<std::string> temppattern;
	void createMap(Store& m, FileMgr f);
	std::vector<std::map<key, value>::iterator> textSearchFilenames;
	Store store;
};

#endif
