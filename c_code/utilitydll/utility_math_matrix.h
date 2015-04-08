#pragma once
#include <string>
#include "utility_macro.h"
#include "utility_math_vector.h"
#include "utility_math_imatrix.h"
#include "utility_math_basic.h"
using namespace std;

namespace utility
{
template < class T >
class Matrix : public IMatrix<T>
{
public:
	Matrix()
		:IMatrix<T>(),
		m_ppData(NULL),
		m_isOwnData(false)
	{
	}
	~Matrix()
	{
		UnLoadData();
	}

	Matrix(T** p, int rows, int cols)
	{
		m_ppData = p;
		(this->m_nCols) = rows;
		(this->m_nCols) = cols;
		m_isOwnData = false;
	}
	Matrix(const Matrix<T> &from)
		: m_ppData(NULL),
		IMatrix<T>::m_nRows(0),
		IMatrix<T>::m_nCols(0),
		m_isOwnData(false)
	{
		from.CopyTo(*this);
	}
	template < class T2 >
	friend ostream& operator << (ostream &os, const Matrix<T2>& mat);

public:
	void AllocateSpace(int m, int n)
	{
		if (m !=(this->m_nRows)|| n !=(this->m_nCols))
		{
			utility::release_mat_space(m_ppData, (this->m_nRows));
			(this->m_nCols) = m;
			(this->m_nCols) = n;
			utility::allocate_mat_space(m_ppData, m, n);
			m_isOwnData = true;
		}
	}
	
	void Attach(T** p, int rows, int cols)
	{
		UnLoadData();

		m_ppData = p;
		(this->m_nCols) = rows;
		(this->m_nCols) = cols;
		m_isOwnData = false;
	}

	void Detatch(T** &p, int &m, int &n)
	{
		p = m_ppData;
		m =(this->m_nRows);
		n =this->m_nCols;

		m_ppData = NULL;
		(this->m_nCols) = 0;
		(this->m_nCols) = 0;
		m_isOwnData = false;
	}

	void LoadData(const string &file_name)
	{
		FILE* fp = fopen(file_name.c_str(), "rb");
		EXIT_ASSERT(fp);
		int m;
		int n;
		fread(&m, sizeof(int), 1, fp);
		fread(&n, sizeof(int), 1, fp);

		AllocateSpace(m, n);

		for (int i = 0; i <(this->m_nRows); i++)
		{
			fread(m_ppData[i], sizeof(T), n, fp);
		}
		fclose(fp);
	}
	
	void LoadDataToHomogeneous(const string &file_name)
	{
		FILE* fp = fopen(file_name.c_str(), "rb");
		EXIT_ASSERT(fp);
		int m;
		int n;
		fread(&m, sizeof(int), 1, fp);
		fread(&n, sizeof(int), 1, fp);

		AllocateSpace(m, n + 1);

		for (int i = 0; i <(this->m_nRows); i++)
		{
			fread(m_ppData[i], sizeof(T), n, fp);
			SMART_ASSERT(0).Exit();
			//m_ppData[i][n] = 1;
		}
		fclose(fp);
	}
	
	void SaveData(const string &file_name) const
	{
		EXIT_ASSERT((this->m_nRows) > 0 && (this->m_nCols) > 0);

		FILE* fp = fopen(file_name.c_str(), "wb");
		EXIT_ASSERT(fp);

		fwrite(&(this->m_nRows), sizeof(int), 1, fp);
		fwrite(&(this->m_nCols), sizeof(int), 1, fp);

		for (int i = 0; i <(this->m_nRows); i++)
		{
			fwrite(m_ppData[i], sizeof(T), (this->m_nCols), fp);
		}

		fclose(fp);
	}

	void BinaryWrite(ostream &os) const
	{
		os.write((char*)&(this->m_nRows), sizeof(int));
		os.write((char*)&(this->m_nCols), sizeof(int));

		for (int i = 0; i <(this->m_nRows); i++)
		{
			os.write((char*)m_ppData[i], sizeof(T) *(this->m_nCols));
		}
	}

	void BinaryRead(istream &is)
	{
		int m, n;
		is.read((char*)&m, sizeof(int));
		is.read((char*)&n, sizeof(int));

		AllocateSpace(m, n);

		for (int i = 0; i <(this->m_nRows); i++)
		{
			is.read((char*)m_ppData[i], sizeof(T) * n);
		}
	}

	void BinaryWrite(FILE* fp) const
	{
		fwrite(&(this->m_nRows), sizeof(int), 1, fp);
		fwrite(&(this->m_nCols), sizeof(int), 1, fp);

		for (int i = 0; i <(this->m_nRows); i++)
		{
			fwrite(m_ppData[i], sizeof(T),  (this->m_nCols), fp);
		}
	}

	void BinaryRead(FILE* fp)
	{
		int m, n;
		fread(&m, sizeof(int), 1, fp);
		fread(&n, sizeof(int), 1, fp);

		AllocateSpace(m, n);

		for (int i = 0; i <(this->m_nRows); i++)
		{
			fread(m_ppData[i], sizeof(T), (this->m_nCols), fp);
		}
	}


