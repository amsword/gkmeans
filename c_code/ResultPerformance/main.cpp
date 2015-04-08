#include "ANNPerformance.h"
#include "SearchEngineSCK2.h"
#include "SearchEngineSCKMeans3.h"
#include "TypeConvert.h"

#include "make_mex.h"

#define RETRIEVAL_RESULT prhs[0]
#define STR_GND prhs[1]
#define VEC_TEST_TOPKS prhs[2]
#define NUM_DATA_BASE_POINTS prhs[3]


#define RECALL_EACH_TOPK plhs[0]

void mexFunction(int nlhs, mxArray *plhs[], 
				 int nrhs, const mxArray *prhs[])
{
	SMART_ASSERT(nrhs == 4)(nrhs).Exit();

	string str_gnd;

	ANNPerformance perf;

	Vector<int> vecTopKs;
	mexConvert(VEC_TEST_TOPKS, vecTopKs);

	ForwardIndex<double> gnd;
	mexConvert(STR_GND, str_gnd);

	int max_value;
	int max_idx;
	vecTopKs.MaxValue(max_value, max_idx);
	gnd.LoadData(str_gnd, max_value, false);

	SMatrix<int> matRetrievalResult;
	mexConvert(RETRIEVAL_RESULT, matRetrievalResult);

	int num_data_base_points;
	mexConvert(NUM_DATA_BASE_POINTS, num_data_base_points);

	SMART_ASSERT(matRetrievalResult.Rows() == gnd.Rows())
		(matRetrievalResult.Rows())
		(gnd.Rows()).Exit();

	int numCandidate = matRetrievalResult.Cols();

	RECALL_EACH_TOPK = mxCreateDoubleMatrix(numCandidate, vecTopKs.Size(), mxREAL);

	SMatrix<double> matRecallEachTopk;

	mexConvert(RECALL_EACH_TOPK, matRecallEachTopk);

	SMART_ASSERT(1)(matRetrievalResult.Rows())(matRetrievalResult.Cols())
		(num_data_base_points)(gnd.Rows())(gnd.Cols())(vecTopKs)
		(matRecallEachTopk.Rows())(matRecallEachTopk.Cols()).Exit();

	perf.RecallNumberRetrieved(
		&matRetrievalResult,
		num_data_base_points,
		&gnd,
		&vecTopKs,
		&matRecallEachTopk);
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
//	Vector<SMatrix<double> > vecmatCodeBook(2);
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
