#include "MultiIndexMapper.h"


namespace utility
{
MultiIndexMapper::MultiIndexMapper()
{
}

MultiIndexMapper::MultiIndexMapper(const Vector<int>& vec)
{
	Initialize(vec);
}

MultiIndexMapper::~MultiIndexMapper()
{
}

void MultiIndexMapper::Initialize(const Vector<int> &vec)
{
	int length = vec.Size();

	m_vecProductCumulate.resize(length);

	m_vecProductCumulate[0] = 1;
	
	for (int i = 1; i < length; i++)
	{
		m_vecProductCumulate[i] = m_vecProductCumulate[i - 1] * 
			vec[i - 1];
	}
	m_nTotalNumber = m_vecProductCumulate[length - 1] * 
		vec[length - 1];
}

long long MultiIndexMapper::Map(const Vector<int>& multi_index) const
{
	return Map(&multi_index[0]);
}

long long MultiIndexMapper::Map(const int* p_idx) const
{
	long long s = 0;

	for (int i = 0; i < m_vecProductCumulate.Size(); i++)
	{
		s += p_idx[i] * m_vecProductCumulate[i];
	}

	return s;
}

void MultiIndexMapper::Map(long long s, Vector<int> &multi_index) const
{
	int length = m_vecProductCumulate.Size();
	multi_index.AllocateSpace(length);
	for (int i = length - 1; i >= 0; i--)
	{
		int idx = s / m_vecProductCumulate[i];
		multi_index[i] = idx;
		s -= idx * m_vecProductCumulate[i];
	}
}

long long MultiIndexMapper::GetTotalNumber() const
{
	return m_nTotalNumber;
}
}
