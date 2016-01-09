#ifndef TEXTSEARCH_H
#define TEXTSEARCH_H

/////////////////////////////////////////////////////////////////////
// TextSearch.h - Controller class                              //
// version 1.0                                                                //
// Application: CSE687 - Object Oriented Design-Cataloguer         //
// Platform:    Dell Inspiron, Win 8.1, Visual Studio 2013         //
// Author:      Venkata Subba Rao Cheedella, Syracuse Univesity    //
//              vcheedel@syr.edu                                   //
/////////////////////////////////////////////////////////////////////


/*

Test Search Package retrieves the files from the Map for a given pattern
and send the list of files to display package.  Each file of a pattern is 
searched for provided text. The list of files where the text is present is 
sent to the display package.

Public Interfaces
====================

void searchText(std::vector<std::map<key,value>::iterator>& miters, std::string text); --> Search text for a given array
of map iterators. It retirves the absolute file name, search the files against the text.
std::vector<std::map<key, value>::iterator>& getFiles(std::vector<std::string> patt, 
std::vector<std::map<key, value>::iterator>& filenameiters); ---> For a given set of patterns, it retrives the file name
from an array of map iterators. It search each pattern against each file retrieved.

Required Files
======================
Display.h, Display.cpp. FileMgr.h, FileMgr.cpp, Store.h, Store.cpp, FileSystem.h,
FileSystem.cpp, TestCatalogue.h, TestCatalogue.cpp, TextSearch.h and TextSearch.cpp.

Build Process
==============

From the Visual Studio Developer's Command Prompt:
devenv Sockets-Server.sln /rebuild debug

*/

/*
	ver 1.0  : 11 feb 2015
	- First Release
*/



#include <iostream>
#include <fstream>
#include "../Store/Store.h"
#include <string>
#include <vector>
#include <map>

class TextSearch
{
public:
	TextSearch(Store& s);
	~TextSearch();	
	std::vector<std::map<key, value>::iterator> searchText(std::vector<std::map<key, value>::iterator>& miters, std::string text);
	std::vector<std::map<key, value>::iterator>& getFiles(std::vector<std::string> patt, std::vector<std::map<key, value>::iterator>& filenameiters);


private:
	Store& storage;
	std::vector<std::string> spatterns;
	std::string textstring;		

};

#endif

