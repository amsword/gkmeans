#include "utility_global.h"
#include "utility_logger.h"

utility::Logger g_logger;

utility::Logger* GetGlobalLogger()
{
	//return NULL;
	return &g_logger;
}
