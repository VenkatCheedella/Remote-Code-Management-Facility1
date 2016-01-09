/////////////////////////////////////////////////////////////////////
// TextSearch.cpp - Implementation of TextSearch class             //
//                                                                 //
// Application: CSE687 - Object Oriented Design-Cataloguer         //
// Platform:    Dell Inspiron, Win 8.1, Visual Studio 2013         //
// Author:      Venkata Subba Rao Cheedella, Syracuse Univesity    //
//              vcheedel@syr.edu                                   //
/////////////////////////////////////////////////////////////////////

#include "TextSearch.h"
#include "../FileSystemDemo/FileSystem.h"
#include "../Store/Store.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstddef>
#include <list>
#include <set>

//---------------------<Promotion constructor>---------------------------

TextSearch::TextSearch(Store& s) :storage(s)
{
}

//--------------------<Default destructor>--------------------------

TextSearch::~TextSearch()
{
}

//----------------------------------<Get the filenames for given pattern >------------------------------------------------

std::vector<std::map<key, value>::iterator>& TextSearch::getFiles(std::vector<std::string> patt, std::vector<std::map<key, value>::iterator>& filenameiters)
{	
		for (unsigned int i = 0; i < patt.size(); i++)
		{
			std::size_t pos;
			while (patt[i].find_first_of("*") != std::string::npos)
			{
				pos = patt[i].find_first_of("*");
				patt[i].erase(patt[i].begin() + pos);
			}
		}
		for (unsigned int i = 0; i < patt.size(); i++) // Search each file of a map against a set if patterns.
		{
			for (auto miter = storage.getFileBegin(); miter != storage.getFileEnd(); miter++)
			{
				std::size_t found = (miter->first).find(patt[i]);
				if (found != std::string::npos)
					filenameiters.push_back(miter);
			}
		}
		return filenameiters;		
}



//----------------------------< Search the files for given text>-------------------------------------------

std::vector<std::map<key, value>::iterator> TextSearch::searchText(std::vector<std::map<key, value>::iterator>& miters, std::string text)
{	
		std::vector<std::map<key, value>::iterator> fileiters;
		fileiters.clear();
		for (unsigned int i = 0; i < miters.size(); i++)
		{
			std::ifstream file;
			auto liter = miters[i]->second.begin();
			std::string fname = FileSystem::Path::fileSpec(**liter, miters[i]->first); // Retrive the absolute file name for a file.
			file.open(fname);
			if (file.fail())
			{

				file.close();
				std::cout << "\n" << "Unable to open the file" << fname << "\n";
			}
			else
			{
				std::string temp;
				while (getline(file, temp))
				{
					std::size_t found;					
					found = temp.find(text);  // Search the text against a retrived line
					if (found != std::string::npos)
					{
						fileiters.push_back(miters[i]);
						break;
					}

				}

			}
		}		
		return fileiters;
}

//--------------------<Test Stub>--------------------

#ifdef TEST_TEXTSEARCH
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
	s.saveFile("venk2.txt",v1);
	value v2;
	v1.push_back(viter++);
	s.saveFile("venk1.txt", v2);
	std::vector<std::map<key, value>::iterator> mapiters;
	std::vector<std::map<key, value>::iterator> mapitersmatch;
	auto miter = s.getFileBegin();	
	mapiters.push_back(miter);
	mapiters.push_back(miter++);
	std::vector<std::string> patts;
	patts.push_back("ven");
	TextSearch Tsearch(s);
	mapitersmatch = Tsearch.getFiles(patts, mapiters);
	for (unsigned int i = 0; i < mapitersmatch.size(); i++)
	{
		for (auto liter = mapitersmatch[i]->second.begin(); liter != mapitersmatch[i]->second.end(); liter++)
		{
			std::cout << "\n" << FileSystem::Path::fileSpec(**liter, mapitersmatch[i]->first) << "\n";
		}
	}	
}
#endif