#pragma once

#include <utility>
#include <vector>
#include <list>
#include <string>
#include "utility_math.h"
#include "BruteforceKNNer.h"



//typedef InvertedIndexList vector<list<pair<int, double>>>
namespace utility
{
	using namespace std;
template <class T>
class InvertedIndex
{
public:
	void BinaryWrite(ostream &os);

	virtual void BinaryWrite(FILE* fp);
	virtual void BinaryRead(FILE* fp);

	void BinaryRead(istream &is);

	void SaveData(const string &file_name);
	
	void LoadData(const string &file_name);

public:
	void ComputeAvgCenter(const Matrix<T> &mat_training, Matrix<T> &mat_centers);
public:
	void SortList();
	void Add(int i, pair<int, T>& item);
	
	void Resize(int num_item);

	//virtual void Query(const T* query, Vector<pair<int, T>> &vec_nn_cluster) const;
	//virtual void Query(const T* query, int k, Vector<pair<int, T>> &vec_nn_cluster) const;
	//virtual void OptQuery(const T* query, int k, Vector<pair<int, T>> &vec_nn_cluster) const;
	const list<pair<int, T> >& GetList(int i) const;
	const list<pair<int, T> >& operator[](int i) const;

	int GetPoints() const;

public:
	void CheckDistance() const;

public:
	void Build(const Matrix<T> &mat_centers,
		const Matrix<T> &mat_data,
		int k);
	//mat_forward[i][j]: for jth data point, the ith ranked center. Id and distance
	void Build(int num_centers, const Matrix<pair<int, T> > &mat_forward, int row_used);

