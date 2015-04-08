#pragma once
#include "utility_math.h"

#include "DistanceCalculator.h"

using namespace utility;
class UTILITY_DLL EuclideanDistance : public DistanceCalculator
{
public:
	EuclideanDistance();
	~EuclideanDistance();

public:
	void Initialize(int dim);

public:
	double Distance(
			const void* p_query, const void* p_right) const;
private:
	int m_nDimension;
};

