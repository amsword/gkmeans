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
		::SearchEngine* psearch_engine,
		const SMatrix<double>* pmatQuery,
		const Vector<SMatrix<double> >* pvecmatCodeBook,
		int numCandidate,
		SMatrix<int>* pmatRetrievalResults);


private:
	void ClearHeap(Vector<Heap<PAIR<double> > > &vecTop_knn);
	
	void Evaluate(int idx_begin, int idx_end);



private:

	void PopFromHeap(Heap<PAIR<double> >& heap, int* p_idx);

private:
	//const SMatrix<bool>* m_pmatDataBase;

	const SMatrix<double>* m_pmatQuery;

	const Vector<SMatrix<double> >* m_pvecmatCodeBook;

	int m_nNumCandidate;
	int m_nNumTest;
	int m_nNumTraining;

	SMatrix<int>* m_pmatRetrievalResults;
private:
	::SearchEngine* m_psearchEngine;
};