	void UnLoadData()
	{
		if (m_isOwnData)
		{
			utility::release_mat_space(m_ppData, (this->m_nRows));
			m_isOwnData = false;
		}
		m_ppData = NULL;
		(this->m_nCols) = -1;
		(this->m_nCols) = -1;
	}

public:
	template <class T2>
	void Convert(Matrix<T2> &mat2)
	{
		mat2.AllocateSpace(this->m_nRows,(this->m_nCols));
		for (int i = 0; i <(this->m_nRows); i++)
		{
			for (int j = 0; j <this->m_nCols; j++)
			{
				mat2[i][j] = (T2)m_ppData[i][j];
			}
		}
	}

public:
	bool IsEmpty() const
	{
		return m_ppData == NULL;
	}
public:
	T Norm(int L) const
	{
		switch(L)
		{
		case 1:
			return NormL1();
		default:
			EXIT_ASSERT(0);
		}
	}

public:
	long long CheckDuplicateRow() const
	{
		long long total =(this->m_nRows)*(this->m_nRows);

		long long count = 0;
#pragma omp parallel for reduction(+ : count) schedule(dynamic)
		for (long long k = 0; k < total; k++)
		{
			int i = k /(this->m_nRows);
			int j = (k % (this->m_nRows));

			if (i >= j)
			{
				continue;
			}

			T dist = L1_distance(m_ppData[i], m_ppData[j],(this->m_nCols));
			if (dist < 1e-3)
			{
				count++;
			}
		}

		return count;
	}

	T** Ptr()
	{
		return m_ppData;
	}

public:
	T SquaredL2Distance(const Matrix<T> &mat) const
	{
		EXIT_ASSERT(mat.Rows() ==(this->m_nRows)&&
			mat.Cols() ==(this->m_nCols));

		//T** pp2 = mat.Ptr();

		T r = 0;
		for (int i = 0; i <(this->m_nRows); i++)
		{
			r += utility::squared_distance(m_ppData[i], mat[i],(this->m_nCols));
		}

		return r;
	}
	void CopyTo(Matrix<T> &to) const
	{
		to.AllocateSpace(this->m_nRows,(this->m_nCols));
		for (int i = 0; i <(this->m_nRows); i++)
		{
			memcpy(to.m_ppData[i], m_ppData[i], sizeof(T) *(this->m_nCols));
		}
		to.m_isOwnData = true;
	}
	void CopyRowsTo(const Vector<int> &vec, Matrix<T>& to) const
	{
		to.AllocateSpace(vec.Size(),(this->m_nCols));
		for (int i = 0; i < vec.Size(); i++)
		{
			int idx = vec[i];
			memcpy(to[i], m_ppData[idx], sizeof(T) *(this->m_nCols)); 
		}
	}
	void SetValueZeros()
	{
		for (int i = 0; i <(this->m_nRows); i++)
		{
			memset(m_ppData[i], 0, sizeof(T) *(this->m_nCols)); 
		}
	}
	void SetValue(T a)
	{
		if (a == T())
		{
			SetValueZeros();
		}
		else
		{
			for (int i = 0; i <(this->m_nRows); i++)
			{
				for (int j = 0; j <this->m_nCols; j++)
				{
					m_ppData[i][j] = a;
				}
			}
		}
	}

public:
	void Col(int j, Vector<T>& vec) const
	{
		vec.AllocateSpace(this->m_nRows);
		for (int i = 0; i <(this->m_nRows); i++)
		{
			vec[i] = m_ppData[i][j];
		}
	}
	void Row(int* p_idx, int num_idx, Matrix<T> &mat) const
	{
		EXIT_ASSERT(num_idx > 0);
		mat.AllocateSpace(num_idx, this->m_nCols);

		for (int i = 0; i < num_idx; i++)
		{
			int idx = p_idx[i];
			for (int j = 0; j <this->m_nCols; j++)
			{
				mat[i][j] = m_ppData[idx][j];
			}
		}
	}
public: // operator
	T* operator [] (int i)
	{
		return m_ppData[i];
	}
	T* operator [] (int i) const
	{
		return m_ppData[i];
	}
	IMatrix<T>& operator = (const IMatrix<T>& a)
	{
		a.CopyTo(*this);
		return (*this);
	}
	IMatrix<T>& operator *= (T v)
	{
		for (int i = 0; i < this->m_nRows; i++)
		{
			T* p = m_ppData[i];
			for (int j = 0; j < this->m_nCols; j++)
			{
				p[j] *= v;
			}
		}
		return *this;
	}

