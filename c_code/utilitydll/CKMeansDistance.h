#pragma once

#include "BinaryCodePartition.h"
#include "DistanceCalculator.h"

using namespace utility;


class UTILITY_DLL CKMeansDistance : public DistanceCalculator
{
public:
	void Initialize(
		const SMatrix<double>* pmatAux,
		const Vector<SMatrix<double> >* pvecmatCenters,
		int code_length,
		int num_raw_feature_partitions,
		int num_binary_partitions);

public:
	virtual bool IsPreprocessingQuery() const;

	virtual void PreProcessing(
		const void* p_query, 
		void* &p_pre_out) const;
	
	virtual double DistancePre(
		void* p_query, 
		const void* p_right) const;
	
	virtual void PostProcessing(void* &p_pre_out) const;

private:
	struct QueryPreprocessing
	{
		Vector<double> vec_lookup;
		Vector<uint64> vec_space;
	};

private:
	int m_nNumberBinaryPartitions;
	int m_nNumberRawFeaturePartitions;
	int m_nCodeLength;

	const SMatrix<double>* m_pmatRotation;
	const Vector<SMatrix<double> >* m_pvecmatCenters;

	int m_nDimension;

	BinaryCodePartition m_bp;
	Vector<int> m_vecBucketNumber;
};

