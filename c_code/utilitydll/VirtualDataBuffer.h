#pragma once

#include <string>
#include "VirtualDataT.h"
#include "utility_math_smatrix.h"

using namespace std;
namespace utility
{
#define MAX_GIGABYTE 0.2
	template <class T>
	class VirtualDataBuffer
	{
	public:

		void Initialize(const VirtualDataT<T> *pvirtual_data)
		{
			m_nIndexStart = 0;
			m_nIndexEnd = 0;
			m_pvirtual_data = pvirtual_data;
		}

		int Rows() const
		{
			return m_pvirtual_data->Rows();
		}
		
		int Cols() const
		{
			return m_pvirtual_data->Cols();
		}

		T* operator [] (int idx)
		{
			if (m_nIndexStart <= idx && m_nIndexEnd > idx)
			{
				return m_matBuffered[idx - m_nIndexStart];
			}
			else
			{
				int num_left = m_pvirtual_data->Rows() - idx;
				int max_read = MAX_GIGABYTE * 1024 * 1024 * 1024 / sizeof(T) / m_pvirtual_data->Cols();
				int num_read = min(num_left, max_read);

				if (num_read == 0)
				{
					return NULL;
				}

				m_nIndexStart = idx;
				m_nIndexEnd = idx + num_read;
				
				m_matBuffered.AllocateSpace(num_read, m_pvirtual_data->Cols());

				m_pvirtual_data->CopyTData(
					m_matBuffered.Ptr(),
					m_nIndexStart, num_read);

				return m_matBuffered[0];
			}
		}


	private:
		SMatrix<T> m_matBuffered;
		int m_nIndexStart;
		int m_nIndexEnd;

		const VirtualDataT<T>* m_pvirtual_data;
	};
};
