#pragma once
#include <cstring>
#include <string>
#include "utility_math_basic.h"
#include "utility_math_vector.h"
#include "utility_macro.h"
#include "utility_math_imatrix.h"
#include "utility_memory.h"
using namespace std;

namespace utility
{
	//matrix is not large and the data are stored continiously.
	template < class T >
	class SMatrix : public IMatrix<T>
	{
	public:
		SMatrix()
			:IMatrix<T>(), 
			m_pData(NULL),
			m_isOwnData(false)
		{
		}
		~SMatrix()
		{
			UnLoadData();
		}

		SMatrix(T* p, int rows, int cols)
			: IMatrix<T>(), 
			m_pData(p),
			m_isOwnData(false)
		{
			(this->m_nRows) = rows;
			this->m_nCols = cols;
		}
		SMatrix(const SMatrix<T> &from)
			: IMatrix<T>(), 
			m_pData(NULL),
			m_isOwnData(false)
		{
			from.CopyTo(*this);
		}

		SMatrix(int rows, int cols)
			: IMatrix<T>(), 
			m_pData(NULL),
			m_isOwnData(false)
		{
			AllocateSpace(rows, cols);
		}

	public:
		void AllocateSpace(int m, int n)
		{
			if (m_isOwnData)
			{
				if (m !=(this->m_nRows)|| n != this->m_nCols)
				{
					utility::release_vector_space(m_pData);
					(this->m_nRows) = m;
					this->m_nCols = n;
					utility::allocate_vector_space(m_pData, (long long)m * (long long)n);
					m_isOwnData = true;
				}
			}
			else
			{
				(this->m_nRows) = m;
				this->m_nCols = n;
				utility::allocate_vector_space(m_pData, (long long)m * (long long)n);
				m_isOwnData = true;
			}
		}

		void Attach(T* p, int rows, int cols)
		{
			UnLoadData();

			m_pData = p;
			m_isOwnData = false;
			(this->m_nRows) = rows;
			this->m_nCols = cols;
		}

		void Detatch(T** &p, int &m, int &n)
		{
			SMART_ASSERT(0).Exit();
			p = m_pData;
			m = this->m_nRows;
			n = this->m_nCols;

			m_pData = NULL;
			(this->m_nRows) = 0;
			this->m_nCols = 0;
			m_isOwnData = false;
		}

		void LoadData(const string &file_name)
		{
			FILE* fp = fopen(file_name.c_str(), "rb");
			SMART_ASSERT(fp)(file_name).Exit();

			BinaryRead(fp);

			fclose(fp);
		}

		void LoadData(const string &file_name, int start, int end)
		{
			FILE* fp = fopen(file_name.c_str(), "rb");
			SMART_ASSERT(fp)(file_name).Exit();

			BinaryRead(fp, start, end);

			fclose(fp);
		}

		void LoadDataToHomogeneous(const string &file_name)
		{
			FILE* fp = fopen(file_name.c_str(), "rb");
			EXIT_ASSERT(fp);
			int m;
			int n;
			SMART_ASSERT(fread(&m, sizeof(int), 1, fp) == 1).Exit();
			fread(&n, sizeof(int), 1, fp);

			AllocateSpace(m, n + 1);

			for (int i = 0; i < this->m_nRows; i += n)
			{
				fread(m_pData + i, sizeof(T), n, fp);
				SMART_ASSERT(0)("NOTE HERE").Exit();
				//m_pData[i + n] = 1;
			}
			fclose(fp);
		}

		void SaveData(const string &file_name) const
		{
			EXIT_ASSERT((this->m_nRows) > 0 && this->m_nCols > 0);

			FILE* fp = fopen(file_name.c_str(), "wb");
			EXIT_ASSERT(fp);

			BinaryWrite(fp);

			fclose(fp);
		}

		void BinaryWrite(FILE* fp) const
		{
			fwrite(&(this->m_nRows), sizeof(int), 1, fp);
			fwrite(&(this->m_nCols), sizeof(int), 1, fp);

			fwrite(m_pData, sizeof(T),(this->m_nRows)* this->m_nCols, fp);
		}

