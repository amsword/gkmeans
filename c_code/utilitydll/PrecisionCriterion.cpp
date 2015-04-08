#include "PrecisionCriterion.h"

void PrecisionCriterion::Initialize(
	const Vector<int> *pQueryLabel,
	const Vector<int>* pDataBaseLabel,
	Vector<double> &vec_precision)
{
	m_pDataBaseLabel = pDataBaseLabel;
	m_pQueryLabel = pQueryLabel;

	m_pVecPrecision = &vec_precision;
	m_nNumberCandidate = m_pVecPrecision->Size();
	m_pVecPrecision->SetValueZeros();


}

void PrecisionCriterion::DebugAssert(
	const SMatrix<int> &matRetrievedIndex, int num_query)
{
	PRINT <<"checking...\n";

	int num_point = m_pDataBaseLabel->Size();

	for (int i = 0; i < num_query; i++)
	{
		for (int j = 0; j < matRetrievedIndex.Cols(); j++)
		{
			int idx = matRetrievedIndex[i][j];

			SMART_ASSERT(idx >= 0 && idx < num_point)(idx)(num_point).Exit();
		}
	}
}

void PrecisionCriterion::Evaluate(
	const SMatrix<int> &matRetrievedIndex, int idx_begin, int idx_end)
{
	int num_query = idx_end - idx_begin;

	DebugAssert(matRetrievedIndex, num_query);
	SMART_ASSERT(m_nNumberCandidate == matRetrievedIndex.Cols())(m_nNumberCandidate)(matRetrievedIndex.Cols()).Exit();

	SMART_ASSERT(idx_begin >= 0 && idx_begin < m_pQueryLabel->Size() &&
		idx_end >= 0 && idx_end <= m_pQueryLabel->Size())
		(idx_end)(m_pQueryLabel->Size()).Exit();

	for (int i = 0; i < num_query; i++)
	{
		const int* p_idx = matRetrievedIndex[i];
		int query_label = m_pQueryLabel->operator[](i + idx_begin);
		//PRINT << query_label << "\n";
		//PRINT << i << "\t" << idx_begin << "\n";
		//SMART_ASSERT(0).Exit();
		for (int j = 0; j < m_nNumberCandidate; j++)
		{
			int idx = p_idx[j];
			//PRINT << (m_pDataBaseLabel->operator[](idx) == query_label) << "\n";
			if (m_pDataBaseLabel->operator[](idx) == query_label)
			{
				m_pVecPrecision->operator[](j)++;
			}
		}
		//SMART_ASSERT(0).Exit();
	}
}
