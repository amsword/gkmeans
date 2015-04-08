
#pragma once
#include "utility.h"
using namespace utility;
class Reconstructor
{
public:
	void Recon(const Vector<SMatrix<double> > &vecmatDictionary,
		const SMatrix<short> &matCodes,
		SMatrix<double> &matRecovered);

	void Recon(const Vector<SMatrix<double> > &vecmatDictionary,
		const SMatrix<unsigned char> &matCodes,
		SMatrix<double> &matRecovered);

private:
	void ReconOne(const Vector<SMatrix<double> > &vecmatDictionary, 
		const short* p_code, 
		double* p_recovered);

	void ReconOne(const Vector<SMatrix<double> > &vecmatDictionary, 
		const unsigned char* p_code, 
		double* p_recovered);

private:
	int CollectDimension(
		const Vector<SMatrix<double> > &vecmatDictionary);


	int m_nNumberSubDictionaryEachPartition;
	int m_nSubDicSizeEachPartition;

};
