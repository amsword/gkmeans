#pragma once
#include "utility_math.h"

#include "DistanceCalculator.h"
#include "BinaryCodePartition.h"

using namespace utility;

class UTILITY_DLL DiagonalDistance : public DistanceCalculator
{
public:
	void Initialize(const Vector<SMatrix<double> >* p_vec_mat_lookup, 
		int num_partitions, 
		int code_length);

public:
	bool IsPreprocessingQuery() const;

	virtual void PreProcessing(const void* p_query, void* &p_pre_out) const;
	virtual double DistancePre(void* p_query, const void* p_right) const;
	virtual void PostProcessing(void* &p_pre_out) const;

private:
	//Vector<double> m_vecDistDebug;
	const Vector<SMatrix<double> >* m_pVecMatLookup;

	int m_nCodeLength;
	int m_nNumberPartitions;

	BinaryCodePartition m_bp;
};

