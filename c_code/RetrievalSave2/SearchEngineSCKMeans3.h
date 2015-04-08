#pragma once


#include "SearchEngine.h"

class SearchEngineSCKMeans3 : public ::SearchEngine
{
public:
	void ANNSearching(void* p_query, int* vec_nn_idx);

public:
	void Initialize(const SMatrix<short>* p_data_base, 
		const Vector<SMatrix<double> >* pvecmatCodeBook,
		int num_candidate);

private:
	//void ConstructInnerProductMatrix();

	//void ConstructInnerProductMatrix(int idx_table);

	//void ConstructPreComputedEachPoint();

	//double ConstructPreComputedEachPoint(int idx_point);

	double ASymmetricDistance(double* p_query, int idx_point, const SMatrix<double> &matQueryCodewordInnerProduct);

	void FindBestCandidateAsy(double* p_query, Heap<PAIR<double> >& heap);
	
	void ConstructQueryCodewordInnerProduct(
		double* p_query, SMatrix<double> &matQueryCodewordInnerProduct);

	void PopFromHeap(Heap<PAIR<double> >& heap, int* p_idx);

private:
	const SMatrix<short>* m_pmatDataBase;
	const Vector<SMatrix<double> >* m_pvecmatCodeBook;

	//Vector<SMatrix<double>> m_vecmatInnerProductCodeBook;

	int m_nMaxWordsEachDictionary;

	int m_method;

	int m_numTables;

	int m_nNumCodeWordEach;
	int m_nSubDimensionsEach;

	int m_nNumCandidate;
};
