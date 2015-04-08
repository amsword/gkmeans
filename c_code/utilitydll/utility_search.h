#pragma once

#include "utility_macro.h"

namespace utility
{
	// target >= p_inc_data[return] && target < p_inc_data[return + 1]
template <class T>
	int binary_search_range(T target, T* p_inc_data, int num)
	{
		EXIT_ASSERT(num > 0);

		int start = 0;
		int end = num - 1;

		if (target < p_inc_data[start])
		{
			return -1;
		}
		if (target > p_inc_data[end])
		{
			return end;
		}

		// p_inc_data[start] <= target <= p_inc_data[end]
		while(start < end)
		{
			if (start == end - 1)
			{
				return start;
			}

			int mid = (start + end) / 2;
			if (target == p_inc_data[mid])
			{
				return mid;
			}
			else if (target < p_inc_data[mid])
			{
				end = mid;
			}
			else
			{
				start = mid;
			}
		}
	}
}
