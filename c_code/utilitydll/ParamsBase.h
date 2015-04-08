#pragma once
#include <string>
#include <map>


namespace utility
{
	using namespace std;
class ParamsBase
{
public:
	ParamsBase();
	~ParamsBase();
	
public:
	virtual void SetParameters(const string &str_configure);

protected:
	void InitFromCmd(int argc, char* argv[], map<string, string> &conf);
};
}
