#pragma once
#include "utility_math.h"

#include "DistanceCalculator.h"

using namespace utility;
class UTILITY_DLL PQDistanceSS : public DistanceCalculator
{
public:
	PQDistanceSS();
	~PQDistanceSS();

public:
	void Initialize(const Vector<SMatrix<double> >* p_vec_mat_dic);

public:
	double Distance(
			const void* p_query, const void* p_right) const;


private:
	const Vector<SMatrix<double> >* m_pVecMatDic;
	Vector<SMatrix<double> > m_VecMatLookup;
	int m_nNumberDictionary;
	short* m_pQuery;
};

