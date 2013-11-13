#ifndef DIRECTORY_PARSER_H
#define DIRECTORY_PARSER_H

#include <iostream>
#include <dirent.h>
#include <vector>
#include <unordered_map>
#include "logger.h"

using namespace std;

class DirectoryParser
{
public:
	DirectoryParser(){}
	std::vector<std::string> getFileNames(
    const std::string& dirName,
    const std::vector<std::string> validExtensions);
  std::unordered_map<std::string, std::string> getLeftRightImagePairs(
    const std::string &dirLeft,
    const std::string &dirRight,
    const std::vector<std::string> validExtensions);
};

#endif