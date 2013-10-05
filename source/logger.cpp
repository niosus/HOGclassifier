#include <stddef.h>  // defines NULL
#include "logger.h"

// Global static pointer used to ensure a single instance of the class.
Logger* Logger::m_pInstance = NULL;  
  
Logger* Logger::instance()
{
   if (!m_pInstance)   // Only allow one instance of class to be generated.
      m_pInstance = new Logger;
   return m_pInstance;
}

void Logger::logError(string className, ErrorType erType, string additionalInfo)
{
	cerr<<"ERROR in "<<className;
	switch (erType)
	{
		case NO_NORMALS:
		cerr<<": no normals provided. "<<additionalInfo<<endl;
		break;
		case NO_POINTS:
		cerr<<": no points provided. "<<additionalInfo<<endl;
		break;
		case NO_POSE:
		cerr<<": wrong pose vector. "<<additionalInfo<<endl;
		break;
		case NO_IMAGE:
		cerr<<": no image provided. "<<additionalInfo<<endl;
		break;
		case NO_PIX_COORDS:
		cerr<<": no pixel coords provided. "<<additionalInfo<<endl;
		break;
		case IMAGE_SIZE_NULL:
		cerr<<": size of image is 0. "<<additionalInfo<<endl;
		break;
		case FILE_NOT_OPENED:
		cerr<<": the file could not be opened. "<<additionalInfo<<endl;
		break;
		case PATH_NOT_SET:
		cerr<<": path not set. "<<additionalInfo<<endl;
		break;
		case NO_MORE_DATA:
		cerr<<": no more data to read. "<<additionalInfo<<endl;
		break;
		case NO_SEQUENCE:
		cerr<<": sequence is impossible to create. Check initialization. "<<additionalInfo<<endl;
		break;
		case NO_PARAMETER:
		cerr<<": one or more parameter was not set. "<<additionalInfo<<endl;
		break;
		case NO_LABEL:
		cerr<<": wrong or no label. "<<additionalInfo<<endl;
		break;
	}
}

void Logger::logInfo(const string& message, const string& param1, const string& param2)
{
	cout<<"INFO: "<<message<<" "<<param1<<" "<<param2<<endl;
}

void logInfo(const string& message, const int& param1, const int& param2)
{
	cout<<"INFO: "<<message<<" "<<param1<<" "<<param2<<endl;
}

void logInfo(const string& message, const int& param)
{
	cout<<"INFO: "<<message<<" "<<param<<endl;
}
 
Logger::~Logger()
{
	if (m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = NULL;
	}
}