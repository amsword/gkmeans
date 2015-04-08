#include "DiagonalDistance.h"
#include "BinaryCodePartition.h"

bool DiagonalDistance::IsPreprocessingQuery() const
{
	return true;
}

double DiagonalDistance::DistancePre(void* p_query, const void* p_right) const
{
	double r = 0;
	uint64* query = (uint64*)p_query;
	uint64* right = query + m_nNumberPartitions;

	m_bp.SplitPartition(
		(const uchar*)p_right,
		m_nCodeLength,
		m_nNumberPartitions,
		right);

	for (int i = 0; i < m_nNumberPartitions; i++)
	{
		int idx1 = query[i];
		int idx2 = right[i];
		const SMatrix<double>& mat = m_pVecMatLookup->operator[](i);
		r += mat[idx1][idx2];
	}

	//SMART_ASSERT(0)(r);
	//static int k = 0;
	//m_vecDistDebug[k++] = r;
		
	return r;
}

void DiagonalDistance::PreProcessing(const void* p_query, void* &p_pre_out) const
{
	p_pre_out = new uint64[m_nNumberPartitions + m_nNumberPartitions];

	uint64 *p_vec = (uint64*)p_pre_out;

	//SMART_ASSERT(0)(m_nNumberPartitions);
	//SMART_ASSERT(0)(m_nCodeLength);

	m_bp.SplitPartition(
		(const uchar*)p_query, 
		m_nCodeLength,
		m_nNumberPartitions,
		p_vec);

	//SMART_ASSERT(0)(Vector<uint64>(p_vec, m_nNumberPartitions));

}

void DiagonalDistance::PostProcessing(void* &p_pre_out) const
{
	if (p_pre_out)
	{
		delete[] (uint64*)p_pre_out;
		p_pre_out = NULL;
	}
	//m_vecDistDebug.SaveData("vec_r");
}

void DiagonalDistance::Initialize(
	const Vector<SMatrix<double> >* p_vec_mat_lookup, int num_partitions, int code_length)
{
			//m_vecDistDebug.AllocateSpace(1.0e6);
	m_pVecMatLookup = p_vec_mat_lookup;

	m_nCodeLength = code_length;
	m_nNumberPartitions = num_partitions;


	SMART_ASSERT(m_nNumberPartitions == m_pVecMatLookup->Size())
		(m_nNumberPartitions)(m_pVecMatLookup->Size()).Exit();

	Vector<int> vec_buckets(m_nNumberPartitions);
	m_bp.SplitEachPartitionLength(m_nCodeLength,
		m_nNumberPartitions,
		vec_buckets.Ptr());

	for (int i = 0; i < m_nNumberPartitions; i++)
	{
		vec_buckets[i] = (1 << vec_buckets[i]);
	}
	for (int i = 0; i < m_nNumberPartitions; i++)
	{
		SMART_ASSERT(vec_buckets[i] == m_pVecMatLookup->operator[](i).Rows() &&
			vec_buckets[i] == m_pVecMatLookup->operator[](i).Cols())
			(i)(vec_buckets[i])(m_pVecMatLookup->operator[](i).Rows())
			(m_pVecMatLookup->operator[](i).Cols()).Exit();
	}
}
