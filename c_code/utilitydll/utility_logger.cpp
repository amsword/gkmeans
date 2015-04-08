#include <time.h>
#include <string>
#include "utility_logger.h"
#include <sstream>

using namespace std;

namespace utility
{
	Logger::Logger() 
	{

	}

	Logger::~Logger()
	{
		CloseFile();
	}

	void Logger::Flush()
	{
		if(m_fout.is_open())
		{
			m_fout.flush();
		}
	}

	void Logger::CloseFile()
	{
			if (m_fout.is_open())
		{
			m_fout.close();
		}
	}

	void Logger::OpenFile()
	{
		time_t current_time;
		time(&current_time);

		struct tm* timeinfo;
		timeinfo = ::localtime(&current_time);

		int offset = 0;

		string file_name;

		while(1)
		{
			stringstream ss;
			string s;
		
			ss << "log_";
			ss << timeinfo->tm_year + 1900;
			ss << "_";
			ss << timeinfo->tm_mon + 1 << "_"
				<< timeinfo->tm_mday << "_"
				<< timeinfo->tm_hour <<  "_"
				<< timeinfo->tm_min << "_"
				<< timeinfo->tm_sec << "_"
				<< offset << ".txt";
			ss >> file_name;
			
			FILE* fp = fopen(file_name.c_str(), "r");
			if (fp != NULL)
			{
				offset++;
				fclose(fp);
			}
			else
			{
				break;
			}
		}

		m_fout.open(file_name.c_str(), ios::ate);

		if(!m_fout.is_open())
		{
			cout << __FILE__ << "\t" << __LINE__ << "\n"
				<< "Log file can't be opened\n";
			throw -2;
		}
	}
}
