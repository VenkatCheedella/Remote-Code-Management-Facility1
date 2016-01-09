#pragma once
#include <iostream>
#include <string>
#include <vector>

using dirstore = std::vector<std::string>;


class FileMgr
{
public:
	FileMgr();
	~FileMgr();
	dirstore& getDirectoriesList(std::string Path, dirstore& dirs);
	void setPatterns(std::vector<std::string> patt);
	void setRecurrsive(std::string rec);
	void filesearch(std::vector<std::string>& tempfile, std::string path);

private:
	std::vector<std::string> patterns;
	bool isRecursive;
	dirstore& getAllDirectories(std::string Path, dirstore& dirs);
};

