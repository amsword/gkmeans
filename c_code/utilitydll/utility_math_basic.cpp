#include <math.h>
#include <assert.h>
#include <cstdlib>
#include "utility_math_basic.h"
#include "utility_macro.h"

#include "utility_types.h"

namespace utility
{
	double sigmf(double x, double alpha)
	{
		return 1.0 / (1 + exp(-x * alpha));
	}


	void rand_perm(int n, int* p, int k)
	{
		EXIT_ASSERT(k <= n);

		int* p_aux = new int[n];
		EXIT_ASSERT(p_aux);

		for (int i = 0; i < n; i++)
		{
			p_aux[i] = i;
		}

		for (int i = 0; i < k; i++)
		{
			double f_idx = (double)rand() / RAND_MAX * (n - 0.1 - i);
			int idx = (int)f_idx + i;

			assert(idx >= i && idx < n);

			int t = p_aux[i];
			p_aux[i] = p_aux[idx];
			p_aux[idx] = t;

			p[i] = p_aux[i];
		}

		delete[] p_aux; 
		p_aux = NULL;
	}
}