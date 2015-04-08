#pragma once
#include "utility_math.h"

namespace utility
{
class MultiIndexMapper
{
public:
	MultiIndexMapper();
	MultiIndexMapper(const Vector<int>& vec);
	~MultiIndexMapper();

public:
	void Initialize(const Vector<int> &vec);
	long long Map(const Vector<int>& multi_index) const;
	long long Map(const int* p_idx) const;
	void Map(long long s, Vector<int> &multi_index) const;

public:
	long long GetTotalNumber() const;
	
private:
	Vector<long long> m_vecProductCumulate;

	long long m_nTotalNumber;
};
}
