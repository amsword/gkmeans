#include "Reconstructor.h"

int Reconstructor::CollectDimension(
	const Vector<SMatrix<double> > &vecmatDictionary)
{
	int dim = 0;
	int num_partitions = vecmatDictionary.size();
	for (int i = 0; i < num_partitions; i++)
	{
		dim += vecmatDictionary[i].Cols();
	}
	return dim;
}


void Reconstructor::ReconOne(
	const Vector<SMatrix<double> > &vecmatDictionary, 
	const short* p_code, 
	double* p_recovered)
{
	int num_partitions = vecmatDictionary.size();

	for (int i = 0; i < num_partitions; i++)
	{
		const SMatrix<double> &matDictionary = vecmatDictionary[i];
		const double* p_word = matDictionary[*p_code++];
		int dim = matDictionary.Cols();
		memcpy(p_recovered, p_word, sizeof(double) * dim);

		for (int j = 1; j < m_nNumberSubDictionaryEachPartition; j++)
		{
			p_word = matDictionary[*p_code++];
			VectorAdd(p_recovered, p_word, dim);
		}

		p_recovered += dim;
	}
}


void Reconstructor::ReconOne(
	const Vector<SMatrix<double> > &vecmatDictionary, 
	const unsigned char* p_code, 
	double* p_recovered)
{
	int num_partitions = vecmatDictionary.size();

	for (int i = 0; i < num_partitions; i++)
	{
		const SMatrix<double> &matDictionary = vecmatDictionary[i];
		const double* p_word = matDictionary[*p_code++];
		int dim = matDictionary.Cols();
		memcpy(p_recovered, p_word, sizeof(double) * dim);

		for (int j = 1; j < m_nNumberSubDictionaryEachPartition; j++)
		{
			p_word = matDictionary[(*p_code++) + j * m_nSubDicSizeEachPartition];
			VectorAdd(p_recovered, p_word, dim);
		}

		p_recovered += dim;
	}
}

void Reconstructor::Recon(
	const Vector<SMatrix<double> > &vecmatDictionary,
	const SMatrix<short> &matCodes,
	SMatrix<double> &matRecovered)
{
	int dim = CollectDimension(vecmatDictionary);

	int num_point = matCodes.Rows();
	SMART_ASSERT(num_point == matRecovered.Rows() &&
		dim == matRecovered.Cols()).Exit();

	SMART_ASSERT((matCodes.Cols() % vecmatDictionary.size()) == 
		0).Exit();

	m_nNumberSubDictionaryEachPartition = matCodes.Cols() / vecmatDictionary.size();


#pragma omp parallel for
	for (int i = 0; i < num_point; i++)
	{
		double* p_recovered = matRecovered[i];
		const short* p_code = matCodes[i];

		ReconOne(vecmatDictionary, p_code, p_recovered);

	}
}


void Reconstructor::Recon(
	const Vector<SMatrix<double> > &vecmatDictionary,
	const SMatrix<unsigned char> &matCodes,
	SMatrix<double> &matRecovered)
{
	int dim = CollectDimension(vecmatDictionary);

	int num_point = matCodes.Rows();
	SMART_ASSERT(num_point == matRecovered.Rows() &&
		dim == matRecovered.Cols()).Exit();

	SMART_ASSERT((matCodes.Cols() % vecmatDictionary.size()) == 
		0).Exit();

	m_nNumberSubDictionaryEachPartition = matCodes.Cols() / vecmatDictionary.size();
	SMART_ASSERT(m_nNumberSubDictionaryEachPartition * vecmatDictionary.size() == matCodes.Cols());

	m_nSubDicSizeEachPartition = vecmatDictionary[0].Rows() / m_nNumberSubDictionaryEachPartition;
	SMART_ASSERT(m_nSubDicSizeEachPartition * m_nNumberSubDictionaryEachPartition == vecmatDictionary[0].Rows());
#pragma omp parallel for
	for (int i = 0; i < num_point; i++)
	{
		double* p_recovered = matRecovered[i];
		const unsigned char* p_code = matCodes[i];

		ReconOne(vecmatDictionary, p_code, p_recovered);

	}
}
