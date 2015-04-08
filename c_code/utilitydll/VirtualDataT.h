#pragma once

#include <string>
#include <omp.h>

#include "utility_search.h"

#include "utility_macro.h"

#include "utility_types.h"

namespace utility
{
	using namespace std;

	template <class T>
	class VirtualDataT
	{
	public:
		VirtualDataT();
		~VirtualDataT();

	public:
		void InitializeLarge(const string& file_name);
		void InitializeSmall(const string& file_name);

		void CopyTData(T* p_data, int idx_start, int count, bool is_show_info = false) const;

		int Rows() const { return m_nNumber; }
		int Cols() const { return m_nDimension; }

	private:
		int* m_pIndexStart;
		string* m_pFileNames;
		int m_nFiles;

		int m_nNumber;
		int m_nDimension;
	};


	template <class T>
	void VirtualDataT<T>::CopyTData(T* p_data, int idx_start, int count, bool is_show_info) const
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
	VirtualDataT<T>::VirtualDataT()
	{
		m_pFileNames = 0;
		m_pIndexStart = 0;
	}

	template <class T>
	VirtualDataT<T>::~VirtualDataT()
	{
		if (m_pFileNames)
		{
			delete[] m_pFileNames;
			m_pFileNames = 0;
		}

		if (m_pIndexStart)
		{
			delete[] m_pIndexStart;
			m_pIndexStart = 0;
		}
	}

	template <class T>
	void VirtualDataT<T>::InitializeLarge(const string& file_name)
	{
		FILE* fp = fopen(file_name.c_str(), "r");
		EXIT_ASSERT(fp);

		char buffer[1024];

		fscanf(fp, "%s\n", buffer);
		string str_folder_name = string(buffer);
		if (str_folder_name[str_folder_name.size() - 1] != '\\' &&
			str_folder_name[str_folder_name.size() - 1] != '/')
		{
			str_folder_name = str_folder_name + string("\\");
		}

		fscanf(fp, "%d\t%d\n", &m_nNumber, &m_nDimension);
		fscanf(fp, "%d\n", &m_nFiles);

		m_pFileNames = new string[m_nFiles];
		EXIT_ASSERT(m_pFileNames);

		m_pIndexStart = new int[m_nFiles];
		EXIT_ASSERT(m_pFileNames);

		int pre_idx_end = 0;
		for (int i = 0; i < m_nFiles; i++)
		{
			int idx_start;
			int idx_end;
			fscanf(fp, "%d\t%d\t%s\n", &idx_start, &idx_end, buffer);

			EXIT_ASSERT(idx_start == pre_idx_end);
			EXIT_ASSERT(idx_end > idx_start);
			pre_idx_end = idx_end;
			m_pIndexStart[i] = idx_start;
			m_pFileNames[i] = str_folder_name + string(buffer);
		}
		fclose(fp);
	}

	template <class T>
	void VirtualDataT<T>::InitializeSmall(const string& raw_data_file_name)
	{
		m_nFiles = 1;

		m_pFileNames = new string[m_nFiles];
		EXIT_ASSERT(m_pFileNames);
		m_pFileNames[0] = raw_data_file_name;

		m_pIndexStart = new int[m_nFiles];
		EXIT_ASSERT(m_pFileNames);
		m_pIndexStart[0] = 0;

		FILE* fp = fopen(raw_data_file_name.c_str(), "rb");
		EXIT_ASSERT(fp);

		fread(&m_nNumber, sizeof(int), 1, fp);
		fread(&m_nDimension, sizeof(int), 1, fp);
		fclose(fp);
	}
}
