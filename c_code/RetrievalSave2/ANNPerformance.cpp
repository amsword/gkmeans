#include "ANNPerformance.h"

void ANNPerformance::ClearHeap(Vector<Heap<PAIR<double> > > &vecTop_knn)
{
	for (int i = 0; i < vecTop_knn.Size(); i++)
	{
		vecTop_knn[i].clear();
	}
}

void ANNPerformance::Evaluate(int idx_begin, int idx_end)
{
#pragma omp parallel for
	for (int i = idx_begin; i < idx_end; i++)
	{
		m_psearchEngine->ANNSearching(
			m_pmatQuery->operator[](i), 
			m_pmatRetrievalResults->operator[](i));
	}
}

void ANNPerformance::RecallNumberRetrieved(
	::SearchEngine *psearch_engine,
		const SMatrix<double>* pmatQuery,
		const Vector<SMatrix<double> >* pvecmatCodeBook,
		int numCandidate,
		SMatrix<int>* pmatRetrievalResults)
{
	m_pmatQuery = pmatQuery;
	m_pvecmatCodeBook = pvecmatCodeBook;
	m_nNumCandidate = numCandidate;
	m_pmatRetrievalResults = pmatRetrievalResults;
	m_psearchEngine = psearch_engine;

	m_nNumTraining = psearch_engine->GetNumberDataPoint();

	m_nNumTest = m_pmatQuery->Rows();

	int batch_size = 10e4;

	SMART_ASSERT(m_pmatRetrievalResults->Rows() == m_nNumTest && 
		m_pmatRetrievalResults->Cols() == m_nNumCandidate)
		(m_pmatRetrievalResults->Rows())
		(m_nNumTest)
		(m_pmatRetrievalResults->Cols())
		(m_nNumCandidate).Exit();

	SMART_ASSERT(m_nNumCandidate <= m_psearchEngine->GetNumberDataPoint())
		(m_psearchEngine->GetNumberDataPoint()).Exit();

	//SMART_ASSERT((m_nNumTest % batch_size) == 0).Exit();
	int batch_num = m_nNumTest / batch_size + 1;

	for (int batch_idx = 0; batch_idx < batch_num; batch_idx++)
	{
		PRINT << batch_idx << "/" << batch_num << "\n";
		int idx_begin = batch_idx * batch_size;

		if (idx_begin >= m_nNumTest)
		{
			break;
		}

		int idx_end = idx_begin + batch_size;
		
		if (idx_end > m_nNumTest)
		{
			idx_end = m_nNumTest;
		}
		PRINT << idx_begin << "\t" << idx_end << "\n";
		Evaluate(idx_begin, idx_end);
	}
}
