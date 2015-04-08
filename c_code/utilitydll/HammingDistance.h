#pragma once
#include "utility_math.h"

#include "DistanceCalculator.h"

using namespace utility;



class UTILITY_DLL HammingDistance: public DistanceCalculator
{
public:
	void Initialize(int code_length_in_byte);

public:
	virtual double Distance(
		const void* p_query, const void* p_right) const;

	//virtual void Preprocessing(const void* p_query, QueryPreprocessing &out) const;
	//virtual double Distance(const QueryPreprocessing &query, 
	//	const void* p_right) const;

private:
	int m_nCodeLengthInByte;
};

