#include "CKMeansDistance.h"

using namespace utility;

void CKMeansDistance::Initialize(
	const SMatrix<double>* pmatRotation,
	const Vector<SMatrix<double> >* pvecmatCenters,
	int code_length,
	int num_raw_feature_partitions,
	int num_binary_partitions)
{
	m_pmatRotation = pmatRotation;
	m_pvecmatCenters = pvecmatCenters;

	m_nNumberRawFeaturePartitions = num_raw_feature_partitions;
	m_nNumberBinaryPartitions = num_binary_partitions;
	m_nCodeLength = code_length;

	SMART_ASSERT(m_nNumberRawFeaturePartitions = m_pvecmatCenters->Size()).Exit();

	m_vecBucketNumber.AllocateSpace(m_nNumberBinaryPartitions);

	m_bp.SplitEachPartitionLength(
		m_nCodeLength, 
		m_nNumberBinaryPartitions, 
		m_vecBucketNumber.Ptr());
	
	//SMART_ASSERT(0)(m_vecBucketNumber);

	for (int i = 0; i < m_nNumberBinaryPartitions; i++)
	{
		m_vecBucketNumber[i] = (1 << m_vecBucketNumber[i]);
	}
	//SMART_ASSERT(0)(m_vecBucketNumber).Exit();

	int m_nDimension = 0;
	for (int i = 0; i < m_nNumberRawFeaturePartitions; i++)
	{
		m_nDimension += m_pvecmatCenters->operator[](i).Cols();
		SMART_ASSERT(m_vecBucketNumber[i] == m_pvecmatCenters->operator[](i).Rows())
			(m_vecBucketNumber[i])
			(m_pvecmatCenters->operator[](i).Rows()).Exit();
	}
	SMART_ASSERT(m_nDimension == m_pmatRotation->Rows() && m_nDimension == m_pmatRotation->Cols()).Exit();
}

bool CKMeansDistance::IsPreprocessingQuery() const
{
	//PRINT << "1\n";
	return true;
}

void CKMeansDistance::PreProcessing(const void* p_query, void* &p_pre_out) const
{
	//PRINT << m_nDimension << "\n"
	//	<< m_nNumberBinaryPartitions << "\n"
	//	<< m_nNumberRawFeaturePartitions << "\n"
	//	<< m_pmatRotation->Rows() << "\n"
	//	<< m_pmatRotation->Cols() << "\n";
	p_pre_out = NULL;
	//SMART_ASSERT(0).Exit();
	//return;

	//const int sub_dim = m_nDimension / m_nNumberRawFeaturePartitions;

	const int total_num = m_vecBucketNumber.SumUp();

	p_pre_out = new QueryPreprocessing();

	Vector<double> &lookup = ((QueryPreprocessing*)p_pre_out)->vec_lookup;
	lookup.AllocateSpace(total_num);

	const double* pf_query = (const double*)p_query;

	Vector<double> vec_rotated(m_nDimension);
	m_pmatRotation->Multiple(pf_query, vec_rotated.Ptr());

	int k = 0;
	int idx_row = 0;
	const double* p_rotated = vec_rotated.Ptr();

	for (int i = 0; i < m_nNumberRawFeaturePartitions; i++)
	{
		SMART_ASSERT(i < m_vecBucketNumber.Size())
			(i < m_pvecmatCenters->Size()).Exit();

		int num_bucket = m_vecBucketNumber[i];

		int sub_dim = m_pvecmatCenters->operator[](i).Cols();
		const SMatrix<double>& mat_centers = m_pvecmatCenters->operator[](i);
		SMART_ASSERT(mat_centers.Rows() == num_bucket &&
			mat_centers.Cols() == sub_dim).Exit();

		for (int j = 0; j < num_bucket; j++)
		{
			SMART_ASSERT(k < lookup.Size()).Exit();
			lookup[k++] = squared_distance(
				p_rotated, 
				mat_centers[j], 
				sub_dim); 
		}
		p_rotated += sub_dim;
	}

	Vector<uint64> &vec_space = ((QueryPreprocessing*)p_pre_out)->vec_space;
	vec_space.AllocateSpace(m_nNumberBinaryPartitions);
	//PRINT << "1\n";
}
double CKMeansDistance::DistancePre(void* p_query, const void* p_right) const
{
	//PRINT << "1\n";

	QueryPreprocessing* p_query_processing = (QueryPreprocessing*)p_query;

	Vector<double> &lookup = p_query_processing->vec_lookup;

	uint64* p_partitions = p_query_processing->vec_space.Ptr();
	m_bp.SplitPartition(
		(const uchar*)p_right,
		m_nCodeLength,
		m_nNumberBinaryPartitions,
		p_partitions);

	int idx_row = 0;
	double r = 0;
	for (int i = 0; i < m_nNumberBinaryPartitions; i++)
	{
		int idx = *p_partitions++;
		r += lookup[idx_row + idx];
		idx_row += m_vecBucketNumber[i];
	}
	//SMART_ASSERT(0)(r).Exit();
	//PRINT << "1\n";
	return r;
}
void CKMeansDistance::PostProcessing(void* &p_pre_out) const
{
	//return;
	//PRINT << "1\n";
	if (p_pre_out)
	{
		delete p_pre_out;
		p_pre_out = NULL;
	}
	//PRINT << "1\n";
}
