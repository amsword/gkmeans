#include "TypeConvert.h"
#include "IndexEncoding.h"
#include "make_mex.h"
#include "make_mat.h"
#include <omp.h>

#define MAT_Z prhs[0] 
#define MAT_DIC prhs[1]
#define N_SPARSITY prhs[2]
//#define NUM_CAN_BEST prhs[3]
//#define OLD_BINARY_REPRESENTATION prhs[3]
#define PARAMETER prhs[3]

#define BINARY_REPRESENTATION plhs[0]

int test()
{
	string str_file_name = "E:\\research\\working\\test_data\\test_gk_means.mat";

	SMatrix<double> matZ;
	Vector<SMatrix<double> > vecmatDictionary;
	SMatrix<CodeType> mat_target;
	int num_dic;
	int is_initialize;
	int num_grouped;
	{
		MATFile* fp = matOpen(str_file_name.c_str(), "r");
		SMART_ASSERT(fp).Exit();

		mexConvert(matGetVariable(fp, "Z"), matZ);
		mexConvert(matGetVariable(fp, "all_D"), vecmatDictionary);
		mexConvert(matGetVariable(fp, "num_sub_dic_each_partition"), num_dic);
		mexConvert(matGetVariable(fp, "mat_compact_B"), mat_target);

		mxArray * para_encode = matGetVariable(fp, "para_encode");
		mexConvert(mxGetField(para_encode, 0, "is_initialize"), is_initialize);
		mexConvert(mxGetField(para_encode, 0, "num_grouped"), num_grouped);
		matClose(fp);
	}

	IndexEncoding ie;
	ie.SetIsInitialize(is_initialize);
	ie.SetNumberGroup(num_grouped);
	ie.SetEncodingType(Type_gk_means);
	ie.SetEncodingType(Type_additive_quantization);

	SMatrix<CodeType> matRepresentation;
	matRepresentation.AllocateSpace(mat_target.Rows(), mat_target.Cols());

	int num_selected_rows = 10;
	matRepresentation.AllocateSpace(num_selected_rows, mat_target.Cols());
	ie.Solve(SMatrix<double>(matZ.Ptr(), num_selected_rows, matZ.Cols()), 
		vecmatDictionary, 
		num_dic, 
		matRepresentation);
	PRINT << "good\n";
	for (int i = 0; i < matRepresentation.Rows(); i++)
	{
		for (int j = 0; j < matRepresentation.Cols(); j++)
		{
			cout << (int)(matRepresentation[i][j]) << "\t";
		}
		cout << "\n";
	}

	//for (int i = 0; i < mat_target.Rows(); i++)
	//{
	//	for (int j = 0; j < mat_target.Cols(); j++)
	//	{
	//		SMART_ASSERT(matRepresentation[i][j] == mat_target[i][j]).Exit();
	//	}
	//}

	return 0;
}



#ifdef _WINDOWS
int main()
{
	test();

	return 0;
}

#endif


//compact B
void mexFunction(int nlhs, mxArray *plhs[], 
				 int nrhs, const mxArray *prhs[])
{
	int num_thd = omp_get_num_procs();
	omp_set_num_threads(num_thd);

	SMART_ASSERT(nrhs == 4).Exit();

	SMatrix<double> matZ;
	Vector<SMatrix<double> > vecmatDictionary;
	int num_dic;

	mexConvert(MAT_Z, matZ);
	mexConvert(MAT_DIC, vecmatDictionary);
	mexConvert(N_SPARSITY, num_dic);
	SMART_ASSERT(num_dic > 0)(num_dic).Exit();

	int num_partitions = vecmatDictionary.size();

	mwSize size[2];
	size[0] = num_dic * num_partitions;
	size[1] = matZ.Rows();

	//BINARY_REPRESENTATION = mxCreateNumericArray(2, size, mxINT16_CLASS, mxREAL);
	BINARY_REPRESENTATION = mxCreateNumericArray(2, size, mxUINT8_CLASS, mxREAL);
	SMatrix<CodeType> matRepresentation;
	mexConvert(BINARY_REPRESENTATION, matRepresentation);

	// method:
	// 2: ock-means described in the paper
	// others: jck-means
	IndexEncoding mp;

	if (num_dic > 1)
	{
		SMART_ASSERT(mxIsEmpty(PARAMETER) == false).Exit();

		int is_initialize;
		int num_grouped;
		mexConvert(mxGetField(PARAMETER, 0, "is_initialize"), is_initialize);;


		TypeEncoding type_encoding = Type_gk_means;
		int is_ock = 0;
		{
			mxArray* p2 = mxGetField(PARAMETER, 0, "is_ock");
			if (p2)
			{

				mexConvert(p2, is_ock);
				if (is_ock)
				{
					type_encoding = Type_ock;
				}
			}
		}
		{
			mxArray* p2 = mxGetField(PARAMETER, 0, "encoding_type");
			if (p2)
			{
				string str_encoding_type;
				mexConvert(p2, str_encoding_type);

				{
					if (is_ock)
					{
						SMART_ASSERT(str_encoding_type == "ock").Exit();
					}
				}

				if (str_encoding_type == "gkmeans")
				{
					mp.SetEncodingType(Type_gk_means);
					mexConvert(mxGetField(PARAMETER, 0, "num_grouped"), num_grouped);
					mp.SetNumberGroup(num_grouped);
				}
				else if (str_encoding_type == "ock")
				{
					mp.SetEncodingType(Type_ock);
					int num_can;
					mexConvert(mxGetField(PARAMETER, 0, "num_can"), num_can);
					mp.SetCandidateNumber(num_can);
				}
				else if (str_encoding_type == "additive_quantization")
				{
					mp.SetEncodingType(Type_additive_quantization);
					int num_can;
					mexConvert(mxGetField(PARAMETER, 0, "num_can"), num_can);
					mp.SetCandidateNumber(num_can);
				}
				else
				{
					SMART_ASSERT(0)(str_encoding_type).Exit();
				}
			}
		}

		mp.SetIsInitialize(is_initialize);

		if (!is_initialize)
		{
			SMatrix<CodeType> mat_old;
			mexConvert(mxGetField(PARAMETER, 0, "old_codes"), mat_old);
			memcpy(matRepresentation.Ptr(), mat_old.Ptr(), sizeof(CodeType) * mat_old.Rows() * mat_old.Cols());
		}
	}

	mp.Solve(matZ, vecmatDictionary, num_dic, matRepresentation);
}