// This file is part of HOGclassifier.

// HOGclassifier is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// HOGclassifier is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with HOGclassifier.  If not, see <http://www.gnu.org/licenses/>.


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