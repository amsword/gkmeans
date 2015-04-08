#pragma once

#include "ForwardIndex.h"
#include "Criterion.h"
#include "utility_math.h"
using namespace utility;

class UTILITY_DLL  VariedTopkCriterion : public Criterion
{
public:

	void Initialize(const SMatrix<int>* pGND,
		int nNumberDataBasePoints, 
		Vector<double> *pvec_result);


public:
	virtual void Evaluate(const SMatrix<int> &matRetrievedIndex, int idx_begin, int idx_end);

private:
	void EvaluateVariedTopKAllQuery(const SMatrix<int> &matRetrievedIndex, int idx_begin, int idx_end, SMatrix<int> &mat_indicator) const;
	void EvaluateVariedTopKAddup(const SMatrix<int> &mat_indicator);

private:
	const SMatrix<int> *m_pmatGND;
	Vector<double>* m_pvecResult;

	int m_nNumberValid;
	int m_nNumberDataBasePoints;
};
