#include "FileName.h"

#include <sstream>
using namespace std;
namespace utility
{
string FileName::Concatenate(const string &str_prefix, int which_one)
{
	stringstream ss;
	ss << which_one;
	string s;
	ss >> s;
	return str_prefix + "_" + s;
}
}
