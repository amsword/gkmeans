#include "PQDistance.h"

PQDistanceSS::PQDistanceSS()
{

}
PQDistanceSS::~PQDistanceSS()
{

}

double PQDistanceSS::Distance(
			const void* p_query, const void* p_right) const
{
	double r = 0;
	const short* query = (const short*)p_query;
	const short* right = (const short*)p_right;

	for (int i = 0; i < m_nNumberDictionary; i++)
	{
		short idx1 = query[i];
		short idx2 = right[i];
		r += m_VecMatLookup[i][idx1][idx2];
	}
	return r;
}
void PQDistanceSS::Initialize(const Vector<SMatrix<double> >* p_vec_mat_dic)
{
	m_pVecMatDic = p_vec_mat_dic;
	m_nNumberDictionary = m_pVecMatDic->Size();

	m_VecMatLookup.AllocateSpace(m_nNumberDictionary);
	for (int i = 0; i < m_nNumberDictionary; i++)
	{
		const SMatrix<double>& dictionary = m_pVecMatDic->operator[](i);
		int dic_size = dictionary.Rows();
		int dim = dictionary.Cols();

		SMatrix<double> &lookup = m_VecMatLookup[i];
		lookup.AllocateSpace(dic_size, dic_size);
		for (int r = 0; r < dic_size; r++)
		{
			for (int c = r + 1; c < dic_size; c++)
			{
				lookup[r][c] = squared_distance(dictionary[r], dictionary[c], dim);
			}
		}
		for (int r = 0; r < dic_size; r++)
		{
			lookup[r][r] = 0;
		}
		for (int r = 0; r < dic_size; r++)
		{
			for (int c = 0; c < r; c++)
			{
				lookup[r][c] = lookup[c][r];
			}
		}
	}
}
