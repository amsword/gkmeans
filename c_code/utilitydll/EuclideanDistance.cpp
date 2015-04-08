#include "EuclideanDistance.h"

EuclideanDistance::EuclideanDistance()
{

}
EuclideanDistance::~EuclideanDistance()
{

}

double EuclideanDistance::Distance(
			const void* p_query, const void* p_right) const
{
	double r = 0;
	const double* query = (const double*)p_query;
	const double* right = (const double*)p_right;

	for (int i = 0; i < m_nDimension; i++)
	{
		double v1 = query[i];
		double v2 = right[i];
		double diff = v1 - v2;
		r += diff * diff;
	}
	return r;
}
void EuclideanDistance::Initialize(int dim)
{
	m_nDimension = dim;
}
