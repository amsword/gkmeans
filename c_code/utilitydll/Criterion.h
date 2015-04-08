#pragma once 

#include "utility_math.h"

namespace utility
{
	class UTILITY_DLL Criterion
	{
	public:
		virtual void BeginEvaluate();
		virtual void Evaluate(const SMatrix<int> &matRetrievedIndex, int idx_begin, int idx_end);
		virtual void EndEvaluate();
	};
}
