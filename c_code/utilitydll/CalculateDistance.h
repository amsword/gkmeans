#pragma once

#include "utility_math.h"

namespace utility
{

	class CalculateDistance
	{
	public: 
		template <class T>
		void Run(const IMatrix<T>* pdata, 
			IMatrix<T>* pdist)
		{
			Run(pdata, pdata, pdist);
		}

		template <class T>
		void Run(const IMatrix<T>* pfrom, 
			const IMatrix<T>* pto,
			IMatrix<T>* pdist)
		{
			EXIT_ASSERT(!pto->IsEmpty() &&
				!pfrom->IsEmpty());

			int num_from = pfrom->Rows();
			int num_to = pto->Rows();
			
			EXIT_ASSERT(pto->Cols() == pfrom->Cols());
			
			int dim = pto->Cols();
			pdist->AllocateSpace(num_from, num_to);

			int total = num_from * num_to;

			long long num_calculated = 0;

#pragma omp parallel for
			for (long long i = 0; i < total; i++)
			{
				int idx_from = (i / num_to);
				int idx_to = (i % num_to);
				pdist->operator[](idx_from)[idx_to] = 
					squared_distance(pfrom->operator[](idx_from),
								pto->operator[](idx_to),
								dim);
#pragma omp atomic
				num_calculated++;
				if ((num_calculated % 10000000) == 0)
				{
					PRINT << num_calculated << '/' << total << '/' << num_calculated / (double)total << "\n";
				}
			}
		}
	};
};
