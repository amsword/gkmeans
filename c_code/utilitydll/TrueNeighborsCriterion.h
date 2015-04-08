#pragma once

#include "ForwardIndex.h"
#include "Criterion.h"
#include "utility_math.h"
using namespace utility;

class UTILITY_DLL  TrueNeighborsCriterion : public Criterion
{
public:

	void Initialize(const SMatrix<int>* pGND,
		int nNumberDataBasePoints, 
		const Vector<int>* pvecTrueNeighbors,
		const SMatrix<double>* pmat_query, 
		const SMatrix<double>* pmat_database, 
		Vector<double> *pvec_result,
		SMatrix<double> *pmat_result,
		Vector<double> *pvec_dist_ratio);


public:
	virtual void Evaluate(const SMatrix<int> &matRetrievedIndex, int idx_begin, int idx_end);

private:
	void EvaluateFixedTopKEach(const int* p_idx, const int* p_gnd, 
		int i, Vector<SMatrix<bool> > &vec_mat_indicator) const;

	void EvaluateVariedTopK(const SMatrix<int> &matRetrievedIndex, int idx_begin, int idx_end);
	void EvaluateVariedTopKAllQuery(const SMatrix<int> &matRetrievedIndex, int idx_begin, int idx_end, SMatrix<int> &mat_indicator) const;
	void EvaluateVariedTopKAddup(const SMatrix<int> &mat_indicator);

	void EvaluateFixedTopK(const SMatrix<int> &matRetrievedIndex, int idx_begin, int idx_end);

	void EvaluateAverageRatio(const SMatrix<int> &matRetrievedIndex, int idx_begin, int idx_end);

	void EvaluateAverageRatioAllQuery(const SMatrix<int> &matRetrievedIndex, 
		int idx_begin, int idx_end, SMatrix<double> &mat_ratio) const;

	void EvaluateAverageRatioAllAddup(const SMatrix<double> &mat_ratio);

private:
	const SMatrix<int> *m_pmatGND;

	Vector<double>* m_pvecResult;
	SMatrix<double>* m_pmatResult;
	Vector<double>* m_pvecDistanceRatio;

	const SMatrix<double>* m_pmatQuery;
	const SMatrix<double>* m_pmatDatabase;

	int m_nCollectedAverageDistanceRatio;

	int m_nNumberCandidate;
	int m_nNumberDataBasePoints;
	const Vector<int>* m_pvecNumberTrueNeighbors;

};