	void Build(int num_centers, const Matrix<pair<int, T> > &mat_forward, double dist_ratio);


public: // only used for program checking
	void ReportDistribute() const;
	void GetCountDistribute(Vector<int> &vec) const;
protected:
	vector<list<pair<int, T> > > m_vec_list;
	int m_nNumberPointsInDataBase;
};


template <class T>
void InvertedIndex<T>::BinaryWrite(FILE* fp)
{
	int num_centers = (int)m_vec_list.size();
	fwrite(&num_centers, sizeof(int), 1, fp);

	for (int i = 0; i < num_centers; i++)
	{
		list<pair<int, T> > &lst = m_vec_list[i];
		typename list<pair<int, T> >::const_iterator c_iter;

		int num = (int)lst.size();
		fwrite(&num, sizeof(int), 1, fp);

		for (c_iter = lst.cbegin(); c_iter != lst.cend(); c_iter++)
		{
			int idx = c_iter->first;
			fwrite(&idx, sizeof(int), 1, fp);
		}
		for (c_iter = lst.cbegin(); c_iter != lst.cend(); c_iter++)
		{
			T dist = c_iter->second;
			fwrite(&dist, sizeof(T), 1, fp);
		}
	}
}

template <class T>
void InvertedIndex<T>::BinaryWrite(ostream &os)
{
	int num_centers = (int)m_vec_list.size();
	os.write((char*)&num_centers, sizeof(int));
	//os.write((char*)&m_nNumberPointsInDataBase, sizeof(int));

	for (int i = 0; i < num_centers; i++)
	{
		list<pair<int, T> > &lst = m_vec_list[i];
		typename list<pair<int, T> >::const_iterator c_iter;

		int num = (int)lst.size();
		os.write((char*)&num, sizeof(int));

		for (c_iter = lst.cbegin(); c_iter != lst.cend(); c_iter++)
		{
			int idx = c_iter->first;
			os.write((char*)&idx, sizeof(int));
		}
		for (c_iter == lst.cbegin(); c_iter != lst.cend(); c_iter++)
		{
			T dist = c_iter->second;
			os.write((char*)&dist, sizeof(T));
		}
	}
}

template <class T>
void InvertedIndex<T>::BinaryRead(FILE* fp)
{
	int num_centers;
	fread(&num_centers, sizeof(int), 1, fp);

	m_vec_list.resize(num_centers);
	for (int i= 0; i < num_centers; i++)
	{
		int num_candiate;
		fread(&num_candiate, sizeof(int), 1, fp);
		list<pair<int, T> >& lst = m_vec_list[i];
		
		lst.resize(num_candiate);
		typename list<pair<int, T> >::iterator iter;
		for (iter = lst.begin();
			iter != lst.end();
			iter++)
		{
			int idx;
			fread(&idx, sizeof(int), 1, fp);
			iter->first = idx;
		}
		for (iter = lst.begin();
			iter != lst.end();
			iter++)
		{
			T dst;
			fread(&dst, sizeof(T), 1, fp);
			iter->second = dst;
		}
	}
}

template <class T>
void InvertedIndex<T>::BinaryRead(istream &is)
{
	int num_centers;
	is.read((char*)&num_centers, sizeof(int));
	//is.read((char*)&m_nNumberPointsInDataBase, sizeof(int));

	m_vec_list.resize(num_centers);
	for (int i= 0; i < num_centers; i++)
	{
		int num_candiate;
		is.read((char*)&num_candiate, sizeof(int));
		list<pair<int, T> >& lst = m_vec_list[i];
		
		lst.resize(num_candiate);

		typename list<pair<int, T> >::iterator iter;
		for (iter = lst.begin();
			iter != lst.end();
			iter++)
		{
			int idx;
			is.read((char*)&idx, sizeof(int));
			iter->first = idx;
		}

		for (iter = lst.begin();
			iter != lst.end();
			iter++)
		{
			T dst;
			is.read((char*)&dst, sizeof(T));
			iter->second = dst;
		}
	}
}

template <class T>
void InvertedIndex<T>::LoadData(const string &file_name)
{
	SMART_ASSERT(0).Exit();
	// ifstream fin;

	// fin.open(file_name, ios::binary);
	// EXIT_ASSERT(fin.is_open());

	// BinaryRead(fin);
	
	// fin.close();

	//PRINT << m_vec_list.size() << "\n";
	//EXIT_ASSERT(0);

}

template <class T>
void InvertedIndex<T>::SaveData(const string &file_name)
{
	FILE* fp = fopen(file_name.c_str(), "wb");
	EXIT_ASSERT(fp);

	BinaryWrite(fp);

	fclose(fp);
}

template <class T>
void InvertedIndex<T>::ReportDistribute() const
{
	int total_num = 0;
	for (int i = 0; i < m_vec_list.size(); i++)
	{
		total_num += m_vec_list[i].size();
	}
	PRINT << "total item in the inverted list: " << total_num << "\n"
		<< "avg number of items in one list: " << total_num / (double)m_vec_list.size() << "\n";
}
template <class T>
void InvertedIndex<T>::GetCountDistribute(Vector<int> &vec) const
{
	vec.AllocateSpace(m_vec_list.size());
	for (int i = 0; i < m_vec_list.size(); i++)
	{
		vec[i] = m_vec_list[i].size();
	}
}

template <class T>
int InvertedIndex<T>::GetPoints() const
{
	return m_nNumberPointsInDataBase;
}

template <class T>
void InvertedIndex<T>::Resize(int num_item)
{
	m_vec_list.resize(num_item);
	//m_nNumberPointsInDataBase = 0;
}

//template <class T>
//void InvertedIndex<T>::Query(const T* query, Vector<pair<int, T>> &vec_nn_cluster) const
//{
//	EXIT_ASSERT(0);
//}

//template <class T>
//void InvertedIndex<T>::OptQuery(const T* query, int k, Vector<pair<int, T>> &vec_nn_cluster) const
//{
//	EXIT_ASSERT(0);
//}
//
//template <class T>
//void InvertedIndex<T>::Query(const T* query, int k, Vector<pair<int, T>> &vec_nn_cluster) const
//{
//	EXIT_ASSERT(0);
//}

template <class T>
const list<pair<int, T> >& InvertedIndex<T>::GetList(int i) const
{
	return m_vec_list[i];
}

template <class T>
const list<pair<int, T> >& InvertedIndex<T>::operator[](int i) const
{
	return m_vec_list[i];
}

template <class T>
void InvertedIndex<T>::Add(int i, pair<int, T>& item)
{
	m_vec_list[i].push_back(item);
	//m_nNumberPointsInDataBase++;
}



template <class T>
void InvertedIndex<T>::ComputeAvgCenter(const Matrix<T> &mat_training, Matrix<T> &mat_centers)
{
	SMART_ASSERT(0).Exit();
	// mat_centers.AllocateSpace(m_vec_list.size(), mat_training.Cols());

	// for(int i = 0; i < mat_centers.Rows(); i++)
	// {
		// const list<pair<int, TypeData>>& lst = m_vec_list[i];

		// Vector<T> vec_total(mat_centers[i], mat_centers.Cols());
		// vec_total.SetValueZeros();
		// typename list<pair<int, TypeData> >::const_iterator citer;
		// for (citer = lst.cbegin();
			// citer != lst.cend(); citer++)
		// {
			// int idx = citer->first;
			// vec_total += Vector<T>(mat_training[idx], mat_training.Cols());
		// }
		// vec_total /= (T)lst.size();
	// }
}

template <class T>
void InvertedIndex<T>::SortList()
{
	SMART_ASSERT(0).Exit();
	// vector<TypeIIList>::iterator iter;
	// for (iter = m_vec_list.begin();
		// iter != m_vec_list.end();
		// iter++)
	// {
		// TypeIIList &lst = *iter;

		// utility::Sorter<TypeData>::Sort(lst, true);
	// }
}

// ------------------------------------
template <class T>
void InvertedIndex<T>::Build(const Matrix<T> &mat_centers,
		const Matrix<T> &mat_data,
		int k)
{
	m_nNumberPointsInDataBase = mat_data.Rows();
	m_vec_list.clear();
	m_vec_list.resize(mat_centers.Rows());
	
	BruteforceKNNer<T> knner;

	Vector<Vector<pair<int, T> > > vecvec_nn(mat_data.Rows());

#pragma omp parallel for
	for (int i = 0; i < mat_data.Rows(); i++)
	{
		if ((i % 100) == 0)
		{
			cout << "building: " << i << "\n";
		}

		knner.Run(mat_data[i], mat_centers, k, vecvec_nn[i]);
	}

	for (int i = 0; i < vecvec_nn.Size(); i++)
	{
		const Vector<pair<int, T> > &vec_nn = vecvec_nn[i];
		for (int j = 0; j < vec_nn.Size(); j++)
		{
			Add(vec_nn[j].first, pair<int, T>(i, vec_nn[j].second));
		}
	}
}

template <class T>
void InvertedIndex<T>::Build(int num_centers, const Matrix<pair<int, T> > &mat_forward, double dist_ratio)
{
	m_vec_list.clear();
	m_vec_list.resize(num_centers);
	
	int count = 0;
	for (int j = 0; j < mat_forward.Cols(); j++)
	{
		double th = mat_forward[0][j].second * dist_ratio;
		for (int i = 0; i < mat_forward.Rows(); i++)
		{
			const pair<int, T> &p = mat_forward[i][j];
			if (p.first != -1 && p.second < th)
			{
				EXIT_ASSERT(p.first < num_centers);
				Add(p.first, pair<int, T>(j, p.second));
				if (i == 1)
				{
					count++;
				}
			}
		}
	}
	PRINT << "benifit pairs: " << count << "\n";
}

template <class T>
void InvertedIndex<T>::Build(int num_centers, const Matrix<pair<int, T> > &mat_forward, int row_used)
{
	//m_nNumberPointsInDataBase = mat_forward.Cols();
	m_vec_list.clear();
	m_vec_list.resize(num_centers);
	
	for (int i = 0; i < min(mat_forward.Rows(), row_used); i++)
	{
		for (int j = 0; j < mat_forward.Cols(); j++)
		{
			const pair<int, T> &p = mat_forward[i][j];
			if (p.first != -1)
			{
				EXIT_ASSERT(p.first < num_centers);
				Add(p.first, pair<int, T>(j, p.second));
			}
		}
	}
}

template < class T >
void InvertedIndex<T>::CheckDistance() const
{
	for (int i = 0; i < m_vec_list.size(); i++)
	{
		const list<pair<int, T> >& lst = m_vec_list[i];
		T pre = -1;
		typename list<pair<int, T> >::const_iterator citer;
		for (citer = lst.cbegin();
			citer != lst.cend();
			citer++)
		{
			EXIT_ASSERT(pre <= citer->second);
			pre = citer->second;
		}
	}
}


}
