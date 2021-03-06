#include "SearchEngineSCK2.h"
#include <ctime>

void SearchEngineSCKMeans2::PopFromHeap(Heap<PAIR<double> >& heap, int* p_idx)
{
	int k = heap.size() - 1;
	//SMART_ASSERT(heap.size() == m_nNumCandidate);

	while (!heap.empty())
	{
		PAIR<double> node;
		heap.popMin(node);
		*(p_idx + k) = node.index;

		k--;
	}
}

void SearchEngineSCKMeans2::ConstructInnerProductMatrix(int idx_table)
{

	const SMatrix<double>& matCodebook = m_pvecmatCodeBook->operator[](idx_table);

	SMatrix<double>& matInnerProduct = m_vecmatInnerProductCodeBook[idx_table];

	int num_code_word = matCodebook.Rows();
	int num_sub_dim = matCodebook.Cols();

	matInnerProduct.AllocateSpace(num_code_word, num_code_word);

	int total = num_code_word * num_code_word;

#pragma omp parallel for
	for (int i = 0; i < total; i++)
	{
		int idx_row = (i / num_code_word);
		int idx_col = (i % num_code_word);

		matInnerProduct[idx_row][idx_col] = dot(matCodebook[idx_row],
			matCodebook[idx_col], num_sub_dim);
	}


}

double SearchEngineSCKMeans2::ConstructPreComputedEachPoint(int idx_point)
{
	double s = 0;

	const CodeType* p_point = m_pmatDataBase->operator[](idx_point);

	for (int k = 0; k < m_numTables; k++)
	{
		const SMatrix<double>& matTable = m_vecmatInnerProductCodeBook[k];

		int idx1;
		int idx2;

		for (int i = 0; i < m_nMaxWordsEachDictionary && ((idx1 = p_point[i]) != -1); i++)
		{
			for (int j = 0; j < m_nMaxWordsEachDictionary && ((idx2 = p_point[j]) != -1); j++)
			{
				s += matTable[idx1 + m_vecAuxOffset[i]][idx2 + m_vecAuxOffset[j]];
			}
		}
		p_point += m_nMaxWordsEachDictionary;
	}

	return 0.5 * s;
}

void SearchEngineSCKMeans2::ConstructPreComputedEachPoint()
{
	m_vecPreComputedEachPoint.AllocateSpace(m_nNumDataBasePoint);

#pragma omp parallel for
	for (int i = 0; i < m_nNumDataBasePoint; i++)
	{
		m_vecPreComputedEachPoint[i] = ConstructPreComputedEachPoint(i);
	}
}

void SearchEngineSCKMeans2::ConstructInnerProductMatrix()
{
	m_vecmatInnerProductCodeBook.AllocateSpace(m_numTables);

	for (int i = 0; i < m_numTables; i++)
	{
		ConstructInnerProductMatrix(i);
	}
}

void SearchEngineSCKMeans2::Initialize(const SMatrix<CodeType>* p_data_base, 
									   const Vector<SMatrix<double> >* pvecmatCodeBook,
									   int num_candidate)
{
	m_pmatDataBase = p_data_base;
	m_nNumDataBasePoint = p_data_base->Rows();

	m_numTables = pvecmatCodeBook->Size();
	m_pvecmatCodeBook = pvecmatCodeBook;
	m_nNumCandidate = num_candidate;
	m_nNumCodeWordEach = pvecmatCodeBook->operator[](0).Rows();
	m_nSubDimensionsEach = pvecmatCodeBook->operator[](0).Cols();

	m_nMaxWordsEachDictionary = p_data_base->Cols() / pvecmatCodeBook->Size();

	m_vecAuxOffset.AllocateSpace(m_nMaxWordsEachDictionary);
	m_vecAuxOffset[0] = 0;
	for (int i = 1; i < m_nMaxWordsEachDictionary; i++)
	{
		m_vecAuxOffset[i] = m_vecAuxOffset[i - 1] + m_nNumCodeWordEach / m_nMaxWordsEachDictionary;
	}

	{
		for (int i = 1; i < pvecmatCodeBook->Size(); i++)
		{
			SMART_ASSERT(pvecmatCodeBook->operator[](i).Rows() == m_nNumCodeWordEach)(pvecmatCodeBook->operator[](i).Rows()).Exit();
			SMART_ASSERT(pvecmatCodeBook->operator[](i).Cols() == m_nSubDimensionsEach)(pvecmatCodeBook->operator[](i).Cols()).Exit();
		}
	}
	m_nTotalTime = 0;
	m_nQueryCount = 0;
	ConstructInnerProductMatrix();

	ConstructPreComputedEachPoint();

	m_vecmatInnerProductCodeBook.UnloadData();
}

void SearchEngineSCKMeans2::ANNSearching(
	void* p_query, int* vec_nn_idx)
{
	Heap<PAIR<double> > heap_knn;

	heap_knn.Reserve(m_nNumCandidate);

	double* pd_query;
	pd_query = (double*)p_query;

	clock_t begin = clock();
	FindBestCandidateAsy(pd_query, heap_knn);
	PopFromHeap(heap_knn, vec_nn_idx);
	clock_t end = clock();

	m_nTotalTime += end - begin;
	m_nQueryCount++;
}

double SearchEngineSCKMeans2::ASymmetricDistance(
	double* p_query, int idx_point, const SMatrix<double> &matQueryCodewordInnerProduct)
{
	double s = 0;

	const CodeType* p_point = m_pmatDataBase->operator[](idx_point);


	for (int k = 0; k < m_numTables; k++)
	{
		const double* p_table = matQueryCodewordInnerProduct[k];

		for (int i = 0; i < m_nMaxWordsEachDictionary; i++)
		{
			int idx = p_point[i] + m_vecAuxOffset[i];
			if (idx == -1)
			{
				continue;
			}
			else
			{
				s += p_table[idx];
			}
		}
		p_point += m_nMaxWordsEachDictionary;
	}

	return m_vecPreComputedEachPoint[idx_point] - s;
}

void SearchEngineSCKMeans2::ConstructQueryCodewordInnerProduct(
	double* p_query, SMatrix<double> &matQueryCodewordInnerProduct)
{
	matQueryCodewordInnerProduct.AllocateSpace(m_numTables, m_nNumCodeWordEach);

	for (int k = 0; k < m_numTables; k++)
	{
		double* p = matQueryCodewordInnerProduct[k];
		const SMatrix<double>& matCodebook = m_pvecmatCodeBook->operator[](k); 

		for (int i = 0; i < m_nNumCodeWordEach; i++)
		{
			p[i] = dot(p_query, matCodebook[i], m_nSubDimensionsEach);
		}
		p_query += m_nSubDimensionsEach;
	}
}


void SearchEngineSCKMeans2::FindBestCandidateAsy(double* p_query, Heap<PAIR<double> >& heap_knn)
{
	SMatrix<double> matQueryCodewordInnerProduct;

	ConstructQueryCodewordInnerProduct(p_query, 
		matQueryCodewordInnerProduct);

	Vector<double*> vecTables(m_numTables);

	for (int j = 0; j < m_nNumDataBasePoint; j++)
	{
		PAIR<double> node;

		double s = ASymmetricDistance(p_query, j, matQueryCodewordInnerProduct);

		node.distance = s;

		if (heap_knn.size() < m_nNumCandidate)
		{
			node.index = j;
			heap_knn.insert(node);
		}
		else
		{
			const PAIR<double>& top = heap_knn.Top();
			if (top < node)
			{
				node.index = j;
				heap_knn.popMin();
				heap_knn.insert(node);
			}
		}
	}
}
