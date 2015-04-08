#pragma once

#include <string>
#include <map>
#include <sstream>

using namespace std;

namespace utility
{
	template <class TFrom, class TTo>
	void TypeConvert(TFrom &from, TTo &to)
	{

		stringstream ss;
		ss << from;
		ss >> to;
	}

	template <class T>
	void TypeConvert(const string & from, T &to)
	{
		stringstream ss;

		ss << from;
		ss >> to;
	}



	void Init(const std::string& file_name, 
		std::map<std::string, std::string>& conf);

}
