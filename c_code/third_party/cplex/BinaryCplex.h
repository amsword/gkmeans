#pragma once

#include "utility.h"
#include "CPLEX\cplex_header.h"
using namespace utility;

class BinaryCplex
{
public:
	void Solve(const SMatrix<double>& matZ,
		const SMatrix<double>& matDictionary,
		int nSparsity,
		SMatrix<short>& matRepresentation);

private:
	void Preprocessing(const SMatrix<double>& matDictionary);

	void QuadraticEfficient(const SMatrix<double>& matDictionary);

	void ConstraintEfficient();

	void InitializeCPLEX();
	void UninitializeCPLEX();

	void Solve(const double* pz,
		const SMatrix<double>& matDictionary,
		short* prepresentation) const;

private:
	int m_nNumPoint;
	int m_nDictionarySize;
	int m_nSubDictionarySize;
	int m_nDimension;
	int m_nSparsity;


	SMatrix<double> m_matQEfficient;
	SMatrix<int> m_matQIndex;
	Vector<int> m_vecQBegin;
	Vector<int> m_vecQCount;

	Vector<double> m_vecConEfficient;
	Vector<int> m_vecConIndex;
	Vector<int> m_vecConBegin;
	Vector<int> m_vecConCount;
	Vector<double> m_vecLower;
	Vector<double> m_vecUpper;

	Vector<char> m_vecTypes;

	Vector<double> m_vecRhs;
	Vector<char> m_vecSense;

	CPXENVptr m_env;
	Vector<CPXLPptr>      m_vecLp;

};