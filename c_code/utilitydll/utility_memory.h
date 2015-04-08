#pragma once

#include "utility_macro.h"

namespace utility
{
	template <class T>
	void allocate_mat_space(T** &pp_data, int rows, int cols)
	{
		pp_data = new T*[rows];
		EXIT_ASSERT(pp_data);

		for (int i = 0; i < rows; i++)
		{
			pp_data[i] = new T[cols];
			EXIT_ASSERT(pp_data[i]);
		}
	}

	template < class T >
	void release_mat_space(T** &pp_data, int rows)
	{
		if (pp_data)
		{
			for (int i = 0; i < rows; i++)
			{
				if (pp_data[i])
				{
					delete[] pp_data[i];
					pp_data[i] = 0;
				}
			}
			delete[] pp_data;
			pp_data = 0;
		}
	}

	template < class T >
	void allocate_vector_space(T* &p_data, long long num)
	{
		p_data = new T[num];
		SMART_ASSERT(p_data).Exit();
	}

	template < class T >
	void release_vector_space(T* &p_data)
	{
		if (p_data)
		{
			delete[] p_data;
			p_data = NULL;
		}
	}
}
