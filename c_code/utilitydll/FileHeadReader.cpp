#include <stdio.h>
#include "FileHeadReader.h"
#include "utility_macro.h"

namespace utility
{
	void GetDims(const string &file_name, int &rows, int &cols)
	{
		FILE* fp = fopen(file_name.c_str(), "rb");
		SMART_ASSERT(fp)(file_name).Exit();

		SMART_ASSERT(fread(&rows, sizeof(int), 1, fp) == 1).Exit();
		SMART_ASSERT(fread(&cols, sizeof(int), 1, fp) == 1).Exit();

		fclose(fp);
	}
}
