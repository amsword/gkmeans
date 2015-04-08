#pragma once

#include <string>

#include "utility.h"
#include "SearchEngine.h"

#include "SearchEngineSCKMeans.h"

using namespace std;

using namespace utility;

class ANNPerformance
{
public:
	void RecallNumberRetrieved(
		const SMatrix<int>* pmatRetrievalResult,
		int num_data_base_points,
		const ForwardIndex<double>* pgnd,
		const Vector<int>* pvecTopKs,
		SMatrix<double>* pmatRecallEachTopk);


private:
	void ClearHeap(Vector<Heap<PAIR<double> > > &vecTop_knn);

	void Evaluate(int idx_begin, int idx_end);

	void Evaluate(int idx_test, const int* p_retrieved_idx, double* p_recall, int topk);

	void Evaluate1(int idx_test, const int* p_retrieved_idx, double* p_recall, int topk);


	void AddupBatchResult(Vector<SMatrix<double> > &vecmatRecallEach);

private:

	void PopFromHeap(Heap<PAIR<double> >& heap, int* p_idx);

private:
	//const SMatrix<bool>* m_pmatDataBase;

	const ForwardIndex<double>* m_pgnd;

	int m_nNumCandidate;
	int m_nNumTest;
	int m_nNumTraining;

	const Vector<int>* m_pvecTopKs;

	SMatrix<double>* m_pmatRecallEachTopk;
	const SMatrix<int>* m_pmatRetrievalResult;

private:
};
