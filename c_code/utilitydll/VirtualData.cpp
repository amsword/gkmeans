#include <iostream>

#include "VirtualData.h"

namespace utility
{
	VirtualData::VirtualData()
	{
		m_pFileNames = 0;
		m_pIndexStart = 0;
	}

	VirtualData::~VirtualData()
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
	
	void VirtualData::InitializeLarge(const string& file_name)
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

	void VirtualData::InitializeSmall(const string& raw_data_file_name)
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

	void VirtualData::FindWhere(int idx_global, string& file_name, int &idx_in_file) const
	{
		EXIT_ASSERT(idx_global >= 0 && idx_global < m_nNumber);

		int file_idx_start = 
			utility::binary_search_range(idx_global, m_pIndexStart, m_nFiles);

		int idx_pp_data = 0;




		int data_idx_start = m_pIndexStart[file_idx_start];
		file_name = m_pFileNames[file_idx_start];
		idx_in_file = idx_global - data_idx_start;
	}



	/*void VirtualData::CopyData(float* p_data, int idx_start, int count) const
	{
		EXIT_ASSERT(idx_start >= 0 && idx_start + count - 1 < m_nNumber);

		EXIT_ASSERT(count > 0);

		int file_idx_start = 
			utility::binary_search_range(idx_start, m_pIndexStart, m_nFiles);

		while(count > 0)
		{
			int data_idx_start = m_pIndexStart[file_idx_start];

			FILE* fp = fopen(m_pFileNames[file_idx_start].c_str(), "rb");
			EXIT_ASSERT(fp);

			int m, d;
			fread(&m, sizeof(int), 1, fp);
			EXIT_ASSERT(m > 0);

			fread(&d, sizeof(int), 1, fp);
			EXIT_ASSERT(d == m_nDimension);

			fseek(fp, sizeof(float) * d * (idx_start - data_idx_start), SEEK_CUR);

			int curr_count = m - idx_start + data_idx_start;

			if (curr_count > count)
			{
				curr_count = count;
			}

			fread(p_data, sizeof(float), curr_count * d, fp);

			count -= curr_count;
			p_data += curr_count * d;		

			fclose(fp);
		}*/
	//}

	//void VirtualData::CopyData(float** pp_data, int idx_start, int count) const
	//{
	//	EXIT_ASSERT(idx_start >= 0 && idx_start + count - 1 < m_nNumber);

	//	EXIT_ASSERT(count > 0);

	//	int file_idx_start = 
	//		utility::binary_search_range(idx_start, m_pIndexStart, m_nFiles);

	//	int idx_pp_data = 0;
	//	while(count > 0)
	//	{
	//		PRINT << idx_pp_data << "\n";

	//		int data_idx_start = m_pIndexStart[file_idx_start];

	//		FILE* fp = fopen(m_pFileNames[file_idx_start].c_str(), "rb");
	//		EXIT_ASSERT(fp);

	//		int m, d;
	//		fread(&m, sizeof(int), 1, fp);
	//		EXIT_ASSERT(m > 0);

	//		fread(&d, sizeof(int), 1, fp);
	//		EXIT_ASSERT(d == m_nDimension);

	//		fseek(fp, sizeof(float) * d * (idx_start - data_idx_start), SEEK_CUR);

	//		int curr_count = m - idx_start + data_idx_start;

	//		if (curr_count > count)
	//		{
	//			curr_count = count;
	//		}

	//		for (int i = 0; i < curr_count; i++)
	//		{
	//			fread(pp_data[idx_pp_data++], sizeof(float), d, fp);
	//		}

	//		count -= curr_count;
	//		fclose(fp);
	//	}
	//}

	//void VirtualData::CopyData(int** pp_data, int idx_start, int count) const
	//{
	//	EXIT_ASSERT(0);
	//}
}