	IMatrix<T>& operator += (const IMatrix<T>& mat)
	{
		for (int i = 0; i < this->m_nRows; i++)
		{
			T* p = m_ppData[i];
			const T* p2 = mat[i];
			for (int j = 0; j < this->m_nCols; j++)
			{
				p[j] += p2[j];
			}
		}
		return *this;
	}

public:
	void Mean(int dim, Vector<T>& mean)
	{
		if (dim == 1)
		{
			Mean1(mean);
		}
	}

public:
	void CalculateMeanVariance(int dim, Vector<T>& mean, Vector<T>& var)
	{
		EXIT_ASSERT(dim == 1);

		mean.AllocateSpace(this->m_nCols);
		mean.SetValueZeros();

		var.AllocateSpace(this->m_nCols);
		var.SetValueZeros();
		for (int j = 0; j <this->m_nCols; j++)
		{
			for (int i = 0; i <this->m_nRows; i++)
			{
				mean[j] += m_ppData[i][j];
				var[j] += m_ppData[i][j] * m_ppData[i][j];
			}
		}
		for (int j = 0; j <this->m_nCols; j++)
		{
			var[j] /= this->m_nRows;
			mean[j] = mean[j] / this->m_nRows;
			var[j] -= mean[j] * mean[j];
		}
	}
	T Distance(const Matrix<T>& mat, int norm_type) const
	{
		switch (norm_type)
		{
		case 1:
			return DistanceL1(mat);
		default:
			EXIT_ASSERT(0);
		}
	}
	void AddcB(T c, const Matrix<T>& B)
	{
		for (int i = 0; i < this->m_nRows; i++)
		{
			for (int j = 0; j < this->m_nCols; j++)
			{
				m_ppData[i][j] += c * B.m_ppData[i][j];
			}
		}
	}
	void AddcB_MP(T c, const Matrix<T>& B)
	{
#pragma omp parallel for
		for (int i = 0; i < this->m_nRows; i++)
		{
			for (int j = 0; j < this->m_nCols; j++)
			{
				m_ppData[i][j] += c * B.m_ppData[i][j];
			}
		}
	}

	void AssignMulti(const Matrix<T>& matA, const Matrix<T> &matBT)
	{
		AllocateSpace(matA.Rows(), matBT.Rows());

		#pragma omp parallel for
		for (int s = 0; s < (this->m_nCols) * this->m_nCols; s++)
		{
			int i = s / this->m_nCols;
			int j = (s % this->m_nCols);
	
			double r = 0.0;
			for (int k = 0; k < matA.Cols(); k++)
			{
				r += matA[i][k] * matBT[j][k];
			}
			m_ppData[i][j] = r;
		}
	}

	void Multiple(T* p, T* out) const
	{
		for (int i = 0; i < this->m_nRows; i++)
		{
			(*out++) = dot(m_ppData[i], p, this->m_nCols);
		}
	}

	void Divide(double d)
	{
		for (int i = 0; i < this->m_nRows; i++)
		{
			T* p = m_ppData[i];
			for (int j = 0; j < this->m_nCols; j++)
			{
				p[j] /= d;
			}
		}
	}
	void Add(const Matrix<T> &mat)
	{
		//T** pp2 = mat.Ptr();
		for (int i = 0; i < this->m_nRows; i++)
		{
			T* p2 = mat[i];
			T* p = m_ppData[i];
			for (int j = 0; j < this->m_nCols; j++)
			{
				p[j] += p2[j];
			}
		}
	}

	int Rows() const
	{
		return this->m_nRows;
	}
	int Cols() const
	{
		return this->m_nCols;
	}

private:
	T NormL1() const
	{
		T s = 0;
		for (int i = 0; i < this->m_nRows; i++)
		{
			s += utility::L1_norm(m_ppData[i], this->m_nCols);
		}
		return s;
	}
	T DistanceL1(const Matrix<T>& mat) const
	{
		T s = 0;
		for (int i = 0; i < this->m_nRows; i++)
		{
			T* pData = m_ppData[i];
			T* pData2 = mat.m_ppData[i];
			for (int j = 0; j < this->m_nCols; j++)
			{
				T diff = pData[j] - pData2[j];
				s += diff > 0 ? diff : -diff;
			}
		}
		return s;
	}
	void Mean1(Vector<T> &mean)
	{
		mean.AllocateSpace(this->m_nCols);
		mean.SetValueZeros();

		for (int i = 0; i < this->m_nRows; i++)
		{
			for (int j = 0; j < this->m_nCols; j++)
			{
				mean[j] += m_ppData[i][j];
			}
		}

		mean /= this->m_nRows;
	}
private:
	T** m_ppData;
	
	bool m_isOwnData;
};

template < class T >
ostream& operator << (ostream &os, const Matrix<T>& mat)
{
	os << "Matrix: " << mat.m_nCols << " x " << mat.m_nCols << "\n";
	for (int i = 0; i < mat.m_nRows; i++)
	{
		for (int j = 0; j < mat.m_nCols; j++)
		{
			os << mat.m_ppData[i][j] << "\t";
		}
		os << "\n";
	}
	return os;
}

template < class T1, class T2 >
ostream& operator << (ostream &os, const pair<T1, T2>& p)
{
	os << "(" << p.first << ", " << p.second << ")";
	return os;
}


}
