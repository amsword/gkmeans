#pragma once
#include "DllExportHeader.h"
#include <iostream>
#include "utility_assert.h"

//#define PRINT (*GetGlobalLogger()) << __FILE__ << ": Line:" << __LINE__ << "\t"
#define PRINT (*GetGlobalLogger()) << __FILE__ << ": Line:" << __LINE__ << "\t"
//------------------------------------------------------------------------
 //#define SMART_ASSERT(expr) if( (expr) ) ; else make_assert( #expr).print_context(__FILE__,__LINE__).SMART_ASSERT_A
#define SMART_ASSERT(expr) if( (expr) ) ; else make_assert(__FILE__, __LINE__).print_context(#expr).SMART_ASSERT_A

#define SHOW SMART_ASSERT(0)

#define EXIT_ASSERT(exp) SMART_ASSERT(exp)(exp).Exit()

#define SMART_ASSERT_A(x) SMART_ASSERT_OP(x,B)
#define SMART_ASSERT_B(x) SMART_ASSERT_OP(x,A)
#define SMART_ASSERT_OP(x,next) SMART_ASSERT_A.print_current_val((x),#x).SMART_ASSERT_##next
