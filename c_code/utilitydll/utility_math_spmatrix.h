#pragma once

#include <string>
#include <utility>
#include "utility_macro.h"
#include "utility_math_vector.h"

using namespace std;

namespace utility
{
template < class T >
class SparseMatrix
{
	template <class T1, class T2>
	friend void SpMatConvert(const SparseMatrix<T1> &from,
		SparseMatrix<T2> &to);
public:
	SparseMatrix()
	{
		m_ppData = NULL;
		m_ppIndex = NULL;
		m_pNumNonZeros = NULL;
		m_nRows = -1;
		m_nCols = -1;
	}

	SparseMatrix(const utility::Vector<pair<int, T > > &vec_one_for_each_row)
	{
		m_nRows = vec_one_for_each_row.Size();
		m_nCols = 1;
		utility::allocate_vector_space(m_ppData, m_nRows);
		utility::allocate_vector_space(m_ppIndex, m_nRows);
		utility::allocate_vector_space(m_pNumNonZeros, m_nRows);

		for (int i = 0; i < m_nRows; i++)
		{
			m_pNumNonZeros[i] = 1;
			utility::allocate_vector_space(m_ppIndex[i], m_pNumNonZeros[i]);
			utility::allocate_vector_space(m_ppData[i], m_pNumNonZeros[i]);

			m_ppIndex[i][0] = vec_one_for_each_row[i].first;
			m_ppData[i][0] = vec_one_for_each_row[i].second;
		}
	}

	~SparseMatrix()
	{
		UnLoadData();
	}
	
public:
	virtual void SaveData(const string &file_name)
	{
		FILE* fp = fopen(file_name.c_str(), "wb");
		EXIT_ASSERT(fp);
		int m = m_nRows;
		int n = m_nCols;
		fwrite(&m, sizeof(int), 1, fp);
		fwrite(&n, sizeof(int), 1, fp);
		
		for (int i = 0; i < m_nRows; i++)
		{
			int num_stored = m_pNumNonZeros[i];
			fwrite(&num_stored, sizeof(int), 1, fp);

			fwrite(m_ppIndex[i], sizeof(int), m_pNumNonZeros[i], fp);
			fwrite(m_ppData[i], sizeof(T), m_pNumNonZeros[i], fp);
		}

		fclose(fp);
	}
	
	virtual void LoadData(const string &file_name)
	{
		UnLoadData();

		FILE* fp = fopen(file_name.c_str(), "rb");
		SMART_ASSERT(fp)(file_name).Exit();
		int m;
		int n;
		fread(&m, sizeof(int), 1, fp);
		fread(&n, sizeof(int), 1, fp);
		
		m_nRows = m;
		m_nCols = n;
		utility::allocate_vector_space(m_ppData, m_nRows);
		utility::allocate_vector_space(m_ppIndex, m_nRows);
		utility::allocate_vector_space(m_pNumNonZeros, m_nRows);

		for (int i = 0; i < m_nRows; i++)
		{
			fread(&m_pNumNonZeros[i], sizeof(int), 1, fp);
			utility::allocate_vector_space(m_ppIndex[i], m_pNumNonZeros[i]);
			utility::allocate_vector_space(m_ppData[i], m_pNumNonZeros[i]);
			fread(m_ppIndex[i], sizeof(int), m_pNumNonZeros[i], fp);
			fread(m_ppData[i], sizeof(T), m_pNumNonZeros[i], fp);
		}

		fclose(fp);
	}

	void CopytTo(SparseMatrix<T>& to)
	{
		to.UnLoadData();
		to.m_nRows = m_nRows;
		to.m_nCols = m_nCols;

		utility::allocate_vector_space(to.m_ppData, m_nRows);
		utility::allocate_vector_space(to.m_ppIndex, m_nRows);
		utility::allocate_vector_space(to.m_pNumNonZeros, m_nRows);

		for (int i = 0; i < m_nRows; i++)
		{
			to.m_pNumNonZeros[i] = m_pNumNonZeros[i];

			utility::allocate_vector_space(to.m_ppIndex[i], m_pNumNonZeros[i]);
			utility::allocate_vector_space(to.m_ppData[i], m_pNumNonZeros[i]);

			memcpy(to.m_ppIndex[i], m_ppIndex[i], sizeof(int) * m_pNumNonZeros[i]);
			memcpy(to.m_ppData[i], m_ppData[i], sizeof(T) * m_pNumNonZeros[i]);
		}
	}

	virtual void UnLoadData()
	{
		utility::release_mat_space(m_ppData, m_nRows);
		utility::release_mat_space(m_ppIndex, m_nRows);
		utility::release_vector_space(m_pNumNonZeros);
		m_nRows = -1;
		m_nCols = -1;
	}

public:
	void Row(int i, int* &p_idx, T* &p_value, int &num_non_zeros) const
	{
		p_idx = m_ppIndex[i];
		
		if (m_ppData) p_value = m_ppData[i];
		else p_value = NULL;

		num_non_zeros = m_pNumNonZeros[i];
	}
	
	int Rows() const 
	{
		return m_nRows;
	}
	
	int Cols() const
	{
		return m_nCols;
	}

	T Max() const
	{
		T max_value = -1000;

		for (int i = 0; i < m_nRows; i++)
		{
			for (int j = 0; j < m_pNumNonZeros[i]; j++)
			{
				T v = m_ppData[i][j];
				if (v > max_value)
				{
					max_value = v;
				}
			}
		}
		return max_value;
	}

protected:
	T** m_ppData; //m_ppData[i]: the i th row, data itself
	int** m_ppIndex; // m_ppIndex[i]: the col index of the i th row
	int* m_pNumNonZeros; // m_pNumNonZeros[i]: the number of m_ppData[i]
	int m_nRows;
	int m_nCols;
};


template <class T1, class T2>
void SpMatConvert(const SparseMatrix<T1> &from,
	SparseMatrix<T2> &to)
{
	to.UnLoadData();
	to.m_nRows = from.m_nRows;
	to.m_nCols = from.m_nCols;

	allocate_vector_space(to.m_pNumNonZeros, to.m_nRows);
	memcpy(to.m_pNumNonZeros, from.m_pNumNonZeros, sizeof(int) * to.m_nRows);

	allocate_vector_space(to.m_ppIndex, from.m_nRows);
	allocate_vector_space(to.m_ppData, from.m_nRows);

	for (int i = 0; i < from.m_nRows; i++)
	{
		int n = from.m_pNumNonZeros[i];
		allocate_vector_space(to.m_ppIndex[i], n);
		memcpy(to.m_ppIndex[i], from.m_ppIndex[i], sizeof(int) * n);

		allocate_vector_space(to.m_ppData[i], n);
		for (int j = 0; j < n; j++)
		{
			to.m_ppData[i][j] = from.m_ppData[i][j];
		}
	}
}
}
