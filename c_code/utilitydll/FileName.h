#pragma once
#include <string>

using namespace std;

namespace utility
{
class FileName
{
public:
	FileName();
	~FileName();

public:
	static string Concatenate(const string &str_prefix, int which_one);
};
}
