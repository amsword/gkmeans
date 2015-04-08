#pragma once

#include <string>
#include "DllExportHeader.h"

using namespace std;
namespace utility
{
	UTILITY_DLL void GetDims(const string &file_name, int &rows, int &cols);
}