		void BinaryRead(FILE* fp)
		{
			int r;
			int m;
			int n;
			r = fread(&m, sizeof(int), 1, fp);
			SMART_ASSERT(r == 1)(r).Exit();
			r = fread(&n, sizeof(int), 1, fp);
			SMART_ASSERT(r == 1)(r).Exit();

			AllocateSpace(m, n);

			r = fread(m_pData, sizeof(T), n * m, fp);
			SMART_ASSERT(r == n * m)(r).Exit();
		}

		void BinaryRead(FILE* fp, int row_start, int row_end)
		{
			int m;
			int n;
			fread(&m, sizeof(int), 1, fp);
			fread(&n, sizeof(int), 1, fp);

			int real_start = max(0, row_start);
			int real_end = min(m, row_end);
			if (real_end <= real_start)
			{
				return;
			}
			
			AllocateSpace(real_end - real_start, n);

			fseek(fp, (long long)real_start * (long long)n * sizeof(T), SEEK_CUR);
			fread(m_pData, sizeof(T), n * (real_end - real_start), fp);
		}


		void UnLoadData()
		{
			if (m_isOwnData)
			{
				utility::release_vector_space(m_pData);
				m_isOwnData = false;
			}
			m_pData = NULL;
			(this->m_nRows) = 0;
			this->m_nCols = 0;
		}

	public:
		void Scale()	
		{
			T s = NormL1();
			s /=(this->m_nRows)* this->m_nCols;
			s = 1.0 / s;

			T* p = m_pData;
			for (int i = 0; i <(this->m_nRows)* this->m_nCols; i++)
			{
				(*p) *= s;
				p++;
			}
		}
	public:
		template <class T2>
		void Convert(SMatrix<T2> &mat2)
		{
			mat2.AllocateSpace(this->m_nRows, this->m_nCols);
			for (int i = 0; i <(this->m_nRows)* this->m_nCols; i++)
			{
				mat2.m_pData[i] = (T2)m_pData.m_pData[i];
			}
		}

	public:
		bool IsEmpty() const
		{
			return m_pData == NULL;
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
				return T();
			}
		}

		void NormalizeEveryRowL1()
		{
			for (int i = 0; i < this->m_nRows; i++)
			{
				T* p = operator [](i);
				T v = L1_norm(p, this->m_nCols);
				v = v / this->m_nCols;
				scale_multi_vector(1.0 / v, p, p, this->m_nCols);
			}
		}

	public:
		void CopyTo(SMatrix<T> &to) const
		{
			to.AllocateSpace(this->m_nRows, this->m_nCols);
			memcpy(to.m_pData, m_pData, sizeof(T) * (long long)this->m_nCols * (long long)this->m_nRows);
			to.m_isOwnData = true;
		}
		
		void SetValueZeros()
		{
			memset(m_pData, 0, sizeof(T) *(this->m_nRows)* this->m_nCols); 
		}

		void SetValue(T a)
		{
			for (int i = 0; i <(this->m_nRows)* this->m_nCols; i++)
			{
				m_pData[i] = a;
			}
		}


	public: // operator
		SMatrix<T>& operator = (const SMatrix<T>& a)
		{
			a.CopyTo(*this);
			return (*this);
		}
		SMatrix<T>& operator *= (T v)
		{
			for (int i = 0; i <(this->m_nRows)* this->m_nCols; i++)
			{
				m_pData[i] *= v;
			}
			return *this;
		}

		T* operator[](int i)
		{
			return m_pData + (long long)i * (long long)(this->m_nCols);
		}

		T* operator[](int i) const
		{
			SMART_ASSERT(i >= 0 && i < this->m_nRows);
			return m_pData + (long long)i * (long long)(this->m_nCols);
		}

		T* operator[](long long i) const
		{
			return m_pData + i * this->m_nCols;
		}

		SMatrix<T> operator *(T s)
		{
			SMatrix<T> mat(this->m_nRows, this->m_nCols);
			T* p_result = mat.Ptr();
			T* p_source = m_pData;

			for (int i = 0; i <(this->m_nRows)* this->m_nCols; i++)
			{
				(*p_result++) = (*p_source++) * s;
			}
			return mat;
		}

