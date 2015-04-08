#pragma once

#include "utility.h"

#define MAX_SPARSITY 8
#define MAX_NUM_CANDIDATE 20

//#define TYPE_GKMEANS 10
//#define TYPE_OCKMEANS 1
//#define TYPE_AQ 100

enum TypeEncoding
{
	Type_gk_means,
	Type_ock,
	Type_additive_quantization
};


typedef unsigned char CodeType;

using namespace utility;

/*
contraint type 0: k-means
constraint type 1: change the cardinality of the vector to be c
constraint type 2: change the cardinality of the vector to be c. partition-based
*/
class IndexEncoding
{
public:
	IndexEncoding();

	void SetCandidateNumber(int num_can);
	void SetIsInitialize(bool is_initialize);
	//void SetIsOCK(int is_ock);
	void SetEncodingType(TypeEncoding type_encoding);
	void SetNumberGroup(int num_grouped);

	//void Solve(const SMatrix<double>& matZ,
	//	const Vector<SMatrix<double> >& vecmatDictionary,
	//	int num_dic_each_partition,
	//	SMatrix<short>& matRepresentation); // the method value only valid when num_dic is larger than 1

	void Solve(const SMatrix<double>& matZ,
		const Vector<SMatrix<double> >& vecmatDictionary,
		int num_dic_each_partition,
		SMatrix<unsigned char>& matRepresentation); // the method value only valid when num_dic is larger than 1


	// constraint type 0:
	/*void SolveKMeans(const SMatrix<double>& matZ,
	const SMatrix<double>& matDictionary,
	SMatrix<short>& matRepresentation);
	*/
private:
	void MultiDictionaryPreprocess(const SMatrix<double> &matDictionary, 
		int num_dic_each_partition, 
		SMatrix<double> &matPrecomputed) const;

	void MultiDictionaryPreprocess(
		const Vector<SMatrix<double> > &vecmatDictionary, 
		int num_dic_each_partition, 
		Vector<SMatrix<double> > &vecmatPrecomputed) const;

	void GKMeansPreprocess(
		const Vector<SMatrix<double> >& vecmatDictionary,
		int num_dic_each_partition,
		Vector<SMatrix<double> > & vecmatPrecomputed) const;

	void GKMeansPreprocess(const SMatrix<double> &matDictionary, 
		int num_dic_each_partition, 
		SMatrix<double >  &matPrecomputed) const;

	void Solve(const double* pz,
		const SMatrix<double>& matDictionary,
		short* prepresentation) const;

	void SolveOptimized(const double* pz,
		const SMatrix<double>& matDictionary,
		short* prepresentation) const;

	/*void SolveOptimizedAdv(const double* pz,
	const SMatrix<double>& matDictionary,
	short* prepresentation) const;*/

	int SolveOptimizedAdv2(const double* pz,
		const SMatrix<double>& matDictionary, 
		const SMatrix<double>& matInnerProduct,
		short* prepresentation) const;

	int SolveOptimizedAdv3(const double* pz,
		const SMatrix<double> &matDictionary, 
		int num_dic_each_partition,
		const SMatrix<double> &matPrecomputed,
		short* prepresentation) const;

	int SolveAdditiveQuantization(const double* pz, 
		const SMatrix<double> &matDictionary, 
		int num_dic_each_partition, 
		const SMatrix<double> &matPrecomputed, 
		short* prepresentation);

	int SolveAdditiveQuantization(const Vector<double> &vec_x_map, 
		const SMatrix<double> &matPrecomputed, 
		int num_dic_each_partition, 
		short* prepresentation) const;

	int SolveOptimizedAdv3(
		const Vector<double> &vec_x_map, 
		const SMatrix<double> &matPrecomputed, 
		int num_dic_each_partition, 
		short* prepresentation) const;

	bool SolveOptimizedAdv3Once(
		const Vector<double> &vec_x_map, 
		const SMatrix<double> &matPrecomputed, 
		int num_dic_each_partition, 
		short* prepresentation) const;

	void PrecomputeA(
		int idx_dic_a, int idx_dic_b, 
		const Vector<double> &vec_x_map, 
		const SMatrix<double> &matPrecomputed, 
		int num_dic_each_partition, 
		short* prepresentation, 
		Vector<double> &vec_pre_a, int sub_dic_size) const;


	//bool SolveOptimizedAdv3TwiceInitialize(
	//	int idx_dic_a, int idx_dic_b,
	//	const Vector<double> &vec_x_map, 
	//	const SMatrix<double> &matPrecomputed, 
	//	int num_dic_each_partition, 
	//	short* prepresentation) const;

	bool SolveOptimizedAdv3Twice(
		int idx_dic_a, int idx_dic_b,
		const Vector<double> &vec_x_map, 
		const SMatrix<double> &matPrecomputed, 
		int num_dic_each_partition, 
		short* prepresentation) const;

	bool SolveOptimizedAdv3Twice(
		const Vector<double> &vec_x_map, 
		const SMatrix<double> &matPrecomputed, 
		int num_dic_each_partition, 
		short* prepresentation) const;

	//bool SolveOptimizedAdv3TwiceInitialize(
	//	const Vector<double> &vec_x_map, 
	//	const SMatrix<double> &matPrecomputed, 
	//	int num_dic_each_partition, 
	//	short* prepresentation) const;

	//void SolveOptimizedAdv3Initialize(
	//	const Vector<double> &vec_x_map, 
	//	const SMatrix<double> &matPrecomputed, 
	//	int num_dic_each_partition, 
	//	short* prepresentation) const;

	void SolveOptimizedAdv2Recursive(
		const Vector<double> &vec_x_map, 
		const SMatrix<double> &matInnerProduct, 
		int idx, 
		short* prepresentation, 
		double &error) const;



	void BestNextWordsSMart(
		const Vector<double> &vec_x_map, 
		const SMatrix<double> &matInnerProduct, 
		const short* prepresentation,
		int idx, 
		short next_idx[], 
		double next_errors[]) const;


	void SolveOptimizedAdvRecursive(const double* pz,
		const SMatrix<double>& matDictionary,
		int idx,
		short* prepresentation,
		double &error) const;

	void SolveKMeans(const double* pz,
		const SMatrix<double>& matDictionary,
		short* prepresentation) const;


private:
	void BestNextWord(double* p_residual, 
		const SMatrix<double>& matDictionary, 
		const bool* p_rest_dict,
		double& best_error, int& best_idx) const;

	void BestNextWord(const double* p_residual, 
		const SMatrix<double>& matDictionary, 
		double& best_error, int& best_idx) const;

	void BestNextWord(const double* p_residual, 
		const SMatrix<double>& matDictionary, 
		int idx_start,
		int idx_end,
		double& best_error, short& best_idx) const;

	void BestNextWords(const double* p_residual, 
		const SMatrix<double>& matDictionary, 
		int idx_start,
		int idx_end,
		SMatrix<double> &mat_diff, Vector<short>& best_idx) const;


private:
	int m_nNumPoint;
	int m_nNumberDictionaryEachPartition; 
	int m_nSubDicSizeEachPartition;
	int m_nNumBestCan; // used for ock-means and additive_quantization

	int m_nNumberOfGroup;
	//int m_aq_candidate;

	//int m_is_ockmeans;

	TypeEncoding m_type_encoding;

	bool m_isInitialize;
	//Vector<int> m_vecNumberIterativeAssignment;
	//const Vector<SMatrix<double> >* m_pvecMatDictionary;
};
