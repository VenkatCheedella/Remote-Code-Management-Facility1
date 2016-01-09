#include "Store.h"
#include <iostream>
//#include <string>
//#include <vector>
//#include <map>
//#include <list>


Store::Store()
{
}


Store::~Store()
{
}


std::set<std::string>& Store::getDirs()
{
	return listOfDirs;

}
std::map<key, value>& Store::getMap()
{
	return catalogue;
}


#ifdef TEST_STORE
int main()
{

}
#endif
