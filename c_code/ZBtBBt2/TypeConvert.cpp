#include "TypeConvert.h"

template <>
void mexConvert<string>(const mxArray* input, string& str)
{
	char *p = mxArrayToString(input);

	str = p;
	mxFree(p);
}