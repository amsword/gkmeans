#pragma once
#include "utility.h"

using namespace utility;

class SearchEngine
{
public:
	virtual void ANNSearching(void* p_query, int* p_idx) {}
	
	int GetNumberDataPoint() { return m_nNumDataBasePoint; }

	void Print() { PRINT << m_nQueryCount << "\t" << (double)m_nTotalTime / (double)m_nQueryCount << "\n"; }

protected:
	int m_nNumDataBasePoint;

	long long m_nTotalTime;
	int m_nQueryCount;

};
