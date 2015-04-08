#include "utility_memory.h"

namespace utility
{
	class BitVector
	{
	public:
		BitVector();
		~BitVector();

	public:
		void Resize(int num_bit);
		void SetZeros();
		void BitSet(int idx);
		int HammingDistance(const BitVector& vec) const;
		void SaveChar(const string &file_name) const;

		bool operator [](int i) const;
		int size() const;

	private:
		void BinarySave(FILE* fp) const;

	private:
		unsigned long long* m_pValue;
		int m_count; // number of ulonglong
		int m_num_bit; // number of valid bit
		const int m_ele_size;
	};
}
