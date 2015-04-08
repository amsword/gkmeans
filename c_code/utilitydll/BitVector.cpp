#include <string>
#include <cstring>
#include <stdio.h>
#include "BitVector.h"
#include "utility_macro.h"

namespace utility
{
	BitVector::BitVector()
		: m_pValue(NULL),
		m_count(0),
		m_ele_size(sizeof(long long) * 8)
	{
	}
	BitVector::~BitVector()
	{
		utility::release_vector_space(m_pValue);
		m_count = 0;
	}

	void BitVector::SetZeros()
	{
		memset(m_pValue, 0, sizeof(long long) * m_count);
	}

	void BitVector::Resize(int min_size)
	{
		m_num_bit = min_size;

		int allocated_count = min_size / m_ele_size;
		if (allocated_count * m_ele_size < min_size)
		{
			allocated_count++;
		}

		if (m_count < allocated_count)
		{
			utility::release_vector_space(m_pValue);
			utility::allocate_vector_space(m_pValue, allocated_count);
			m_count = allocated_count;
		}
	}

	void BitVector::BitSet(int bit_idx)
	{
		SMART_ASSERT(bit_idx >= 0 && bit_idx <= m_count * m_ele_size)(bit_idx).Exit();

		int idx = bit_idx / m_ele_size;
		int sub_num = (bit_idx % m_ele_size);

		unsigned long long* p = m_pValue + idx;
		*p = *p | ((unsigned long long)1 << sub_num);
	}

	void BitVector::BinarySave(FILE* fp) const
	{
		fwrite(&m_num_bit, sizeof(int), 1, fp);
		int tmp = 1;
		fwrite(&tmp, sizeof(int), 1, fp);

		char one = 1;
		char zero = 0;
		for (int i = 0; i < m_num_bit; i++)
		{
			if (operator[](i))
			{
				fwrite(&one, sizeof(char), 1, fp);
			}
			else
			{
				fwrite(&zero, sizeof(char), 1, fp);
			}
		}
	}

	int BitVector::size() const
	{
		return m_num_bit;
	}

	void BitVector::SaveChar(const string &file_name) const
	{
		FILE* fp = fopen(file_name.c_str(), "wb");
		EXIT_ASSERT(fp);

		BinarySave(fp);
		fclose(fp);
	}

	bool BitVector::operator [](int bit_idx) const
	{
		int idx = bit_idx / m_ele_size;
		int sub_num = (bit_idx % m_ele_size);

		unsigned long long* p = m_pValue + idx;
		return (*p & ((unsigned long long)1 << sub_num));
	}

	int BitVector::HammingDistance(const BitVector& vec) const
	{
		SMART_ASSERT(vec.m_count == m_count)(vec.m_count)(m_count).Exit();

		unsigned long long* p1 = m_pValue;
		unsigned long long* p2 = vec.m_pValue;
		int h = 0;
		for (int i = 0; i < m_count; i++)
		{
			SMART_ASSERT(0).Exit(); // the following is commented
			//h += __popcnt64((*p2++) ^ (*p1++));
		}
		return h;
	}
}
