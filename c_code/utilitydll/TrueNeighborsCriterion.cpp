#include <math.h>
#include "TrueNeighborsCriterion.h"

void TrueNeighborsCriterion::Initialize(
	const SMatrix<int>* pGND,
	int nNumberDataBasePoints, 
	const Vector<int>* pvecTrueNeighbors,
	const SMatrix<double>* pmat_query, 
	const SMatrix<double>* pmat_database, 
	Vector<double> *pvec_result,
	SMatrix<double> *pmat_result,
	Vector<double> *pvec_dist_ratio)
{
	m_pmatGND = pGND;

	m_pmatResult = pmat_result;
	m_pvecResult = pvec_result;
	m_pvecNumberTrueNeighbors = pvecTrueNeighbors;
	m_pvecDistanceRatio = pvec_dist_ratio;
	m_nNumberDataBasePoints = nNumberDataBasePoints;

	if (m_pvecNumberTrueNeighbors->Ptr())
	{
		m_pmatResult->SetValueZeros();
		m_pvecResult->SetValueZeros();
		m_nNumberCandidate = m_pmatResult->Cols();
	}

	if (m_pvecDistanceRatio->Ptr())
	{
		m_pvecDistanceRatio->SetValueZeros();
	}

	m_nCollectedAverageDistanceRatio = min(m_nNumberCandidate, m_pmatGND->Cols());

	SMART_ASSERT(m_nNumberCandidate > 0)(m_nNumberCandidate).Exit();
	
	int largest = m_pmatGND->Cols();
	for (int i = 0; i < m_pvecNumberTrueNeighbors->Size(); i++)
	{
		int required = m_pvecNumberTrueNeighbors->operator[](i);
		SMART_ASSERT(required <= largest)(required)(largest).Exit();
	}

	m_pmatQuery = pmat_query;
	m_pmatDatabase = pmat_database;
}

void TrueNeighborsCriterion::EvaluateFixedTopKEach(
	const int* p_idx, const int* p_gnd, 
	int idx_point, Vector<SMatrix<bool> > &vec_mat_indicator) const
{
	int num_topks = m_pvecNumberTrueNeighbors->Size();

	Vector<bool> vec_indicator(m_nNumberDataBasePoints);

	for (int i = 0; i < num_topks; i++)
	{
		int topk = m_pvecNumberTrueNeighbors->operator[](i);

		vec_indicator.SetValueZeros();

		for (int j = 0; j < topk; j++)
		{
			int idx = p_gnd[j];
			vec_indicator[idx] = 1;
		}

		for (int j = 0; j < m_nNumberCandidate; j++)
		{
			int idx = p_idx[j];
			if (vec_indicator[idx])
			{
				vec_mat_indicator[i][idx_point][j]++;
			}
		}
	}
}

void TrueNeighborsCriterion::EvaluateVariedTopKAddup(
	const SMatrix<int> &mat_indicator)
{
	int num_query = mat_indicator.Rows();

	int collected = mat_indicator.Cols();

	for (int i = 0; i < num_query; i++)
	{
		const int* p_indicator = mat_indicator[i];

		for (int j = 0; j < collected; j++)
		{
			if (p_indicator[j])
			{
				m_pvecResult->operator[](j)++;
			}
		}
	}
}

void TrueNeighborsCriterion::EvaluateVariedTopKAllQuery(
	const SMatrix<int> &matRetrievedIndex, int idx_begin, int idx_end, SMatrix<int> &mat_indicator) const
{
	int num_query = idx_end - idx_begin;
	SMART_ASSERT(m_nNumberCandidate == matRetrievedIndex.Cols())
		(m_nNumberCandidate)(matRetrievedIndex.Cols()).Exit();

	int collected = min(m_nNumberCandidate, m_pmatGND->Cols());

	mat_indicator.AllocateSpace(num_query, collected);
	mat_indicator.SetValueZeros();

#pragma omp parallel for
	for (int i = 0; i < num_query; i++)
	{
		const int* p_idx = matRetrievedIndex[i];
		const int* p_gnd = m_pmatGND->operator[](i + idx_begin);

		Vector<int> vec_indicator(m_nNumberDataBasePoints);
		vec_indicator.SetValueZeros();

		int* p_query_indicator = mat_indicator[i];

		for (int j = 0; j < collected; j++)
		{
			int idx = p_gnd[j];
			vec_indicator[idx] = j + 1; // 1-based. 0 is not valid
		}
		for (int j = 0; j < collected; j++)
		{
			int idx = p_idx[j];
			int position = vec_indicator[idx];
			if (position)
			{
				position--;
				if (j >= position)
				{
					p_query_indicator[j]++;
				}
				else
				{
					p_query_indicator[position]++;
				}
			}
		}
	}

}

void TrueNeighborsCriterion::EvaluateVariedTopK(
	const SMatrix<int> &matRetrievedIndex, 
	int idx_begin, 
	int idx_end)
{
	SMatrix<int> mat_indicator;

	EvaluateVariedTopKAllQuery(matRetrievedIndex, idx_begin, idx_end, mat_indicator);
	EvaluateVariedTopKAddup(mat_indicator);
}


