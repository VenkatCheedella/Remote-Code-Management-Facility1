#pragma once
#include <iostream>
#include <set>
#include <map>
#include <list>
#include <set>

using key = std::string;
using ref = std::set<std::string>::iterator;
using value = std::list<std::set<std::string>::iterator>;

class Store
{
public:
	Store();
	~Store();	
	std::set<std::string>& getDirs();
	std::map<key, value>& getMap();
		
private:
	std::set<std::string> listOfDirs;
	std::map<key, value> catalogue;	
};

