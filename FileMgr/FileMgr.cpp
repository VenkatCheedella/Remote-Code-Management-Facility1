/////////////////////////////////////////////////////////////////////
// FileMgr.cpp - Implementation of FileMgr class                     //
//                                                                 //
// Application: CSE687 - Object Oriented Design-Cataloguer         //
// Platform:    Dell Inspiron, Win 8.1, Visual Studio 2013         //
// Author:      Venkata Subba Rao Cheedella, Syracuse Univesity    //
//              vcheedel@syr.edu                                   //
/////////////////////////////////////////////////////////////////////


#include "FileMgr.h"
#include "../FileSystemDemo/FileSystem.h"

//-------------------<Promotion constructor>------------------------

FileMgr::FileMgr() :isRecursive(false)
{
	
}

//-------------------<Promotion deconstructor>---------------------

FileMgr::~FileMgr()
{
}


//----------------< Get directory information of a given path >-----------------

Store& FileMgr::getDirectoriesList(std::string Path, Store& dirs)
{
		FileSystem::Directory::setCurrentDirectory(Path);
		std::string path = FileSystem::Directory::getCurrentDirectory();
		dirs.savePath(path);
		std::vector<std::string> onlydirs;
		if (isRecursive == true)
			dirs = getAllDirectories(path, dirs);	
		return dirs;	
}


//----------------------< Recursive search >---------------------------------

Store& FileMgr::getAllDirectories(std::string Path, Store& dirs)
{	
		std::vector<std::string> onlydirs;
		unsigned int lengthofdirs = dirs.lenOfDirs();
		onlydirs = FileSystem::Directory::getDirectories(Path);
		if (onlydirs.size() > 0)
		onlydirs.erase(onlydirs.begin(), onlydirs.begin() + 2);
		for (auto viter = onlydirs.begin(); viter != onlydirs.end(); viter++)
		{
			FileSystem::Directory::setCurrentDirectory(Path);
			dirs.savePath(FileSystem::Path::fileSpec(FileSystem::Directory::getCurrentDirectory(), *viter));
		}
		if (dirs.lenOfDirs() > lengthofdirs) // Checks if there are directories in the present directory.
		{
			unsigned  count = 0;
			for (auto siter = dirs.getDirsBegin(); siter != dirs.getDirsEnd(); siter++)
			{
				count++;
				if (count >= lengthofdirs)
					getAllDirectories(*siter, dirs);
			}

		}

		return dirs;	
}

//--------------------<Set Patterns>---------------------------

void FileMgr::setPatterns(std::vector<std::string> patt)
{
	patterns = patt;	
}



 //--------------< Check if user has provided recursive >--------------------

void FileMgr::setRecurrsive(std::string rec)
{
	if (rec == "/s")
		isRecursive = true;
	else
		isRecursive = false;
}

//----------------------< File Search in a directory >----------------------------

void FileMgr::filesearch(std::vector<std::string>& tempfile, std::string path)
{
	for (unsigned int i = 0; i < patterns.size(); i++)
	{
		std::vector<std::string> patternfiles;
		patternfiles = FileSystem::Directory::getFiles(path, patterns[i]);
		for (unsigned int i = 0; i < patternfiles.size(); i++)
			tempfile.push_back(patternfiles[i]);
	}			
}

//------------------------<Test Stub>--------------------------

#ifdef TEST_FILEMGR
int main()
{
	FileMgr ex;
	ex.setRecurrsive("/s");	
	std::vector<std::string> paths;
	paths.push_back(".");
	Store S;
	ex.getDirectoriesList("e:/sample", S);
	for (auto diriter = S.getDirsBegin(); diriter != S.getDirsEnd(); diriter++)
		std::cout << "\n" << *diriter<< "\n";
}
#endif