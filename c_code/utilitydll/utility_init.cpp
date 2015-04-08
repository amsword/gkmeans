#include <fstream>
#include <string>
#include <map>
#include <iostream>
#include "utility_init.h"
#include "utility_macro.h"

using namespace std;

namespace utility
{
std::string Trim(const std::string& str)
{
	char buf[512];
	int n = 0;
	for (int i = 0; i < str.length(); i++)
	{
		char c = str[i];
		if (c == ' ')
		{
			continue;
		}
		buf[n++] = c;
	}
	buf[n] = 0;
	return std::string(buf);
}
bool Parse(const std::string &line, 
	std::string &first, 
	std::string &second)
{
	std::string::size_type p1;
	std::string str;

	p1 = std::min(line.find('/'),
		line.find(','));
	p1 = std::min(p1, line.find(';'));
	if (p1 != std::string::npos)
	{
		str = line.substr(0, p1);
	}
	else
	{
		str = line;
	}

	p1 = str.find('=');
	if (p1 == std::string::npos)
	{
		return false;
	}
	first = Trim(str.substr(0, p1));
	second = Trim(str.substr(p1 + 1));
	return true;
}

void read_parameters(const std::string &file_name,
	std::map<std::string, std::string> &conf)
{
	std::ifstream fin;
	fin.open(file_name.c_str());

	EXIT_ASSERT(fin.is_open());

	std::string line;
	conf.clear();
	while(fin.eof() == false)
	{
		std::getline(fin, line);
		std::string first, second;
		bool b;
		b = Parse(line, first, second);
		if (b == false)
		{
			continue;
		}
		conf[first] = second;
	}
}

void print_conf(const std::map<string, string> &conf)
{
	cout << "-------------------------------\n" 
		<< "Print configure file information\n";

	map<string, string>::const_iterator iter;
	for (iter = conf.begin();
		iter != conf.end();
		iter++)
	{
		cout << iter->first.c_str() << " = " 
			<< iter->second.c_str() << "\n";
	}

	cout << "--------------------------------------";
}

void Init(const std::string& file_name, 
	std::map<std::string, std::string>& conf)
{
	read_parameters(file_name, conf);
}
}
