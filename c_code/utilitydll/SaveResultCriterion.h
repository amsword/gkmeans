#pragma once

#include "ForwardIndex.h"
#include "Criterion.h"
#include "utility_math.h"
using namespace utility;

class UTILITY_DLL  SaveResultCriterion : public Criterion
{
public:
	SaveResultCriterion();
	~SaveResultCriterion();

	void Initialize(int num_query, int m_nCandidate, const string &file_name);


public:
	virtual void Evaluate(const SMatrix<int> &matRetrievedIndex, int idx_begin, int idx_end);

private:

private:
	FILE* m_fp;
	int m_nAdded;
	int m_nTotal;
	int m_nCandidate;
};
