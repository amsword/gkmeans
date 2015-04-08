#include "Multiplication.h"

void computeZBt(const SMatrix<double> &matZ, const SMatrix<short> &matB, 
		 SMatrix<double> &matZBt)
{
	int num_point = matZ.Rows();
	int dim = matZ.Cols();
	int num_sub_dic = matB.Cols();

	matZBt.SetValueZeros();

	for (int i = 0; i < num_point; i++)
	{
		const double* p_point = matZ[i];
		const short* p_words_idx = matB[i];
		for (int j = 0; j < num_sub_dic; j++)
		{
			short idx_word = p_words_idx[j];
			VectorAdd(matZBt[idx_word], p_point, dim);
		}
	}
}


void computeZBt(const SMatrix<double> &matZ, const SMatrix<unsigned char> &matB, 
		 SMatrix<double> &matZBt)
{
	int num_point = matZ.Rows();
	int dim = matZ.Cols();
	int num_sub_dic = matB.Cols();
	int sub_dic_size = matZBt.Rows() / num_sub_dic;
	SMART_ASSERT((matZBt.Rows() % num_sub_dic) == 0).Exit();

	matZBt.SetValueZeros();

	for (int i = 0; i < num_point; i++)
	{
		const double* p_point = matZ[i];
		const unsigned char* p_words_idx = matB[i];
		for (int j = 0; j < num_sub_dic; j++)
		{
			short idx_word = p_words_idx[j] + j * sub_dic_size;
			VectorAdd(matZBt[idx_word], p_point, dim);
		}
	}
}

void computeBBt(const SMatrix<short> &matB, 
		 SMatrix<int> &matBBt)
{
	int num_point = matB.Rows();

	int num_sub_dic = matB.Cols();

	matBBt.SetValueZeros();
	for (int i = 0; i < num_point; i++)
	{
		const short* p_idx_words = matB[i];
		for (int j1 = 0; j1 < num_sub_dic; j1++)
		{
			int k1 = p_idx_words[j1];
			for (int j2 = 0; j2 < num_sub_dic; j2++)
			{
				int k2 = p_idx_words[j2];
				matBBt[k1][k2]++;
			}
		}
	}
}

void computeBBt(const SMatrix<unsigned char> &matB, 
		 SMatrix<int> &matBBt)
{
	int num_point = matB.Rows();

	int num_sub_dic = matB.Cols();
	int sub_dic_size = matBBt.Rows() / num_sub_dic;

	matBBt.SetValueZeros();
	for (int i = 0; i < num_point; i++)
	{
		const unsigned char* p_idx_words = matB[i];
		for (int j1 = 0; j1 < num_sub_dic; j1++)
		{
			int k1 = p_idx_words[j1] + j1 * sub_dic_size;
			for (int j2 = 0; j2 < num_sub_dic; j2++)
			{
				int k2 = p_idx_words[j2] + j2 * sub_dic_size;
				matBBt[k1][k2]++;
			}
		}
	}
}