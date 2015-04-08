#include "SaveResultCriterion.h"

SaveResultCriterion::SaveResultCriterion() : 
	m_fp(NULL),
	m_nAdded(0)
{
}
SaveResultCriterion::~SaveResultCriterion()
{
	if (m_fp)
	{
		fclose(m_fp);
		m_fp = NULL;
	}
}

void SaveResultCriterion::Initialize(int num_query, int nCandidate, const string &file_name)
{
	m_fp = fopen(file_name.c_str(), "wb");
	SMART_ASSERT(m_fp)(file_name).Exit();

	m_nTotal = num_query;
	m_nCandidate = nCandidate;
	fwrite(&m_nTotal, sizeof(int), 1, m_fp);
	fwrite(&m_nCandidate, sizeof(int), 1, m_fp);
}	

void SaveResultCriterion::Evaluate(
	const SMatrix<int> &matRetrievedIndex, int idx_begin, int idx_end)
{
	SMART_ASSERT(idx_begin == m_nAdded)(idx_begin)(m_nAdded).Exit();

	SMART_ASSERT(matRetrievedIndex.Cols() == m_nCandidate)(m_nCandidate)(matRetrievedIndex.Cols()).Exit();

	SMART_ASSERT(idx_end - idx_begin <= matRetrievedIndex.Rows()).Exit();

	fwrite(matRetrievedIndex.Ptr(), sizeof(int), (idx_end - idx_begin) * m_nCandidate, m_fp);

}
