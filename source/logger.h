#ifndef ERROR_LOGGER_H
#define ERROR_LOGGER_H


#include <string>
#include <iostream>
using namespace std;

class Logger{
public:
	enum ErrorType{
		NO_NORMALS,
		NO_POINTS,
		NO_POSE,
		NO_IMAGE,
		NO_PIX_COORDS,
		IMAGE_SIZE_NULL,
		FILE_NOT_OPENED,
		PATH_NOT_SET,
		NO_MORE_DATA,
		NO_SEQUENCE,
		NO_PARAMETER,
		NO_LABEL
	};

	enum DebugType{
		INFO_MESSAGE
	};

	static Logger* instance();
	void logError(string className, ErrorType erType, string additionalInfo="");
	void logInfo(const string& message, const string& param1="", const string& param2="");

	template<class T>
	void logInfo(const string& message, const T& param1, const T& param2)
	{
		cout<<"INFO: "<<message<<" "<<param1<<" "<<param2<<endl;
	}

	template<class T>
	void logInfo(const string& message, const T& param)
	{
		cout<<"INFO: "<<message<<" "<<param<<endl;
	}

	void logVerticalSpace(){cout<<endl<<endl<<endl;}

	~Logger();

private:
   Logger(){};  // Private so that it can  not be called
   Logger(Logger const&){};             // copy constructor is private
   Logger& operator=(Logger const&){return *Logger::instance();}  // assignment operator is private
   static Logger* m_pInstance;
};

#endif