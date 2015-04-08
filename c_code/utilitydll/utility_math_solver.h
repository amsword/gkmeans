#pragma once

#include "utility_sort.h"

namespace utility
{

	template <class T>
	void Solve2x2AXB(T left11, T left12, 
		T left22, 
		T right1, 
		T right2, 
		T &slope, 
		T &displacement)
	{
		T delta = left11 * left22 - left12 * left12;

		if (delta == 0)
		{
			displacement = right1 - slope * left11;
		}
		else
		{
			slope = left22 * right1 - left12 * right2;
			slope /= delta;

			displacement = -left12 * right1 + left11 * right2;
			displacement /= delta;
		}
	}

	// min{\sum_i{a_i\|x - b_i\|_1}} a_i should be larger than 0
	template <class T >
	void SolveSumL1(const T* pa, const T* pb, int num, T &result)
	{
		Vector<int> vec_sorted_idx;
		Sorting::SortIndex(pb, num, vec_sorted_idx);

		T scan = 0;
		for (int i = 0; i < num; i++)
		{
			SMART_ASSERT(pa[i] >= 0)(pa[i]).Exit();
			scan += pa[i];
		}
		scan = -scan;
		if (scan == 0)
		{
			result = 0;
			return;
		}

		for (int i = 0; i < num; i++)
		{
			int idx = vec_sorted_idx[i];
			if (scan + 2 * pa[idx] >= 0)
			{
				result = pb[idx];
				return;
			}
			else
			{
				scan += 2 * pa[idx];
			}
		}

	}

}
