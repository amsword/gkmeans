#pragma once
#include <string>
#include <climits>
#include "utility_math.h"

#include "InvertedIndex.h"
#include "MultiIndexMapper.h"

#ifndef _WINDOWS
#define _fseeki64 fseek
#endif

using namespace std;

namespace utility
{
template <class T>
class ForwardIndex : public SparseMatrix<T>
{
public:
	ForwardIndex(){}
	ForwardIndex(const Vector<pair<int, T> >& vec) : SparseMatrix<T>(vec) { }
public:
	void LoadData(const string &file_name, int maxNN = INT_MAX, bool is_dist = false)
	{
		this->UnLoadData();

		FILE* fp = fopen(file_name.c_str(), "rb");
		SMART_ASSERT(fp)(file_name).Exit();

		int m;
		int n;
		SMART_ASSERT(fread(&m, sizeof(int), 1, fp) == 1);
		SMART_ASSERT(fread(&n, sizeof(int), 1, fp) == 1);
		
		(this->m_nRows) = m;
		this->m_nCols = n;
		if (is_dist)
		{
			utility::allocate_vector_space(this->m_ppData, (this->m_nRows));
		}
		utility::allocate_vector_space(this->m_ppIndex, (this->m_nRows));
		utility::allocate_vector_space(this->m_pNumNonZeros, (this->m_nRows));

		for (int i = 0; i < (this->m_nRows); i++)
		{
			int num_stored;
			SMART_ASSERT(fread(&num_stored, sizeof(int), 1, fp) == 1);

			if (num_stored > maxNN)
			{
				(this->m_pNumNonZeros)[i] = maxNN;
			}
			else
			{
				(this->m_pNumNonZeros)[i] = num_stored;
			}

			utility::allocate_vector_space(this->m_ppIndex[i], (this->m_pNumNonZeros)[i]);
			SMART_ASSERT(fread(this->m_ppIndex[i], sizeof(int), (this->m_pNumNonZeros)[i], fp) 
				== (this->m_pNumNonZeros)[i]);
			
#ifdef _WINDOWS
			_fseeki64(fp, sizeof(int) * ((long long)num_stored - (this->m_pNumNonZeros)[i]), SEEK_CUR);
#else
			SMART_ASSERT(fseek(fp, sizeof(int) * ((long long)num_stored - (this->m_pNumNonZeros)[i]), SEEK_CUR) == 0);
#endif
			if (is_dist)
			{
				utility::allocate_vector_space(this->m_ppData[i], this->m_pNumNonZeros[i]);
				SMART_ASSERT(fread(this->m_ppData[i], sizeof(T), (this->m_pNumNonZeros)[i], fp) == 0);
			}
			else
			{
#ifdef _WINDOWS
				_fseeki64(fp, (this->m_pNumNonZeros)[i] * sizeof(T), SEEK_CUR);
#else
				SMART_ASSERT(fseek(fp, (this->m_pNumNonZeros)[i] * sizeof(T), SEEK_CUR) == 0);
#endif
			}
#ifdef _WINDOWS
			_fseeki64(fp, sizeof(T) * (num_stored - (this->m_pNumNonZeros)[i]), SEEK_CUR);
#else
			SMART_ASSERT(fseek(fp, sizeof(T) * (num_stored - (this->m_pNumNonZeros)[i]), SEEK_CUR) == 0);
#endif
		}

		fclose(fp);
	}

public:
	void GetList(int i, Vector<pair<int, T> > &vec) const;
	void FromCol(const Matrix<pair<int, T> > &mat_assign);
public:
	void BuildInvertedIndex(
		int num_centers, 
		int num_NN_used,
		InvertedIndex<T> & inverted_list);

	static void BuildInvertedIndex(
		const Matrix<int> &mat_multiple_index,
		const MultiIndexMapper& mapper,
		InvertedIndex<T> &inverted_index);

	static void BuildInvertedIndex(
		const Matrix<int> &mat_multiple_index,
		const Matrix<T> &mat_dist,
		const MultiIndexMapper& mapper,
		InvertedIndex<T> &inverted_index);
};

// -------------------------------------------------------------------

template < class T>
void ForwardIndex<T>::GetList(int which_one, Vector<pair<int, T> > &vec) const
{
	int* p_idx;
	T* p_value;
	int num_valid;
	Row(which_one, p_idx, p_value, num_valid);

	vec.AllocateSpace(num_valid);

	for (int i = 0; i < num_valid; i++)
	{
		vec[i].first = p_idx[i];
		
		if (p_value) vec[i].second = p_value[i];
		else vec[i].second = -1;
	}
}

template < class T>
void ForwardIndex<T>::FromCol(const Matrix<pair<int, T> > &mat_assign)
{
	EXIT_ASSERT(0);

}

template < class T>
void ForwardIndex<T>::BuildInvertedIndex(int num_centers, int num_NN_used,
		InvertedIndex<T> & inverted_list)
{
	if (num_centers == 0)
	{
		for (int i = 0; i < (this->m_nRows); i++)
		{
			int* p_idx;
			T* p_value;
			int num_valid;

			Row(i, p_idx, p_value, num_valid);
			for (int j = 0; j < num_valid; j++)
			{
				if (p_idx[j] > num_centers)
				{
					num_centers = p_idx[j];
				}
			}
		}
		num_centers++;
	}

	inverted_list.Resize(num_centers);

	for (int i = 0; i < (this->m_nRows); i++)
	{
		int* p_idx;
		T* p_value;
		int num_valid;

		Row(i, p_idx, p_value, num_valid);
		
		for (int j = 0; j < ::min(num_valid, num_NN_used); j++)
		{
			if (p_idx[j] < 0 || p_idx[j] >= num_centers)
			{
				PRINT << i << "\t" << p_idx[j] << "\n";
			}

			EXIT_ASSERT(p_idx[j] >= 0 && p_idx[j] < num_centers);

			pair<int, T> p(i, -1);
			if (p_value) p.second = p_value[j];

			inverted_list.Add(p_idx[j], p);
		}
	}
}

template <class T>
void ForwardIndex<T>::BuildInvertedIndex(
	const Matrix<int> &mat_multiple_index,
	const MultiIndexMapper& mapper,
	InvertedIndex<T> &inverted_index)
{
	EXIT_ASSERT(0);
	Vector<int> vec_idx;

	int totalNumber = mapper.GetTotalNumber();

	inverted_index.Resize(mapper.GetTotalNumber());

	for (int j = 0; j < mat_multiple_index.Cols(); j++)
	{
		mat_multiple_index.Col(j, vec_idx);

		int idx = mapper.Map(vec_idx);

		inverted_index.Add(idx, std::pair<int, double>(j, -1));
	}
	inverted_index.SortList();
}

template < class T > 
void ForwardIndex<T>::BuildInvertedIndex(
		const Matrix<int> &mat_multiple_index,
		const Matrix<T> &mat_dist,
		const MultiIndexMapper& mapper,
		InvertedIndex<T> &inverted_index)
{
		Vector<int> vec_idx;

	int totalNumber = mapper.GetTotalNumber();

	inverted_index.Resize(mapper.GetTotalNumber());

	for (int j = 0; j < mat_multiple_index.Cols(); j++)
	{
		mat_multiple_index.Col(j, vec_idx);
		int idx = mapper.Map(vec_idx);

		Vector<T> vec_dist;
		mat_dist.Col(j, vec_dist);

		inverted_index.Add(idx, std::pair<int, T>(j, vec_dist.SumUp()));
	}
	inverted_index.SortList();
}




}
