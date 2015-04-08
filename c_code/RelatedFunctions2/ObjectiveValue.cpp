#include <cfloat>
#include "ObjectiveValue.h"

ObjectiveValue::ObjectiveValue()
{
}

double ObjectiveValue::Compute(
	const double* pz, const Vector<SMatrix<double> > &vecmatDictionary,
	const short* pcode)
{
	double s =0;
	if (m_nNumberDictionaryEachPartition == 1)
	{
		int num_partition = vecmatDictionary.size();
		for (int i = 0; i < num_partition; i++)
		{
			const SMatrix<double> &matDictionary = vecmatDictionary[i];
			int dim = matDictionary.Cols();

			const double* pword = matDictionary[*pcode++];
			s += squared_distance(pz, pword, dim);
			pz += dim;
		}
	}
	else
	{
		int num_partition = vecmatDictionary.size();

		for (int i = 0; i < num_partition; i++)
		{
			const SMatrix<double> &matDictionary = vecmatDictionary[i];
			int dim = matDictionary.Cols();
			
			
			for (int k = 0; k < dim; k++)
			{
				const short* p_code = pcode;
				double v = *pz++;
				for (int j = 0; j < m_nNumberDictionaryEachPartition; j++)
				{
					const double* pword = matDictionary[*p_code++];
					v -= *(pword + k);
				}
				s += v * v;
			}
			pcode += m_nNumberDictionaryEachPartition;
		}
	}
	return s;
}

double ObjectiveValue::Compute(
	const double* pz, const Vector<SMatrix<double> > &vecmatDictionary,
	const unsigned char* pcode)
{
	double s =0;
	if (m_nNumberDictionaryEachPartition == 1)
	{
		int num_partition = vecmatDictionary.size();
		for (int i = 0; i < num_partition; i++)
		{
			const SMatrix<double> &matDictionary = vecmatDictionary[i];
			int dim = matDictionary.Cols();

			const double* pword = matDictionary[*pcode++];
			s += squared_distance(pz, pword, dim);
			pz += dim;
		}
	}
	else
	{
		int num_partition = vecmatDictionary.size();

		for (int i = 0; i < num_partition; i++)
		{
			const SMatrix<double> &matDictionary = vecmatDictionary[i];
			int dim = matDictionary.Cols();
			
			for (int k = 0; k < dim; k++)
			{
				const unsigned char* p_code = pcode;
				double v = *pz++;
				for (int j = 0; j < m_nNumberDictionaryEachPartition; j++)
				{
					const double* pword = matDictionary[(int)(*p_code++) + j * m_nSubDicSizeEachPartition];
					v -= *(pword + k);
				}
				s += v * v;
			}
			pcode += m_nNumberDictionaryEachPartition;
		}
	}
	return s;
}

double ObjectiveValue::Compute(
	const SMatrix<double> &matZ, 
	const Vector<SMatrix<double> > &vecmatDictionary,
	const SMatrix<short> &matRepresentation)
{
	m_nNumberDictionaryEachPartition = matRepresentation.Cols() / vecmatDictionary.size();
	SMART_ASSERT((matRepresentation.Cols() % vecmatDictionary.size()) == 0).Exit();

	int num_point = matZ.Rows();

	double s = 0;
#pragma omp parallel for reduction(+ : s)
	for (int i = 0; i < num_point; i++)
	{
		const double* pz = matZ[i];
		const short* pcode = matRepresentation[i];

		s += Compute(pz, vecmatDictionary, pcode);
	}

	return s;
}


double ObjectiveValue::Compute(
	const SMatrix<double> &matZ, 
	const Vector<SMatrix<double> > &vecmatDictionary,
	const SMatrix<unsigned char> &matRepresentation)
{
	m_nNumberDictionaryEachPartition = matRepresentation.Cols() / vecmatDictionary.size();
	SMART_ASSERT((matRepresentation.Cols() % vecmatDictionary.size()) == 0).Exit();

	m_nSubDicSizeEachPartition = vecmatDictionary[0].Rows() / m_nNumberDictionaryEachPartition;
	SMART_ASSERT(m_nSubDicSizeEachPartition * m_nNumberDictionaryEachPartition == vecmatDictionary[0].Rows());
	for (int i = 1; i < vecmatDictionary.size(); i++)
	{
		SMART_ASSERT(vecmatDictionary[i].Rows() == vecmatDictionary[0].Rows());
	}

	int num_point = matZ.Rows();

	double s = 0;
#pragma omp parallel for reduction(+ : s)
	for (int i = 0; i < num_point; i++)
	{
		const double* pz = matZ[i];
		const unsigned char* pcode = matRepresentation[i];

		s += Compute(pz, vecmatDictionary, pcode);
	}

	return s;
}
