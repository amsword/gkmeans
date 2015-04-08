#pragma once

#include "utility.h"

#define MAX_SPARSITY 8
#define MAX_NUM_CANDIDATE 20

using namespace utility;

/*
contraint type 0: k-means
constraint type 1: change the cardinality of the vector to be c
constraint type 2: change the cardinality of the vector to be c. partition-based
*/
class ObjectiveValue
{
public:
	ObjectiveValue();

	double Compute(const SMatrix<double> &matZ, const Vector<SMatrix<double> > &vecmatDictionary,
		const SMatrix<short> &matRepresentation);

	double Compute(const SMatrix<double> &matZ, const Vector<SMatrix<double> > &vecmatDictionary,
		const SMatrix<unsigned char> &matRepresentation);

private:
	double Compute(const double* pz, const Vector<SMatrix<double> > &vecmatDictionary,
		const short* pcode);

	double Compute(const double* pz, const Vector<SMatrix<double> > &vecmatDictionary,
		const unsigned char* pcode);

private:
	int m_nNumberDictionaryEachPartition;
	int m_nSubDicSizeEachPartition;
	//const Vector<SMatrix<double> >* m_pvecMatDictionary;
};
