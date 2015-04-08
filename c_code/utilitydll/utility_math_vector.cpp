#include <fstream>
#include "utility_math_vector.h"

namespace utility
{
	template <> void Vector<Vector<pair<int, double> > >::SaveData(const string &file_name) const
	{
		FILE* fp = fopen(file_name.c_str(), "wb");
		SMART_ASSERT(fp)(file_name).Exit();

		int rows = m_nCount;
		int cols = rows; // useless often

		fwrite(&rows, sizeof(int), 1, fp);
		fwrite(&cols, sizeof(int), 1, fp);

		for (int i = 0; i < rows; i++)
		{
			const Vector<pair<int, double> > &vec = m_pData[i];

			int s = vec.Size();
			fwrite(&s, sizeof(int), 1, fp);
			for (int j = 0; j < vec.Size(); j++)
			{
				fwrite(&(vec[j].first), sizeof(int), 1, fp);
			}
			for (int j = 0; j < vec.Size(); j++)
			{
				fwrite(&(vec[j].second), sizeof(double), 1, fp);
			}
		}

		fclose(fp);
	}

	template<> void Vector<Vector<pair<int, double> > >::LoadData(const string &file_name)
	{
		FILE* fp = fopen(file_name.c_str(), "rb");
		SMART_ASSERT(fp)(file_name).Exit();

		int rows;
		int cols;
		fread(&rows, sizeof(int), 1, fp);
		fread(&cols, sizeof(int), 1, fp);

		AllocateSpace(rows);

		for (int i = 0; i < rows; i++)
		{
			int s;
			fread(&s, sizeof(int), 1, fp);

			Vector<pair<int, double> > &vec = m_pData[i];

			vec.AllocateSpace(s);

			for (int j = 0; j < s; j++)
			{
				fread(&(vec[j].first), sizeof(int), 1, fp);
			}
			for (int j = 0; j < s; j++)
			{
				fread(&(vec[j].second), sizeof(double), 1, fp);
			}
		}

		fclose(fp);
	}
}
