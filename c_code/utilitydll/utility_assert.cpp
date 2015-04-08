#include <cstdlib>
#include "utility_assert.h"
#include "utility_global.h"

Assert::Assert() : SMART_ASSERT_A(*this),
	SMART_ASSERT_B(*this)
{

}

Assert& Assert::print_context(const char* exp)
{
	*GetGlobalLogger() << "Failed: " << exp << "\n";
	return *this;
}

void Assert::Exist0()
{
	(*GetGlobalLogger()).Flush();
	(*GetGlobalLogger()).CloseFile();
	exit(0);
}

void Assert::Exit()
{
	(*GetGlobalLogger()).Flush();
	(*GetGlobalLogger()).CloseFile();
	throw -1;
}

Assert make_assert(const char *file_name, int line_number)
{
	*GetGlobalLogger() << "File: " << file_name << ". Line: " << line_number << "\t";
	return Assert();
}
