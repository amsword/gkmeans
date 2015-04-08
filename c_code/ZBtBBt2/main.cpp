#include "utility.h"
#include <list>
#include "TypeConvert.h"
#include "Multiplication.h"

using namespace std;

typedef unsigned char CodeType;

#define MAT_Z prhs[0]
#define MAT_B prhs[1]
#define NUM_ALL_WORDS prhs[2]

#define ZBT plhs[0]
#define BBT plhs[1]

using namespace utility;

void mexFunction(int nlhs, mxArray *plhs[], 
				 int nrhs, const mxArray *prhs[])
{
	SMART_ASSERT(nrhs == 3)(nrhs).Exit();
	SMART_ASSERT(nlhs == 2).Exit();

	SMatrix<double> matZ;
	SMatrix<CodeType> matB;
	int num_all_words;

	mexConvert(MAT_Z, matZ);
	mexConvert(MAT_B, matB);
	mexConvert(NUM_ALL_WORDS, num_all_words);
	int dim = matZ.Cols();

	SMatrix<double> matZBt;
	SMatrix<int> matBBt;

	mwSize dims[2];
	dims[0] = num_all_words;
	dims[1] = num_all_words;
	ZBT = mxCreateDoubleMatrix(dim, num_all_words, mxREAL);
	BBT = mxCreateNumericArray(2, dims, mxINT32_CLASS, mxREAL);

	mexConvert(ZBT, matZBt);
	mexConvert(BBT, matBBt);

	computeZBt(matZ, matB, matZBt);
	computeBBt(matB, matBBt);
}
