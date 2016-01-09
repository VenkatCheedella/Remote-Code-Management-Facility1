#include "FileMgr.h"
#include "../FileSystem/FileSystemDemo/FileSystem.h"



FileMgr::FileMgr() :isRecursive(false)
{
}


FileMgr::~FileMgr()
{
}

dirstore& FileMgr::getDirectoriesList(std::string Path, dirstore& dirs)
{
	
	dirs.push_back(Path);
	std::vector<std::string> onlydirs;
	if (isRecursive == true)
		dirs = getAllDirectories(Path, dirs);
	else
	{
		onlydirs = FileSystem::Directory::getDirectories(Path);
		for (auto viter = onlydirs.begin()+2; viter != onlydirs.end(); viter++)
			dirs.push_back(FileSystem::Path::fileSpec(Path, *viter));
	}
		
	return dirs;
}

dirstore& FileMgr::getAllDirectories(std::string Path, dirstore& dirs)
{
	std::vector<std::string> onlydirs;
	int lengthofdirs = dirs.size();
	onlydirs = FileSystem::Directory::getDirectories(Path);
	onlydirs.erase(onlydirs.begin(), onlydirs.begin() + 2);
	for (auto viter = onlydirs.begin(); viter != onlydirs.end(); viter++)
		dirs.push_back(FileSystem::Path::fileSpec(Path, *viter));	
	if (dirs.size() > lengthofdirs)
	{
		for (int i = lengthofdirs; i < dirs.size(); i++)
			getAllDirectories(dirs[i], dirs);
	}		
	
	return dirs;
}

void FileMgr::setPatterns(std::vector<std::string> patt)
{
	patterns = patt;
}


void FileMgr::setRecurrsive(std::string rec)
{
	if (rec == "/s")
		isRecursive = true;
	else
		isRecursive = false;
}

void FileMgr::filesearch(std::vector<std::string>& tempfile, std::string path)
{
	for (auto i = 0; i < patterns.size(); i++)
		tempfile = FileSystem::Directory::getFiles(path, patterns[i]);
}

#ifdef TEST_FILEMGR
int main()
{
	FileMgr ex;
	ex.setRecurrsive("/s");
	std::vector<std::string> dirlist;
	ex.getDirectoriesList("e:/sample", dirlist);
	for (auto diriter = dirlist.begin(); diriter != dirlist.end(); diriter++)
		std::cout << "\n" << *diriter<< "\n";
}
#endif