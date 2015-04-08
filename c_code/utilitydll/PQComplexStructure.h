#pragma once

#include <algorithm>
#include <list>


using namespace std;

/**
* Priority Queue Implementation
*
* The priority queue is implemented with a m_heap.  A m_heap is a complete
* (full) binary tree in which each parent is less than both of its
* children, but the order of the children is unspecified.
* Note that a m_heap uses 1-based indexing to allow for power-of-2
* location of parents and children.  We ignore element 0 of m_heap array.
*/
template < typename T>
class PQComplexStructure {
public:
	PQComplexStructure()
	{
		m_nCount = 0;
		m_heap = NULL;
		m_nLength = 0;
	}
	/**
	* Destructor.
	*
	*/
	~PQComplexStructure()
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
		m_heap = new pair<list<T>::iterator, double>[m_nLength];  // m_heap uses 1-based indexing
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
		m_list.clear();
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
	void insert(T key, double value)
	{
		

		/* If m_heap is full, re-allocate space. */
		if (m_nCount == m_nLength-1) 
		{
			int length2 = m_nLength * 2;
			pair<list<T>::iterator, double>* heap2 = new pair<list<T>::iterator, double>[length2];
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
		while (par > 0  && value < m_heap[par].second) {
			m_heap[loc] = m_heap[par];     /* Move parent down to loc. */
			loc = par;
			par = loc / 2;
		}
		
		m_list.push_front(key);
		list<T>::iterator iter = m_list.begin();

		/* Insert the element at the determined location. */
		m_heap[loc] = pair<list<T>::iterator, double>(iter, value);
	}

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
	* ofReturns the node of minimum value from the m_heap (top  the m_heap).
	*
	* Params:
	*     value = out parameter used to return the min element
	* Returns: false if m_heap empty
	*/
	bool popMin(T& key, double &value)
	{
		if (m_nCount == 0) {
			return false;
		}

		/* Switch first node with last. */
		swap(m_heap[1],m_heap[m_nCount]);

		m_nCount -= 1;
		Heapify(1);      /* Move new node 1 to right position. */

		pair<list<T>::iterator, double> &p = m_heap[m_nCount + 1];
		key = *p.first;
		value = p.second;
		return true;  /* Return old last node. */
	}

	void Top(T& key, double &value)
	{
		pair<list<T>::iterator, double> &p = m_heap[1];
		key = *p.first;
		value = p.second;
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
		if (left <= m_nCount && m_heap[left].second < m_heap[parent].second) {
			minloc = left;
		}

		/* Check the right child */
		int right = left + 1;
		if (right <= m_nCount && m_heap[right].second < m_heap[minloc].second) {
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
	pair<list<T>::iterator, double>* m_heap;
	int m_nLength;

	list<T> m_list;

	/**
	* Number of element in the m_heap
	*/
	int m_nCount;
};
