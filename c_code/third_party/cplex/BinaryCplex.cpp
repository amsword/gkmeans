#include "BinaryCplex.h"

#include <ctime>


void BinaryCplex::Solve(const double* pz,
						const SMatrix<double>& matDictionary,
						short* prepresentation) const
{
	int status;
	Vector<double> vecLinearPart(m_nDictionarySize);

	matDictionary.MultipleN(pz, vecLinearPart.Ptr());

	char     *probname = "have_a_try";
	//CPXLPptr lp = CPXcreateprob (m_env, &status, probname);
	CPXLPptr lp = m_vecLp[omp_get_thread_num()];

	PRINT << omp_get_thread_num() << "\n";

	status = CPXcopylp (m_env, lp, 
		m_nDictionarySize, m_nSparsity, 
		1,	
		vecLinearPart.Ptr(), 
		m_vecRhs.Ptr(),
		m_vecSense.Ptr(), 
		m_vecConBegin.Ptr(), 
		m_vecConCount.Ptr(), 
		m_vecConIndex.Ptr(), 
		m_vecConEfficient.Ptr(),
		m_vecLower.Ptr(), 
		m_vecUpper.Ptr(), NULL);

	status = CPXcopyctype (m_env, lp, m_vecTypes.Ptr());

	status = CPXcopyquad (m_env, lp, 
		m_vecQBegin.Ptr(), 
		m_vecQCount.Ptr(), 
		m_matQIndex.Ptr(), 
		m_matQEfficient.Ptr());

	status = CPXmipopt (m_env, lp);

	SMART_ASSERT(status == 0)(status).Exit();

	Vector<double> result(m_nDictionarySize);

	status = CPXgetx (m_env, lp, result.Ptr(), 0, m_nDictionarySize - 1);
	
	SMART_ASSERT(status == 0)(status).Exit();

	int k = 0;
	for (int i = 0; i < m_nDictionarySize; i++)
	{
		if (result[i] > 0.5)
		{
			prepresentation[k] = i;
			k++;
		}
	}
}

void BinaryCplex::ConstraintEfficient()
{
	m_vecConEfficient.AllocateSpace(m_nDictionarySize);
	m_vecConEfficient.SetValue(1);

	m_vecConIndex.AllocateSpace(m_nDictionarySize);

	m_vecConCount.AllocateSpace(m_nDictionarySize);
	m_vecConCount.SetValue(1);

	for (int i = 0; i < m_nSparsity; i++)
	{
		int start = i * m_nSubDictionarySize;
		for (int j = start; j < start + m_nSubDictionarySize; j++)
		{
			m_vecConIndex[j] = i;
		}
	}

	m_vecConBegin.AllocateSpace(m_nDictionarySize);
	for (int i = 0; i < m_nDictionarySize; i++)
	{
		m_vecConBegin[i] = i;
	}

	m_vecLower.AllocateSpace(m_nDictionarySize);
	m_vecUpper.AllocateSpace(m_nDictionarySize);
	m_vecLower.SetValue(0);
	m_vecUpper.SetValue(1);
}

void BinaryCplex::InitializeCPLEX()
{
	int status;
	m_env = CPXopenCPLEX (&status);
	SMART_ASSERT(m_env != NULL).Exit();

	status = CPXsetintparam (m_env, CPX_PARAM_SCRIND, CPX_OFF);
	SMART_ASSERT(status == 0).Exit();

	status = CPXsetintparam(m_env, CPX_PARAM_THREADS, 1);
	SMART_ASSERT(status == 0).Exit();

	
	// create multiple linear programming problem
	int num_thread = omp_get_max_threads();
	SMART_ASSERT(num_thread > 0)(num_thread).Exit();
	m_vecLp.AllocateSpace(num_thread);
	for (int i = 0; i < num_thread; i++)
	{
		string probname;
		TypeConvert(i, probname);

		m_vecLp[i] = CPXcreateprob (m_env, &status, probname.c_str());
		SMART_ASSERT(m_vecLp[i] != NULL)(m_vecLp[i]).Exit();
	}

}
void BinaryCplex::UninitializeCPLEX()
{
	for (int i = 0; i < m_vecLp.size(); i++)
	{
		CPXfreeprob (m_env, &m_vecLp[i]);
	}
	CPXcloseCPLEX(&m_env);
}

void BinaryCplex::QuadraticEfficient(const SMatrix<double>& matDictionary)
{
	int rows = matDictionary.Rows();
	int cols = matDictionary.Cols();

	m_matQEfficient.AllocateSpace(rows, rows);
	m_matQIndex.AllocateSpace(rows, rows);

	m_vecQBegin.AllocateSpace(rows);
	m_vecQCount.AllocateSpace(rows);

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < rows; j++)
		{
			m_matQEfficient[i][j] = dot(matDictionary[i], matDictionary[j], cols);
			m_matQIndex[i][j] = j;
		}
		m_vecQBegin[i] = i * rows;
		m_vecQCount[i] = rows;
	}
}
void BinaryCplex::Preprocessing(const SMatrix<double>& matDictionary)
{
	QuadraticEfficient(matDictionary);

	m_vecTypes.AllocateSpace(m_nDictionarySize);
	m_vecTypes.SetValue('B');

	m_vecRhs.AllocateSpace(m_nSparsity);
	m_vecRhs.SetValue(1);

	m_vecSense.AllocateSpace(m_nSparsity);
	m_vecSense.SetValue('L');
}

void BinaryCplex::Solve(const SMatrix<double>& matZ,
						const SMatrix<double>& matDictionary,
						int nSparsity,
						SMatrix<short>& matRepresentation)
{

	m_nNumPoint = matZ.Rows();
	m_nDictionarySize = matDictionary.Rows();
	m_nDimension = matZ.Cols();
	m_nSparsity = nSparsity;

	SMART_ASSERT(m_nNumPoint == matRepresentation.Rows())(matRepresentation.Rows()).Exit();

	m_nSubDictionarySize = matDictionary.Rows() / m_nSparsity;
	SMART_ASSERT(m_nSparsity == matRepresentation.Cols())(matRepresentation.Cols()).Exit();

	Preprocessing(matDictionary);

	ConstraintEfficient();

	InitializeCPLEX();

	matRepresentation.SetValue(-1);

//#pragma omp parallel for
	//for (int i = 0; i < m_nNumPoint; i++)
	for (int i = 0; i < 2; i++)
	{
		const double* pz = matZ[i];
		short* prepresentation = matRepresentation[i];

		clock_t begin = clock();

		Solve(pz, matDictionary, prepresentation);

		clock_t end = clock();

		PRINT << "Time cost: " 
			<< (end - begin) / (double)CLOCKS_PER_SEC << "\n";

		PRINT << Vector<short>(prepresentation, 2) << "\n";

		//SMART_ASSERT(0).Exit();
	}

	UninitializeCPLEX();
}