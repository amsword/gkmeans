#include "DistanceCalculator.h"
#include "utility_macro.h"

namespace utility
{
	bool DistanceCalculator::IsPreprocessingQuery() const
	{
		return false;
	}
	double DistanceCalculator::Distance(
		const void* p_query, const void* p_right) const
	{
		SMART_ASSERT(0)("not deserved").Exit();
		return -1;
	}

	void DistanceCalculator::PreProcessing(const void* p_query, void* &p_pre_out) const
	{
		SMART_ASSERT(0).Exit();
	}
	double DistanceCalculator::DistancePre(void* p_query, const void* p_right) const
	{
		SMART_ASSERT(0)("not deserved").Exit();
		return -1;
	}
	void DistanceCalculator::PostProcessing(void* &p_pre_out) const
	{
		SMART_ASSERT(0).Exit();
	}
}
