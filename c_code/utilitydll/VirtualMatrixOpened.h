#pragma once
#include <string>
#include "utility_macro.h"
#include "utility_math_imatrix.h"

using namespace std;

namespace utility
{
	//matrix is not large and the data are stored continiously.
	template < class T >
	class VirtualMatrixOpened : public IMatrix<T>
	{
	public:
		VirtualMatrixOpened()
			:IMatrix<T>(), 
			m_fp(NULL)
		{

		}
		~VirtualMatrixOpened()
		{
			UnLoadData();
		}
		
	public:
	
		void LoadData(const string &file_name)
		{
			m_fp = fopen(file_name.c_str(), "rb");
			SMART_ASSERT(m_fp)(file_name).Exit();
			
			fread(&(this->m_nRows), sizeof(int), 1, m_fp);
			fread(&(this->m_nCols), sizeof(int), 1, m_fp);
		}

		void CopyData(int idx, T* p_data) const
		{
			SMART_ASSERT(sizeof(long int) == 8).Exit();
			fseek(m_fp, (long long)(this->m_nCols) * (long long)idx * sizeof(T) + 8, SEEK_SET);

			fread(p_data, sizeof(T), this->m_nCols, m_fp);
		}

		void UnLoadData()
		{
			if (m_fp)
			{
				fclose(m_fp);
			}
			m_fp = NULL;
			(this->m_nCols) = 0;
			this->m_nCols = 0;
		}

	private:
		FILE* m_fp;
	};

	

}
