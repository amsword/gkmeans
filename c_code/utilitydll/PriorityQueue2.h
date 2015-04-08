#pragma once

#include <algorithm>
#include "utility_sort.h"

namespace utility
{
	/**
	* Priority Queue Implementation
	*
	* The priority queue is implemented with a m_heap.  A m_heap is a complete
	* (full) binary tree in which each parent is less than both of its
	* children, but the order of the children is unspecified.
	* Note that a m_heap uses 1-based indexing to allow for power-of-2
	* location of parents and children.  We ignore element 0 of m_heap array.
	*/
	template < typename T, typename Comp = less<T> >
	class PriorityQueue 
	{
	public:
		PriorityQueue()
		{
			m_nCount = 0;
			m_heap = NULL;
			m_nLength = 0;
		}

		/**
		* Constructor.
		*
		* Params:
		*     size = m_heap size
		*/

		PriorityQueue(int size)
		{
			m_nLength = size+1;
			m_heap = new T[m_nLength];  // m_heap uses 1-based indexing
			m_nCount = 0;
		}

		/**
		* Destructor.
		*
		*/
		~PriorityQueue()
		{
			release();
		}

		void release()
		{
			if (m_heap != NULL)
			{
				delete[] m_heap;
				m_heap = 0;
				m_nCount = 0;
				m_nLength = 0;
			}
		}

		void Reserve(int size)
		{
			release();
			m_nLength = size+1;
			m_heap = new T[m_nLength];  // m_heap uses 1-based indexing
			m_nCount = 0;
		}

		/**
		*
		* Returns: m_heap size
		*/
		int size()
		{
			return m_nCount;
		}

		/**
		* Tests if the m_heap is empty
		*
		* Returns: true is m_heap empty, false otherwise
		*/
		bool empty()
		{
			return size()==0;
		}

		/**
		* Clears the m_heap.
		*/
		void clear()
		{
			m_nCount = 0;
		}


		/**
		* Insert a new element in the m_heap.
		*
		* We select the next empty leaf node, and then keep moving any larger
		* parents down until the right location is found to store this element.
		*
		* Params:
		*     value = the new element to be inserted in the m_heap
		*/
		void insert(T value)
		{
			EXIT_ASSERT(IsExist(value.first) == false);

			/* If m_heap is full, re-allocate space. */
			if (m_nCount == m_nLength-1) 
			{
				int length2 = m_nLength * 2;
				T* heap2 = new T[length2];
				for (int i = 0; i < m_nLength; i++)
				{
					heap2[i] = m_heap[i];
				}

				m_nLength = length2;
				delete[] m_heap;
				m_heap = heap2;
								
				m_nLength = length2;
			}

			int loc = ++(m_nCount);   /* Remember 1-based indexing. */

			/* Keep moving parents down until a place is found for this node. */
			int par = loc / 2;                 /* Location of parent. */
			while (par > 0  && m_comp(value, m_heap[par])) {
				m_heap[loc] = m_heap[par];     /* Move parent down to loc. */
				loc = par;
				par = loc / 2;
			}
			/* Insert the element at the determined location. */
			m_heap[loc] = value;
		}

		bool IsExist(int i);

		bool popMin()
		{
			if (m_nCount == 0) 
			{
				return false;
			}

			/* Switch first node with last. */
			swap(m_heap[1],m_heap[m_nCount]);

			m_nCount -= 1;
			Heapify(1);      /* Move new node 1 to right position. */

			return true;  /* Return old last node. */
		}


		/**
		* Returns the node of minimum value from the m_heap (top of the m_heap).
		*
		* Params:
		*     value = out parameter used to return the min element
		* Returns: false if m_heap empty
		*/
		bool popMin(T& value)
		{
			if (m_nCount == 0) {
				return false;
			}

			/* Switch first node with last. */
			swap(m_heap[1],m_heap[m_nCount]);

			m_nCount -= 1;
			Heapify(1);      /* Move new node 1 to right position. */

			value = m_heap[m_nCount + 1];
			return true;  /* Return old last node. */
		}

		const T& Top()
		{
			// just for ...
			return m_heap[1];
		}
		
		/**
		* Reorganizes the m_heap (a parent is smaller than its children)
		* starting with a node.
		*
		* Params:
		*     parent = node form which to start m_heap reorganization.
		*/
		void Heapify(int parent)
		{
			int minloc = parent;

			/* Check the left child */
			int left = 2 * parent;
			if (left <= m_nCount && m_comp(m_heap[left], m_heap[parent])) {
				minloc = left;
			}

			/* Check the right child */
			int right = left + 1;
			if (right <= m_nCount && m_comp(m_heap[right], m_heap[minloc])) {
				minloc = right;
			}

			/* If a child was smaller, than swap parent with it and m_heapify. */
			if (minloc != parent) {
				swap(m_heap[parent],m_heap[minloc]);
				Heapify(minloc);
			}
		}

	private:
		/**
		* Storage array for the m_heap.
		* Type T must be comparable.
		*/
		T* m_heap;
		int m_nLength;

		/**
		* Number of element in the m_heap
		*/
		int m_nCount;
		Comp m_comp;
	};



}
