#pragma once

#ifdef _WINDOWS

#include "include\v2013\mex.h"

#pragma comment(lib, "../third_party/mex/lib/v2013/libmx.lib")
#pragma comment(lib, "../third_party/mex/lib/v2013/libmex.lib")
#pragma comment(lib, "../third_party/mex/lib/v2013/libmat.lib")

#else
#include <mex.h>
#endif
