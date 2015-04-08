#pragma once

#include <string>
#include <omp.h>

#include "utility_search.h"

#include "utility_macro.h"

#include "utility_types.h"

namespace utility
{
	using namespace std;
	//int binary_search_range(int target, int* p_inc_data, int num);

	//the class will not hold the raw data, but the size information and where the data exist
	// the consideration is the data may be saved in different files, and this class can be 
	// regarded as an intermediate layer, which makes the users feel that only one data file is
	// existed and all of the data are 'loaded'
	class VirtualData
	{
	public:
		VirtualData();
		~VirtualData();

	public:
		// the file_name is an ASCII file, which contains the information of the raw data (where they
		// are, etc.
		void InitializeLarge(const string& file_name);
		void InitializeSmall(const string& file_name);

		// the following three are not recommended
		//void CopyData(float* p_data, int idx_start, int count) const;
		//void CopyData(float** pp_data, int idx_start, int count) const; //the data is not continuously saved
		//void CopyData(int** pp_data, int idx_start, int count) const; //the data is not continuously saved

		void FindWhere(int idx_global, string& file_name, int &idx_in_file) const;

		void SetDataType(const string& str_data_type);

		template<class T>
		void CopyTData(T* p_data, int idx_start, int count, bool is_show_info = false) const;

		template < class T >
		void CopyTData(T** pp_data, int idx_start, int count) const; //the data is not continuously saved

		int Rows() const { return m_nNumber; }
		int Cols() const { return m_nDimension; }
		int GetDimension() const { return m_nDimension; }
		int GetPointNumber() const { return m_nNumber; }
	private:
		VirtualData & operator = (const VirtualData &rhs) { } // it is not allowed
	private:
		int* m_pIndexStart;
		string* m_pFileNames;
		int m_nFiles;

		int m_nNumber;
		int m_nDimension;
	};


	template <class T>
	void VirtualData::CopyTData(T* p_data, int idx_start, int count, bool is_show_info) const
	{
		EXIT_ASSERT(idx_start >= 0 && idx_start + count - 1 < m_nNumber);

		EXIT_ASSERT(count > 0);

		int file_idx_start = 
			utility::binary_search_range(idx_start, m_pIndexStart, m_nFiles);

		while(count > 0)
		{
			int data_idx_start = m_pIndexStart[file_idx_start];


			FILE* fp = fopen(m_pFileNames[file_idx_start].c_str(), "rb");
			//cout << m_pFileNames[file_idx_start].c_str() << "\n";
			EXIT_ASSERT(fp);

			int m, d;
			fread(&m, sizeof(int), 1, fp);
			EXIT_ASSERT(m > 0);

			fread(&d, sizeof(int), 1, fp);
			EXIT_ASSERT(d == m_nDimension);

			fseek(fp, sizeof(T) * d * (idx_start - data_idx_start), SEEK_CUR);

			int curr_count = m - idx_start + data_idx_start;

			if (curr_count > count)
			{
				curr_count = count;
			}

			fread(p_data, sizeof(T), curr_count * d, fp);

			if (is_show_info)
			{
				cout << "Reading data from : " << m_pFileNames[file_idx_start] << "\n"
					<< "\tStart index in this file: " << idx_start - data_idx_start << "\n"
					<< "\tRead " << curr_count << " points in this file\n";
			}

			count -= curr_count;
			idx_start += curr_count;
			p_data += curr_count * d;		

			fclose(fp);



			file_idx_start++;



		}
	}

	template <class T>
	void VirtualData::CopyTData(T** pp_data, int idx_start, int count) const
	{
		EXIT_ASSERT(idx_start >= 0 && idx_start + count - 1 < m_nNumber);

		EXIT_ASSERT(count > 0);

		int file_idx_start = 
			utility::binary_search_range(idx_start, m_pIndexStart, m_nFiles);

		int idx_pp_data = 0;
		while(count > 0)
		{
			//PRINT << idx_pp_data << "\n";

			int data_idx_start = m_pIndexStart[file_idx_start];

			FILE* fp = fopen(m_pFileNames[file_idx_start].c_str(), "rb");
			EXIT_ASSERT(fp);

			int m, d;
			fread(&m, sizeof(int), 1, fp);
			EXIT_ASSERT(m > 0);

			fread(&d, sizeof(int), 1, fp);
			EXIT_ASSERT(d == m_nDimension);

			fseek(fp, sizeof(T) * d * (idx_start - data_idx_start), SEEK_CUR);

			int curr_count = m - idx_start + data_idx_start;

			if (curr_count > count)
			{
				curr_count = count;
			}

			for (int i = 0; i < curr_count; i++)
			{
				fread(pp_data[idx_pp_data++], sizeof(T), d, fp);
			}

			count -= curr_count;
			idx_start += curr_count;
			fclose(fp);

			file_idx_start++;
		}
	}
}
