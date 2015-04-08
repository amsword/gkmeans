#pragma once
#include <algorithm>
#include <list>
using namespace std;

#include "utility_math_vector.h"

namespace utility
{
	template < class T >
	class Sorter
	{

	public:
		template <class T1>
		struct SSmallFirstPair
		{
			bool operator()(const pair<T1, T>& p1, const pair<T1, T> &p2)
			{
				return p1.second < p2.second;
			}
		};

		template <class T1>
		struct SLargeFirstPair
		{
			bool operator()(const pair<T1, T>& p1, const pair<T1, T> &p2)
			{
				return p1.second > p2.second;
			}
		};

	public:
		static void SortIndex(const T* p, int n, bool isSmallFirst, int *s_idx)
		{
			m_p = p;
			for (int i = 0; i < n; i++)
			{
				s_idx[i] = i;
			}

			if (!isSmallFirst)
			{
				sort(s_idx, s_idx + n, LargeFirst);
			}
			else
			{
				sort(s_idx, s_idx + n, SmallFirst);
			}
		}

	public:
		static void Sort(list<pair<int, T> > &lst, bool isSmallFirst)
		{
			if (isSmallFirst)
			{
				lst.sort(SmallFirstPair);
			}
			else
			{
				lst.sort(LargeFirstPair);
			}
		}
	public:
		static bool SmallFirst(int t1, int t2)
		{
			return m_p[t1] < m_p[t2];
		}
		static	bool LargeFirst(int t1, int t2)
		{
			return m_p[t1] > m_p[t2];
		}
	
	public:
		static bool SmallFirstPair(pair<int, T>& p1, pair<int, T> &p2)
		{
			return p1.second < p2.second;
		}
		static bool LargeFirstPair(pair<int, T> &p1, pair<int, T> &p2)
		{
			return p1.second > p2.second;
		}

	private:
		static const T* m_p;
	};


	class Sorting
	{
	public:
		template <class T>
		static void SortIndex(const Vector<T>& vec_data, Vector<int> &vec_sorted_idx)
		{
			int num_point = vec_data.Size();

			vec_sorted_idx.AllocateSpace(num_point);
			for (int i = 0; i < num_point; i++)
			{
				vec_sorted_idx[i] = i;
			}
			SMART_ASSERT(0).Exit();
			// sort(vec_sorted_idx.Ptr(), vec_sorted_idx.Ptr() + num_point, 
				// [&vec_data](int i1, int i2)
			// {
				// return vec_data[i1] < vec_data[i2];
			// });
		}

		template <class T>
		static void SortIndex(const T* pdata, int num_point, Vector<int> &vec_sorted_idx)
		{
			vec_sorted_idx.AllocateSpace(num_point);
			for (int i = 0; i < num_point; i++)
			{
				vec_sorted_idx[i] = i;
			}
			SMART_ASSERT(0).Exit();
			// sort(vec_sorted_idx.Ptr(), vec_sorted_idx.Ptr() + num_point, 
				// [pdata](int i1, int i2)
			// {
				// return pdata[i1] < pdata[i2];
			// });
		}

		template <class T>
		static void SortIndex(const Vector<T>& vec_data, Vector<int> &vec_sorted_idx, Vector<T> &vec_sorted_value)
		{
			int num_point = vec_data.Size();

			SortIndex(vec_data, vec_sorted_idx);

			vec_sorted_value.AllocateSpace(num_point);

			for(int i = 0; i < num_point; i++)
			{
				vec_sorted_value[i] = vec_data[vec_sorted_idx[i]];
			}
		}

	};


	template < class T>
	const T* Sorter<T>::m_p;
}
