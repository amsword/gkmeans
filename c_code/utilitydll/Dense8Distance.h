#pragma once
#include "utility_math.h"

#include "DistanceCalculator.h"

using namespace utility;

class UTILITY_DLL Dense8Distance : public DistanceCalculator
{
public:
	void Initialize(const SMatrix<double>* p_mat_lookup);

public:
	virtual bool IsPreprocessingQuery() const;

	virtual void PreProcessing(const void* p_query, void* &p_pre_out) const;
	virtual double DistancePre(void* p_query, const void* p_right) const;
	virtual void PostProcessing(void* &p_pre_out) const;
private:
	const SMatrix<double>* m_pmatLookup;
	int m_nNumberDictionary;
	int m_nNumberBucketEachPartition;
};

