#include "ANNPerformance.h"
#include "SearchEngineSCK2.h"
#include "SearchEngineSCKMeans3.h"
#include "make_mex.h"
#include "TypeConvert.h"



#define DATA_BASE prhs[0] 
#define CELL_CODE_BOOK prhs[1]
#define QUERY_SET prhs[2]
#define NUM_CANDIDATE prhs[3]

#define RETRIEVAL_RESULT plhs[0]

void mexFunction(int nlhs, mxArray *plhs[], 
				 int nrhs, const mxArray *prhs[])
{
	ANNPerformance perf;

	SMatrix<CodeType> matDataBase;
	mexConvert(DATA_BASE, matDataBase);

	Vector<SMatrix<double> > vecmatCodeBook;
	mexConvert(CELL_CODE_BOOK, vecmatCodeBook);
	
	SMatrix<double> matQuery;
	mexConvert(QUERY_SET, matQuery);

	int numCandidate;
	mexConvert(NUM_CANDIDATE, numCandidate);

	Vector<mwSize> vec_size(2);
	vec_size[1] = matQuery.Rows();
	vec_size[0] = numCandidate;

	RETRIEVAL_RESULT = mxCreateNumericArray(2, vec_size.Ptr(), mxINT32_CLASS, mxREAL);

	SMatrix<int> matRetrievalResult;

	mexConvert(RETRIEVAL_RESULT, matRetrievalResult);

	SMART_ASSERT(1)(matDataBase.Rows())(matDataBase.Cols())
		(vecmatCodeBook.Size())
		(matQuery.Rows())
		(matQuery.Cols())
		(numCandidate)
		(matRetrievalResult.Rows())
		(matRetrievalResult.Cols()).Exit();
	
	//SearchEngineSCKMeans3 search_engine;
	//SearchEngineSCKMeans search_engine; 
	SearchEngineSCKMeans2 search_engine; // proposed one and it is also the general one
	search_engine.Initialize(&matDataBase, &vecmatCodeBook, numCandidate);
	
	perf.RecallNumberRetrieved(&search_engine,
		&matQuery,
		&vecmatCodeBook,
		numCandidate,
		&matRetrievalResult);

	search_engine.Print();
}


////
//int main()
//{
//	string str_dir = "F:\\v-jianfw\\hash\\v-jianfw\\Data_HashCode\\Labelme\\working_sck_means\\test\\new_representation_b\\";
//	string str_gnd;
//
//	ANNPerformance perf;
//
//	SMatrix<short> matDataBase;
//	matDataBase.LoadData(str_dir + "mat_compact_B");
//
//	//SMatrix<bool> matDataBase;
//	//mexConvert(DATA_BASE, matDataBase);
//
//	Vector<SMatrix<double>> vecmatCodeBook(2);
//	vecmatCodeBook[0].LoadData(str_dir + "all_D1");
//	vecmatCodeBook[1].LoadData(str_dir + "all_D2");
//
//	SMatrix<double> matQuery;
//	matQuery.LoadData(str_dir + "query");
//
//	Vector<int> vecTopKs(1);
//	vecTopKs[0] = 1;
//
//
//	ForwardIndex<double> gnd;
//	
//	int max_value;
//	int max_idx;
//	vecTopKs.MaxValue(max_value, max_idx);
//	gnd.LoadData(str_gnd, max_value, false);
//
//	int numCandidate;
//	mexConvert(NUM_CANDIDATE, numCandidate);
//
//	RECALL_EACH_TOPK = mxCreateDoubleMatrix(numCandidate, vecTopKs.Size(), mxREAL);
//
//	SMatrix<double> matRecallEachTopk;
//
//	mexConvert(RECALL_EACH_TOPK, matRecallEachTopk);
//
//	SearchEngineSCKMeans2 search_engine;
//	//SearchEngineSCKMeans search_engine;
//
//	search_engine.Initialize(&matDataBase, &vecmatCodeBook, numCandidate);
//
//	perf.RecallNumberRetrieved(&search_engine,
//		&matQuery,
//		&vecmatCodeBook,
//		&gnd,
//		&vecTopKs,
//		numCandidate,
//		&matRecallEachTopk);
//}
