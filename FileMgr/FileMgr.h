#ifndef FILEMGR_H
#define FILEMGR_H


/////////////////////////////////////////////////////////////////////
// FileMgr.h - To retrive Directory and files                      //
//                                                                 //
// Application: CSE687 - Object Oriented Design-Cataloguer         //
// Platform:    Dell Inspiron, Win 8.1, Visual Studio 2013         //
// Author:      Venkata Subba Rao Cheedella, Syracuse Univesity    //
//              vcheedel@syr.edu                                   //
/////////////////////////////////////////////////////////////////////


/*

Retrieves directories (recursive scan) for a given 
directory with the help of fileSystem. Retrives the files for a given
pattern in a directory.

Public Interfaces:
===================

Store& getDirectoriesList(std::string Path, Store& dirs); ---> Read the 
directory set and request FileSystem to get the list of directories in 
a given directory.

void setPatterns(std::vector<std::string> patt); --> Patterns to get the
files are set using this interface.

void setRecurrsive(std::string rec);  --> If recursive search is needed,
flag has to be set.

Store& getAllDirectories(std::string Path, Store& dirs) -> If the recursive 
search flag is set, it retrievs directories for all directories present in
the given directory.

void filesearch(std::vector<std::string>& tempfile, std::string path)--> For
a given set of patterns, filesearch is preformed in a given path.


Build Process
==============

From the Visual Studio Developer's Command Prompt:
devenv Sockets-Server.sln /rebuild debug

Required Files
======================
FileMgr.h, FileMgr.cpp, Store.h, Store.cpp, FileSystem.h,
FileSystem.cpp.

*/

/*

Maintenance history
===================

ver 1.0  : 11 feb 2015
- First Release


*/


#include <iostream>
#include <string>
#include <vector>
#include "../Store/Store.h"

using dirstore = std::vector<std::string>;


class FileMgr
{
public:
	FileMgr();
	~FileMgr();
	Store& getDirectoriesList(std::string Path, Store& dirs);
	void setPatterns(std::vector<std::string> patt);
	void setRecurrsive(std::string rec);
	void filesearch(std::vector<std::string>& tempfile, std::string path);

private:
	std::vector<std::string> patterns;
	bool isRecursive;
	Store& getAllDirectories(std::string Path, Store& dirs);
};


#endif
