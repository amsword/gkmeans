#include "utility_math_smatrix.h"

namespace utility
{
	template<> SMatrix<SMatrix<double> >& SMatrix<SMatrix<double> >::operator *= (SMatrix<double> v)
	{
		EXIT_ASSERT(0);
		return *this;
	}

	template<> SMatrix<double> SMatrix<SMatrix<double> >::NormL1() const
	{

		return SMatrix<double>();
	}

	template<> SMatrix<SMatrix<int> >& SMatrix<SMatrix<int> >::operator *= (SMatrix<int> v)
	{
		EXIT_ASSERT(0);
		return *this;
	}

	template<> SMatrix<int> SMatrix<SMatrix<int> >::NormL1() const
	{

		return SMatrix<int>();
	}
}