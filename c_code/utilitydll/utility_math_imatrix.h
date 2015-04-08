#pragma once
#include <string>
#include "utility_macro.h"
using namespace std;

namespace utility
{
	//matrix is not large and the data are stored continiously.
	
	template <class T>
	class IMatrix
	{
	public:
		IMatrix() :
		  m_nCols(0),
			  m_nRows(0)
		{
		}
		~IMatrix()
		{
			
		}

	public:
		virtual void AllocateSpace(int m, int n) {}
	
		virtual void LoadData(const string &file_name){}

		virtual void LoadDataToHomogeneous(const string &file_name) {}

		virtual void SaveData(const string &file_name) const {}

		virtual void BinaryWrite(FILE* fp) const {}
		virtual void BinaryRead(FILE* fp) {}

		virtual void UnLoadData(){}

	public:
		virtual bool IsEmpty() const { return false;}

	public:
		virtual T Norm(int L) const{ return T(); }

	public:
		virtual void CopyTo(IMatrix &to) const{}
		virtual void SetValueZeros(){}
		virtual void SetValue(T a){}

	public: // operator
		virtual IMatrix& operator = (const IMatrix& a){ return *this; }
		
		virtual IMatrix& operator *= (T v){ return *this;}

		virtual T* operator[](int i) {return NULL; }
		virtual T* operator[](int i) const {EXIT_ASSERT(0); return NULL; }

		virtual IMatrix& operator += (const IMatrix& mat) {return *this;}

	public:
		int Rows() const
		{
			return m_nRows;
		}
		int Cols() const
		{
			return m_nCols;
		}

	protected:
		int m_nRows;
		int m_nCols;

	};
}
