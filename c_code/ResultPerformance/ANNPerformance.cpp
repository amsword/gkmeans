#include "ANNPerformance.h"

void ANNPerformance::ClearHeap(Vector<Heap<PAIR<double> > > &vecTop_knn)
{
	for (int i = 0; i < vecTop_knn.size(); i++)
	{
		vecTop_knn[i].clear();
	}
}

void ANNPerformance::Evaluate1(int idx_test, const int* p_retrieved_idx, double* p_recall, int topk)
{
	int* p_idx;
	double* p_value;
	int num_valid;

	m_pgnd->Row(idx_test, p_idx, p_value, num_valid);

	SMART_ASSERT(num_valid >= topk);

	map<int, int> indicator;

	for (int i = 0; i < topk; i++)
	{
		int idx = p_idx[i];
		indicator[idx] = 1;
	}

	memset(p_recall, 0, sizeof(double) * m_nNumCandidate);

	for (int i = 0; i < m_nNumCandidate; i++)
	{
		int idx = p_retrieved_idx[i];
		if (indicator.count(idx))
		{
			p_recall[i]++;
		}
	}
}

void ANNPerformance::Evaluate(int idx_test, const int* p_retrieved_idx, double* p_recall, int topk)
{
	int* p_idx;
	double* p_value;
	int num_valid;

	m_pgnd->Row(idx_test, p_idx, p_value, num_valid);

	SMART_ASSERT(num_valid >= topk);

	// set indicator function
	Vector<bool> vec_indicator(m_nNumTraining);
	vec_indicator.SetValueZeros();
	for (int i = 0; i < topk; i++)
	{
		int idx = p_idx[i];
		vec_indicator[idx] = 1;
	}

	memset(p_recall, 0, sizeof(double) * m_nNumCandidate);

	for (int i = 0; i < m_nNumCandidate; i++)
	{
		int idx = p_retrieved_idx[i];
		int is_true = vec_indicator[idx];
		if (is_true)
		{
			p_recall[i]++;
		}
	}
}

void ANNPerformance::AddupBatchResult(Vector<SMatrix<double> > &vecmatRecallEach)
{
	for (int k = 0; k < m_pvecTopKs->size(); k++)
	{
		SMatrix<double> &matRecallEach = vecmatRecallEach[k];
		double* p_out = m_pmatRecallEachTopk->operator[](k);

		for (int i = 0; i < matRecallEach.Rows(); i++)
		{
			double* p_curr = matRecallEach[i];
			for (int j = 0; j < matRecallEach.Cols(); j++)
			{
				*(p_out + j) += p_curr[j];
			}
		}
	}
}

void ANNPerformance::Evaluate(int idx_begin, int idx_end)
{
	int batch_size = idx_end - idx_begin;

	Vector<SMatrix<double> > vecmatRecallEach;
	vecmatRecallEach.AllocateSpace(m_pvecTopKs->size());
	for (int k = 0; k < m_pvecTopKs->size(); k++)
	{
		vecmatRecallEach[k].AllocateSpace(batch_size, m_nNumCandidate);
	}

#pragma omp parallel for
	for (int i = idx_begin; i < idx_end; i++)
	{
		int* p_idx;
		double* p_value;
		int tmp_count;

		int idx_heap = i - idx_begin;

		//m_psearchEngine->ANNSearching(m_pmatQuery->operator[](i), matRetrievedIndex[idx_heap]);

		for (int idx_topk = 0; idx_topk < m_pvecTopKs->size(); idx_topk++)
		{
			//Evaluate(i, matRetrievedIndex[idx_heap], 
			//	vecmatRecallEach[idx_topk][idx_heap], m_pvecTopKs->operator[](idx_topk));
			Evaluate1(i, m_pmatRetrievalResult->operator[](i), 
				vecmatRecallEach[idx_topk][idx_heap], m_pvecTopKs->operator[](idx_topk));
		}
	}

	AddupBatchResult(vecmatRecallEach);
}


void ANNPerformance::RecallNumberRetrieved(
	const SMatrix<int>* pmatRetrievalResult,
	int num_data_base_points,
	const ForwardIndex<double>* pgnd,
	const Vector<int>* pvecTopKs,
	SMatrix<double>* pmatRecallEachTopk)
{
	m_pgnd = pgnd;
	m_pvecTopKs = pvecTopKs;
	m_nNumCandidate = pmatRetrievalResult->Cols();
	m_pmatRecallEachTopk = pmatRecallEachTopk;
	m_pmatRetrievalResult = pmatRetrievalResult;

	m_nNumTraining = num_data_base_points;

	m_nNumTest = m_pgnd->Rows();

	int batch_size = 1000;

	SMART_ASSERT(m_pmatRecallEachTopk->Rows() == m_pvecTopKs->Size())(m_pmatRecallEachTopk->Rows()).Exit();
	SMART_ASSERT(m_pmatRecallEachTopk->Cols() == m_nNumCandidate)(m_pmatRecallEachTopk->Cols()).Exit();
	m_pmatRecallEachTopk->SetValueZeros();

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

	// cummulate
	for (int i = 0; i < m_pmatRecallEachTopk->Rows(); i++)
	{
		double s = 0;
		for (int j = 0; j < m_pmatRecallEachTopk->Cols(); j++)
		{
			m_pmatRecallEachTopk->operator[](i)[j] += s;
			s = m_pmatRecallEachTopk->operator[](i)[j];
		}
	}

	// divide the topk
	for (int i = 0; i < m_pmatRecallEachTopk->Rows(); i++)
	{
		scale_multi_vector(1.0 / m_pvecTopKs->operator[](i), m_pmatRecallEachTopk->operator[](i), 
			m_pmatRecallEachTopk->operator[](i), m_pmatRecallEachTopk->Cols());
	}

	// divide the total num of test
	*m_pmatRecallEachTopk *= 1.0 / m_nNumTest;
}
