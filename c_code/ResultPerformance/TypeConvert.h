#pragma once
#include <string>
#include "utility.h"

#include "make_mex.h"

using namespace utility;
using namespace std;

template <class T>
void mexConvert(const mxArray* input, SMatrix<T> & out)
{
	SMART_ASSERT(input).Exit();

	int m = mxGetM(input);
	int n = mxGetN(input);
	T* p = (T*)mxGetPr(input);
	out.Attach(p, n, m);	
}

template <class T>
void mexConvert(const mxArray* input, SMatrix<SMatrix<T> > & out)
{
		SMART_ASSERT(input).Exit();

	int m = mxGetM(input);
	int n = mxGetN(input);
	
	out.AllocateSpace(n, m);
	
	for (int j = 0; j < n; j++)
	{
		for (int i = 0; i < m; i++)
		{
			const mxArray* arr = mxGetCell(input, j * m + i);
			mexConvert(arr, out[j][i]);
		}
	}
}


// input is a cell
template <class T>
void mexConvert(const mxArray* input, Vector<SMatrix<T> > & out)
{
		SMART_ASSERT(input).Exit();

	int size = 0;
	int m = mxGetM(input);
	int n = mxGetN(input);
	
	size = m * n;

	out.AllocateSpace(size);
	for (int i = 0; i < size; i++)
	{
		const mxArray* parr = mxGetCell(input, i);
		mexConvert(parr, out[i]);
	}
}

template <class T>
void mexConvert(const mxArray* input, T & out)
{
		SMART_ASSERT(input).Exit();
	out = mxGetScalar(input);
}

template <class T>
void mexConvert(const mxArray* input, Vector<Vector<T> > &vecvec)
{
		SMART_ASSERT(input).Exit();
	int size = mxGetM(input) * mxGetN(input);
	
	vecvec.AllocateSpace(size);
	for (int i = 0; i < size; i++)
	{
		const mxArray* parr = mxGetCell(input, i);
		mexConvert(parr, vecvec[i]);
	}
}

template <class T>
void mexConvert(const mxArray* input, Vector<T> &vec)
{
		SMART_ASSERT(input).Exit();
	int num = mxGetM(input) * mxGetN(input);
	
	T* p = (T*)mxGetPr(input);
	vec.Attach(p, num);
}

template <>
void mexConvert<string>(const mxArray* input, string& str);
