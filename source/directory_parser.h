#ifndef DIRECTORY_PARSER_H
#define DIRECTORY_PARSER_H

#include <iostream>
#include <dirent.h>
#include <vector>
#include "logger.h"

using namespace std;

class DirectoryParser
{
public:
	DirectoryParser(){}
	std::vector<std::string> getFileNames(const std::string& dirName, const std::vector<std::string> validExtensions);
};

#endif 