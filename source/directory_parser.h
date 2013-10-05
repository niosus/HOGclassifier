#ifndef DIRECTORY_PARSER_H
#define DIRECTORY_PARSER_H

#include <iostream>
#include <dirent.h>

class DirectoryParser
{
public:
	DirectoryParser(){}
	static std::vector<std::string> getFileNames(const std::string& dirName, const std::vector<string> validExtensions);
	{
		printf("Opening directory %s\n", dirName.c_str());
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
	                printf("Found matching data file '%s'\n", ep->d_name);
	                fileNames.push_back((string) dirName + ep->d_name);
	            } else 
	            {
	                printf("Found file does not match required file type, skipping: '%s'\n", ep->d_name);
	            }
	        }
	        (void) closedir(dp);
	    } else 
	    {
	        printf("Error opening directory '%s'!\n", dirName.c_str());
	    }
    	return fileNames;
	}
};

#endif DIRECTORY_PARSER_H