#include "main_point.h"
#include "TypeConvert.h"
#include "ObjectiveValue.h"
#include "make_mex.h"
#include "Reconstructor.h"
#include <omp.h>
#include "make_mat.h"

int main_exe_construct()
{
	
	return 0;
}
int main_exe_objective_gkmeans()
{
	int num_thd = omp_get_num_procs();
	omp_set_num_threads(num_thd);

	const string str_z = "C:\\Users\\t0908482\\Desktop\\mine\\working\\SIFT1M\\gkmeans\\non_full_training\\matlab.mat";

	SMatrix<double> matZ;
	Vector<SMatrix<double> > vecmatDictionary;
	SMatrix<short> matRepresentation;

	MATFile* fp = matOpen(str_z.c_str(), "r");
	mexConvert(matGetVariable(fp, "Z"), matZ);
	mexConvert(matGetVariable(fp, "all_D"), vecmatDictionary);
	mexConvert(matGetVariable(fp, "compactB"), matRepresentation);

	// method:
	// 2: ock-means described in the paper
	// others: jck-means
	ObjectiveValue mp;

	clock_t begin = clock();
	double s = mp.Compute(matZ, vecmatDictionary, matRepresentation);
	clock_t end = clock();
	PRINT << (end - begin) / (double)CLOCKS_PER_SEC << "\n";
	PRINT << s << "\n";
	return 0;
}
int main_exe_objective()
{
	int num_thd = omp_get_num_procs();
	omp_set_num_threads(num_thd);

	const string str_z = "C:\\Users\\t0908482\\Desktop\\mine\\working\\SIFT1M\\test\\z.double.bin";
	const string str_dic = "C:\\Users\\t0908482\\Desktop\\mine\\working\\SIFT1M\\test\\all_d_";
	const string str_code = "C:\\Users\\t0908482\\Desktop\\mine\\working\\SIFT1M\\test\\code";

	SMatrix<double> matZ;
	Vector<SMatrix<double> > vecmatDictionary;
	SMatrix<short> matRepresentation;

	matZ.LoadData(str_z);
	vecmatDictionary.AllocateSpace(16);
	for (int i = 0; i < 16; i++)
	{
		string str_num;
		TypeConvert(i, str_num);
		string file_name = str_dic + str_num;
		vecmatDictionary[i].LoadData(file_name);
	}
	matRepresentation.LoadData(str_code);

	// method:
	// 2: ock-means described in the paper
	// others: jck-means
	ObjectiveValue mp;

	clock_t begin = clock();
	double s = mp.Compute(matZ, vecmatDictionary, matRepresentation);
	clock_t end = clock();
	PRINT << (end - begin) / (double)CLOCKS_PER_SEC << "\n";
	PRINT << s << "\n";
	return 0;
}

void ComputeObjectiveFunctionValue(int nlhs, mxArray *plhs[], 
				 int nrhs, const mxArray *prhs[])
{
	SMART_ASSERT(nrhs == 4)(nrhs).Exit();
	SMART_ASSERT(nlhs == 1)(nlhs).Exit();

	SMatrix<double> matZ;
	Vector<SMatrix<double> > vecmatDictionary;
	SMatrix<unsigned char> matRepresentation;

	mexConvert(MAT_Z, matZ);

	mexConvert(MAT_DIC, vecmatDictionary);
	mexConvert(BINARY_REPRESENTATION, matRepresentation);

	// method:
	// 2: ock-means described in the paper
	// others: jck-means
	ObjectiveValue mp;

	double s = mp.Compute(matZ, vecmatDictionary, matRepresentation);
	RESULT = mxCreateDoubleScalar(s);
}


void ComputeMultipleAllDAllB(int nlhs, mxArray *plhs[], 
				 int nrhs, const mxArray *prhs[])
{
	Vector<SMatrix<double> > vecmatDictionary;
	SMatrix<unsigned char> matCodes;
	SMatrix<double> matRecovered;

	mexConvert(INPUT_FIRST, vecmatDictionary);
	mexConvert(INPUT_SECOND, matCodes);
	int dim = 0;
	for (int i = 0; i < vecmatDictionary.size(); i++)
	{
		dim += vecmatDictionary[i].Cols();
	}
	int num_point = matCodes.Rows();

	RESULT = mxCreateDoubleMatrix(dim, num_point, mxREAL);
	mexConvert(RESULT, matRecovered);

	Reconstructor recon;
	recon.Recon(vecmatDictionary, matCodes, matRecovered);

}