void TrueNeighborsCriterion::EvaluateFixedTopK(
	const SMatrix<int> &matRetrievedIndex, int idx_begin, int idx_end)
{
	int num_query = idx_end - idx_begin;
	SMART_ASSERT(m_nNumberCandidate == matRetrievedIndex.Cols())(m_nNumberCandidate)(matRetrievedIndex.Cols()).Exit();

	Vector<bool> vec_indicator(m_nNumberDataBasePoints);

	int num_topks = m_pvecNumberTrueNeighbors->Size();

	Vector<SMatrix<bool> > vec_mat_indicator;
	vec_mat_indicator.AllocateSpace(num_topks);

	for (int i = 0; i < num_topks; i++)
	{
		vec_mat_indicator[i].AllocateSpace(matRetrievedIndex.Rows(), 
			matRetrievedIndex.Cols());
		vec_mat_indicator[i].SetValueZeros();
	}

#pragma omp parallel for
	for (int i = 0; i < num_query; i++)
	{
		const int* p_idx = matRetrievedIndex[i];
		const int* p_gnd = m_pmatGND->operator[](i + idx_begin);

		EvaluateFixedTopKEach(p_idx, p_gnd, i, vec_mat_indicator);
	}

	for (int i = 0; i < num_topks; i++)
	{
		for (int j = 0; j < num_query; j++)
		{
			for (int k = 0; k < m_nNumberCandidate; k++)
			{
				if (vec_mat_indicator[i][j][k])
				{
					m_pmatResult->operator[](i)[k]++;
				}
			}
		}
	}
}

void TrueNeighborsCriterion::EvaluateAverageRatioAllAddup(
	const SMatrix<double> &mat_ratio)
{
	int num_query = mat_ratio.Rows();

	SMART_ASSERT(m_pvecDistanceRatio->Size() == m_nCollectedAverageDistanceRatio)
		(m_pvecDistanceRatio->Size())(m_nCollectedAverageDistanceRatio).Exit();

	for (int i = 0; i < num_query; i++)
	{
		for (int j = 0; j < m_nCollectedAverageDistanceRatio; j++)
		{
			m_pvecDistanceRatio->operator[](j) += mat_ratio[i][j];
		}
	}
}

void TrueNeighborsCriterion::EvaluateAverageRatioAllQuery(
	const SMatrix<int> &matRetrievedIndex, 
	int idx_begin, 
	int idx_end,
	SMatrix<double> &mat_ratio) const
{

	int num_query = idx_end - idx_begin;

	//PRINT << num_query << "\n";

	mat_ratio.AllocateSpace(num_query, m_nCollectedAverageDistanceRatio);

	int dim = m_pmatQuery->Cols();

	long long total = num_query * m_nCollectedAverageDistanceRatio;

#pragma omp parallel for
	for (long long k = 0; k < total; k++)
	{
		int i = (k / m_nCollectedAverageDistanceRatio);
		int j = (k % m_nCollectedAverageDistanceRatio);

		const int* p_idx = matRetrievedIndex[i];
		const int* p_gnd = m_pmatGND->operator[](i + idx_begin);
		const double* p_query = m_pmatQuery->operator[](i + idx_begin);

		int idx_true_neighbor = p_gnd[j];
		int idx_app_neighbor = p_idx[j];

		const double* p_true_neighbor = m_pmatDatabase->operator[](idx_true_neighbor);
		const double* p_app_neighbor = m_pmatDatabase->operator[](idx_app_neighbor);

		double true_dist = squared_distance(p_query, 
			p_true_neighbor, dim);
		double app_dist = squared_distance(p_query,
			p_app_neighbor, dim);
		
		double ratio;
		if (true_dist < 0.00001)
		{
			ratio = 1;
		}
		else
		{
			ratio = app_dist / true_dist;
		}
		mat_ratio[i][j] = sqrt(ratio);
	}
}

void TrueNeighborsCriterion::EvaluateAverageRatio(
	const SMatrix<int> &matRetrievedIndex, int idx_begin, int idx_end)
{
	SMatrix<double> mat_ratio;

	//PRINT << "1\n";
	EvaluateAverageRatioAllQuery(matRetrievedIndex, idx_begin, idx_end, mat_ratio);
	//PRINT << "1\n";
	EvaluateAverageRatioAllAddup(mat_ratio);
}

void TrueNeighborsCriterion::Evaluate(
	const SMatrix<int> &matRetrievedIndex, int idx_begin, int idx_end)
{
	//PRINT << "1\n";
	EvaluateFixedTopK(matRetrievedIndex, 
		idx_begin, 
		idx_end);
	//PRINT << "1\n";
	EvaluateVariedTopK(matRetrievedIndex, 
		idx_begin, 
		idx_end);
		//PRINT << "1\n";
	if (m_pmatDatabase->Ptr() && m_pmatQuery->Ptr())
	{
		EvaluateAverageRatio(matRetrievedIndex, idx_begin, idx_end);
	}
}
