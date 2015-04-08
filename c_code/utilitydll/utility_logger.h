#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "DllExportHeader.h"

using namespace std;
namespace utility
{
	class UTILITY_DLL Logger
	{
	public:
		Logger();
		~Logger();

	public:
		template <class T>
		Logger &operator << (const T& info)
		{
			stringstream ss;
			ss << info;
			string str;
	
			str = ss.str();
			printf("%s",str.c_str()); // in mex file, it doesn't support cout, but printf
			fflush(stdout);
#ifdef _WINDOWS
			if (!m_fout.is_open())
			{
				OpenFile();
			}
			m_fout << info;
			m_fout.flush();
#endif
			return *this;
		}
		void Flush();
		void CloseFile();
	private:
		void OpenFile();


	private:
		ofstream m_fout;
	};
}
