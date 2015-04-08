#pragma once

#include "utility_logger.h"

#include "utility_global.h"

class UTILITY_DLL Assert
{
public:
	Assert();
public:
	Assert& SMART_ASSERT_A;
	Assert& SMART_ASSERT_B;
	//whatever member functions
	template <class T>
	Assert& print_current_val(T v,const char* exp)
	{
		(*GetGlobalLogger()) << exp << " = " << v << "\n";
		return *this;
	}
	Assert& print_context(const char* exp);

	void Exit();

	void Exist0();
};

UTILITY_DLL Assert make_assert(const char *file_name, int line_number);
