#include "Dense8Distance.h"

using namespace utility;

void Dense8Distance::Initialize(const SMatrix<double>* p_mat_lookup)
{
	m_pmatLookup = p_mat_lookup;

	m_nNumberBucketEachPartition = 256;
	m_nNumberDictionary = m_pmatLookup->Rows() / 256;

	SMART_ASSERT((m_pmatLookup->Rows() % 256) == 0)(m_pmatLookup->Rows()).Exit();
}

bool Dense8Distance::IsPreprocessingQuery() const
{
	return true;
}

void Dense8Distance::PreProcessing(const void* p_query, void* &p_pre_out) const
{
	p_pre_out = new Vector<double>();

	const int total_number = m_pmatLookup->Rows();

	Vector<double>* p_lookup = (Vector<double>*)p_pre_out;
	p_lookup->AllocateSpace(total_number);
	p_lookup->SetValueZeros();

	const uchar* p_uchar_query = (const uchar*)p_query;

	for (int i = 0; i < m_nNumberDictionary; i++)
	{
		int idx = p_uchar_query[i];
		
		const double* p_from = m_pmatLookup->operator[](idx + i * m_nNumberBucketEachPartition);
		double* p_to = p_lookup->Ptr();

		for (int j = 0; j < total_number; j++)
		{
			(*p_to++) += (*p_from++);
		}
	}
}
double Dense8Distance::DistancePre(void* p_query, const void* p_right) const
{
	const Vector<double>* p_lookup = (const Vector<double>*)p_query;
	const uchar* p_uchar_right = (const uchar*)p_right;

	double r = 0;
	for (int i = 0; i < m_nNumberDictionary; i++)
	{
		int idx = *p_uchar_right++;
		r += p_lookup->operator[](i * m_nNumberBucketEachPartition + idx);
	}
	return r;
}
void Dense8Distance::PostProcessing(void* &p_pre_out) const
{
	if (p_pre_out)
	{
		delete p_pre_out;
		p_pre_out = NULL;
	}
}
