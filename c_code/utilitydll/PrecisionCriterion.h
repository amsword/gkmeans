#pragma once

#include "ForwardIndex.h"
#include "Criterion.h"
#include "utility_math.h"
using namespace utility;

class UTILITY_DLL PrecisionCriterion : public Criterion
{
public:
	void Initialize(const Vector<int> *pQueryLabel,
		const Vector<int>* pDataBaseLabel, 
		Vector<double> &vec_precision);
public:
	virtual void Evaluate(const SMatrix<int> &matRetrievedIndex, int idx_begin, int idx_end);

	void DebugAssert(const SMatrix<int> &matRetrievedIndex, int count);

private:
	const Vector<int> * m_pDataBaseLabel;
	const Vector<int> * m_pQueryLabel;
	Vector<double>* m_pVecPrecision;
	int m_nNumberCandidate;
};
