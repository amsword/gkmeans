#include "VariedTopkCriterion.h"

void VariedTopkCriterion::Initialize(
	const SMatrix<int>* pGND,
	int nNumberDataBasePoints, 
	Vector<double> *pvec_result)
{
	m_pmatGND = pGND;

	m_pvecResult = pvec_result;
	m_nNumberDataBasePoints = nNumberDataBasePoints;

	m_nNumberValid = m_pvecResult->Size();

	SMART_ASSERT(m_nNumberValid <= m_pmatGND->Cols())
		(m_nNumberValid)
		(m_pmatGND->Cols()).Exit();

	m_pvecResult->SetValueZeros();
}	

void VariedTopkCriterion::EvaluateVariedTopKAddup(
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

void VariedTopkCriterion::EvaluateVariedTopKAllQuery(
	const SMatrix<int> &matRetrievedIndex, 
	int idx_begin, 
	int idx_end, 
	SMatrix<int> &mat_indicator) const
{
	int num_query = idx_end - idx_begin;
	SMART_ASSERT(m_nNumberValid <= matRetrievedIndex.Cols())
		(m_nNumberValid)(matRetrievedIndex.Cols()).Exit();

	SMART_ASSERT(matRetrievedIndex.Cols() >= m_nNumberValid)
		(m_nNumberValid)
		(matRetrievedIndex.Cols()).Exit();
	
	mat_indicator.AllocateSpace(num_query, m_nNumberValid);
	mat_indicator.SetValueZeros();

#pragma omp parallel for
	for (int i = 0; i < num_query; i++)
	{
		const int* p_idx = matRetrievedIndex[i];
		const int* p_gnd = m_pmatGND->operator[](i + idx_begin);

		Vector<int> vec_indicator(m_nNumberDataBasePoints);
		vec_indicator.SetValueZeros();

		int* p_query_indicator = mat_indicator[i];

		for (int j = 0; j < m_nNumberValid; j++)
		{
			int idx = p_gnd[j];
			vec_indicator[idx] = j + 1; // 1-based. 0 is not valid
		}
		for (int j = 0; j < m_nNumberValid; j++)
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

void VariedTopkCriterion::Evaluate(
	const SMatrix<int> &matRetrievedIndex, int idx_begin, int idx_end)
{
	SMatrix<int> mat_indicator;

	EvaluateVariedTopKAllQuery(matRetrievedIndex, idx_begin, idx_end, mat_indicator);
	EvaluateVariedTopKAddup(mat_indicator);
}