		SMatrix<T>& operator += (const SMatrix<T>& mat)
		{
			for (int i = 0; i <(this->m_nRows)* this->m_nCols; i++)
			{
				m_pData[i] += mat.m_pData[i];
			}
			return *this;
		}

		SMatrix<T>& operator -= (const SMatrix<T>& mat)
		{
			T* p_mat = mat.m_pData;
			T* p = m_pData;
			for (int i = 0; i <(this->m_nRows)* this->m_nCols; i++)
			{
				(*p) -= (*p_mat++);
				p++;
			}
			return *this;
		}

	public:
		
		T MaxMax() const
		{
			T max_max = -10000;
			for (long long i = 0; i <(this->m_nRows)* this->m_nCols; i++)
			{
				if (m_pData[i] > max_max)
				{
					max_max = m_pData[i];
				}
			}
			return max_max;
		}
	
		void AssignMulti(const IMatrix<T>& matA, const IMatrix<T> &matBT)
		{
			AllocateSpace(matA.Rows(), matBT.Rows());

#pragma omp parallel for
			for (int s = 0; s <(this->m_nRows)* this->m_nCols; s++)
			{
				int i = s / this->m_nCols;
				int j = (s % this->m_nCols);

				double r = 0.0;
				for (int k = 0; k < matA.Cols(); k++)
				{
					r += matA[i][k] * matBT[j][k];
				}
				operator[](i)[j] = r;
			}
		}

		void Add(const IMatrix<T> &mat)
		{
			//T** pp2 = mat.Ptr();
			for (int i = 0; i < this->m_nRows; i++)
			{
				T* p2 = mat[i];
				T* p = m_pData + i * this->m_nCols;
				for (int j = 0; j < this->m_nCols; j++)
				{
					p[j] += p2[j];
				}
			}
		}

		void AddcB(T c, const IMatrix<T>& B)
		{
			for (int i = 0; i < this->m_nRows; i++)
			{
				for (int j = 0; j < this->m_nCols; j++)
				{
					this->operator[](i)[j] += c * B[i][j];
				}
			}
		}

		void Divide(double d)
		{
			T* p = m_pData;
			for (int i = 0; i <(this->m_nRows)* (this->m_nCols); i++)
			{
				(*p++) /= d;
			}
		}

		void Multiple(const T* p, Vector<T> &out) const
		{
			out.AllocateSpace(this->m_nRows);
			Multiple(p, out.Ptr());
		}

		void Multiple(const T* p, T* out) const
		{
			for (int i = 0; i < this->m_nRows; i++)
			{
				(*out++) = dot(operator [](i), p, this->m_nCols);
			}
		}

		void MultipleN(const T* p, T* out) const
		{
			for (int i = 0; i < this->m_nRows; i++)
			{
				(*out++) = -dot(operator [](i), p, this->m_nCols);
			}
		}

		T* Ptr() const
		{
			return m_pData;
		}
		double DistanceL1(const IMatrix<T>& mat) const
		{
			double s = 0;
			for (int i = 0; i < this->m_nRows; i++)
			{
				for (int j = 0; j < this->m_nCols; j++)
				{
					double diff = this->operator[](i)[j] - (double)mat[i][j];
					s += diff > 0 ? diff : -diff;
				}
			}
			return s;
		}
	private:
		T NormL1() const
		{
			T s = 0;
			s += utility::L1_norm(m_pData, this->m_nCols * this->m_nRows);
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
					mean[j] += m_pData[i][j];
				}
			}

			mean /= this->m_nRows;
		}
	private:
		T* m_pData;
		
		bool m_isOwnData;
	};

	template <class T>
	ostream& operator << (ostream& out, const SMatrix<T> &v)
	{
		out << "Matrix: \n";
		out << "Rows = " << v.Rows() << "\n"
			<< "Cols = " << v.Cols() << "\n";

		for (int i = 0; i < v.Rows(); i++)
		{
			for (int j = 0; j < v.Cols(); j++)
			{
				out << v[i][j] << "\t";
			}
			out << "\n";
		}
		out << "\n";
		return out;
	}



}
