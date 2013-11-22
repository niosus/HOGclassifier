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

#include "directory_parser.h"
#include "stdio.h"
#include "logger.h"
#include <algorithm>
#include <cassert>

static string toLowerCase(const string& in) {
    string t;
    for (string::const_iterator i = in.begin(); i != in.end(); ++i) {
        t += tolower(*i);
    }
    return t;
}

/**
*
* This function refers to GitHub user DaHoC, so doesn't fall under the above licence,
* please refer to his trainHOG project for current licence.
*
**/
std::vector<std::string> DirectoryParser::getFileNames(const std::string& dirName, const std::vector<std::string> validExtensions)
{
    size_t lastSlashLocation = dirName.rfind("/", dirName.length()-2);
	Logger::instance()->logInfo("Opening directory", dirName.substr(lastSlashLocation+1));
	std::vector<string> fileNames;
    struct dirent* ep;
    size_t extensionLocation;
    DIR* dp = opendir(dirName.c_str());
    if (dp != NULL)
    {
        while (ep = readdir(dp))
        {
            // Ignore (sub-)directories like . , .. , .svn, etc.
            if (ep->d_type & DT_DIR)
                continue;
            extensionLocation = string(ep->d_name).find_last_of("."); // Assume the last point marks beginning of extension like file.ext
            // Check if extension is matching the wanted ones
            string tempExt = toLowerCase(string(ep->d_name).substr(extensionLocation + 1));
            if (find(validExtensions.begin(), validExtensions.end(), tempExt) != validExtensions.end())
            {
                fileNames.push_back((string) dirName + ep->d_name);
            }
        }
        (void) closedir(dp);
    } else
    {
        printf("Error opening directory '%s'!\n", dirName.c_str());
    }
    std::sort(fileNames.begin(), fileNames.end());
    Logger::instance()->logInfo("Done");
	return fileNames;
}

std::unordered_map<std::string, std::string> DirectoryParser::getLeftRightImagePairs(
    const std::string &dirLeft,
    const std::string &dirRight,
    const std::vector<std::string> validExtensions)
{
    Logger::instance()->logInfo("Creating pairs of left right image names");
    std::unordered_map<std::string, std::string> result;
    std::vector<std::string> fileNamesLeft = this->getFileNames(dirLeft, validExtensions);
    std::vector<std::string> fileNamesRight = this->getFileNames(dirRight, validExtensions);
    assert(fileNamesLeft.size() == fileNamesRight.size());
    for (int i = 0; i < fileNamesLeft.size(); ++i)
    {
        // Logger::instance()->logInfo(fileNamesLeft[i]);
        result[fileNamesLeft[i]] = fileNamesRight[i];
    }
    Logger::instance()->logInfo("Done");
    return result;
}