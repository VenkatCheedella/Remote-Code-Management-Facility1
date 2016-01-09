/////////////////////////////////////////////////////////////////////
// TestCatalogue.cpp - Implementation of Controller class          //
//                                                                 //
// Application: CSE687 - Object Oriented Design-Cataloguer         //
// Platform:    Dell Inspiron, Win 8.1, Visual Studio 2013         //
// Author:      Venkata Subba Rao Cheedella, Syracuse Univesity    //
//              vcheedel@syr.edu                                   //
/////////////////////////////////////////////////////////////////////

#include "TestCatalogue.h"
#include <iostream>
#include "../Store/Store.h"
#include "../FileMgr/FileMgr.h"
#include "../FileSystemDemo/FileSystem.h"
#include "../TextSearch/TextSearch.h"


using value = std::list<std::set<std::string>::iterator>;

//-------------------------<Promotion constructor>----------------------

TestCatalogue::TestCatalogue(std::vector<std::string> cmdargs) : arguments(cmdargs)
{

}

//---------------------<Deafult destructor>------------------

TestCatalogue::~TestCatalogue()
{
}

//--------------< Decode the /f<text> option of cmd line >--------------

std::string TestCatalogue::getText(std::string arg)
{
	std::string fsearch;
	fsearch = arg.substr(0, 2);
	if (fsearch == "/f")
	{
		text = arg.substr(2);
	}
	return text;
}

//--------------------< Decode the patterns of a file search >----------------------------------

std::vector<std::string> TestCatalogue::getPatterns(std::string arg)
{	
		patterns.clear();
		std::string pat = arg;
		if (arg == "")
			patterns.push_back("*.*");
		while (pat.find_first_of(" ") != std::string::npos)
		{
			int pos = pat.find_first_of(" ");
			patterns.push_back(pat.substr(0, pos));
			pat = pat.substr(pos + 1);
		}
		if (arg != "")
		patterns.push_back(pat);
		return patterns;					
}

//------------------< Provide an option to search after the catalogue is created >---------------

void TestCatalogue::recSearch(TextSearch& txtSearch)
{
	std::string arg;
	std::vector<std::string> args;
	while (true)
	{
		std::cout << "\n\n" << "Enter the text to search : " << "\t";
		std::string searchText;
		std::getline(std::cin, searchText);
		if (searchText == "")	// Check whether user has entered text or to exit
			break;
		else
		{		
			std::cout << "\n" << "Enter the pattern to search :" << "\t";
			std::getline(std::cin, arg);
			std::vector<std::map<key, value>::iterator> recmapiters;
			txtSearch.getFiles(getPatterns(arg), recmapiters);  // A call to get files of a given pattern
			if (searchText != "")										
					txtSearch.searchText(recmapiters, searchText);	// A call to search text against a set of files							
		}
	}	
}

//--------------------< Controls the data flow and process of the cataloguer >-----------------------------

void TestCatalogue::controller()
{		
		//Store store;
		createCatalogue(store);		
		TextSearch txtSearch(store);		
		std::vector<std::map<key, value>::iterator> miterators;
		for (auto miter = store.getFileBegin(); miter != store.getFileEnd(); miter++)
			miterators.push_back(miter);	// Retrieve list of iterators of Map	

		if (arguments[4] != "")
		{
			std::string temptext = getText(arguments[4]);			
			if (temptext != "")
				textSearchFilenames = txtSearch.searchText(miterators, temptext);  // A call to display list of files that has a given text.
			else
				std::cout << "\n\n" << "Text is not provided";
		}					
}

//------------------------< Creation of cataloguer >---------------------------

void TestCatalogue::createCatalogue(Store& store)
{	
		FileMgr fm;
		for (auto argsiter = arguments.begin(); argsiter != arguments.end(); argsiter++)
		{
			if (*argsiter == "/s")
				fm.setRecurrsive("/s");
		}
		std::vector<std::string> patts = getPatterns(arguments[1]);
		fm.setPatterns(patts);	
		if (arguments[0] == "")
			arguments[0] == ".";
		fm.getDirectoriesList(arguments[0], store);		
		createMap(store, fm);				
}


//-------------------< Creation of Map >-----------------------------------

void TestCatalogue::createMap(Store& str, FileMgr fmgr)
{	
		for (auto diter = str.getDirsBegin(); diter != str.getDirsEnd(); diter++)
		{
			std::vector<std::string> tempfile;
			fmgr.filesearch(tempfile, *diter);
			for (unsigned int i = 0; i < tempfile.size(); i++)
			{
				int keyexist = 0;
				for (auto mapiter = str.getFileBegin(); mapiter != str.getFileEnd(); mapiter++)
				{
					if (mapiter->first == tempfile[i])
					{
						mapiter->second.push_back(diter); // If file(key) is present in the map 
						keyexist++;
					}
				}
				if (keyexist == 0)	// If file(key) is not present in the map
				{
					value dlist;
					dlist.push_back(diter);
					std::string tempstr = tempfile[i];
					str.saveFile(tempstr,dlist);
				}
			}
			tempfile.clear();
		}		
}

//--------------------<Test Stub>-----------------------

#ifdef TEST_TESTCATALOGUE

int main()
{	
	std::vector<std::string> sampleargs;
	sampleargs.push_back(".");
	sampleargs.push_back("*.*");
	sampleargs.push_back("/s");
	sampleargs.push_back("/d");
	sampleargs.push_back("/finclude");
	TestCatalogue testc(sampleargs);
	testc.controller();

}
#endif