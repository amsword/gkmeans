#include <cfloat>
#include "IndexEncoding.h"

IndexEncoding::IndexEncoding()
	: m_nNumBestCan(10), m_isInitialize(0), m_nNumberOfGroup(2), 
	/*m_is_ockmeans(0), */m_type_encoding(Type_gk_means)
{
}

void IndexEncoding::BestNextWords(const double* p_residual, 
								  const SMatrix<double>& matDictionary, 
								  int idx_start,
								  int idx_end,
								  SMatrix<double> &mat_diff, 
								  Vector<short>& best_idx) const
{
	int m_nDimension = matDictionary.Cols();

	SMatrix<double> mat_each_diff(idx_end - idx_start, m_nDimension);

	Heap<PAIR<double> > pq;
	pq.Reserve(m_nNumBestCan);

	for (int i = idx_start; i < idx_end; i++)
	{
		const double* p_dic = matDictionary[i];

		double e = 0;
		for (int j = 0; j < m_nDimension; j++)
		{
			double d = p_residual[j] - p_dic[j];
			mat_each_diff[i - idx_start][j] = d;
			e += d * d;
		}

		if (pq.size() >= m_nNumBestCan)
		{
			const PAIR<double> &p = pq.Top();
			if (p.distance > e)
			{
				pq.popMin();

				pq.insert(PAIR<double>(i - idx_start, e));
			}
		}
		else
		{
			pq.insert(PAIR<double>(i - idx_start, e));
		}

	}

	mat_diff.AllocateSpace(m_nNumBestCan, m_nDimension);
	best_idx.AllocateSpace(m_nNumBestCan);
	for (int i = m_nNumBestCan - 1; i >= 0; i--)
	{
		PAIR<double> p;
		pq.popMin(p);
		best_idx[i] = p.index;
		memcpy(mat_diff[i], mat_each_diff[p.index], sizeof(double) * m_nDimension);
	}
}
void IndexEncoding::BestNextWord(const double* p_residual, 
								 const SMatrix<double>& matDictionary, 
								 int idx_start,
								 int idx_end,
								 double& best_error, short& best_idx) const
{
	best_error = DBL_MAX;
	best_idx = -1;
	int dim = matDictionary.Cols();

	for(int i = idx_start; i < idx_end; i++)
	{
		double err = squared_distance(p_residual,
			matDictionary[i], dim);
		if (err < best_error)
		{
			best_error = err;;
			best_idx = i;
		}
	}
	//best_idx -= idx_start;
}

void IndexEncoding::BestNextWord(const double* p_residual, 
								 const SMatrix<double>& matDictionary, 
								 double& best_error, int& best_idx) const
{
	best_error = DBL_MAX;
	best_idx = -1;
	int dim = matDictionary.Cols();
	int num_center = matDictionary.Rows();
	for(int i = 0; i < num_center; i++)
	{
		double err = squared_distance(p_residual,
			matDictionary[i], dim);
		if (err < best_error)
		{
			best_error = err;;
			best_idx = i;
		}
	}
}

void IndexEncoding::BestNextWord(double* p_residual, 
								 const SMatrix<double>& matDictionary, 
								 const bool* p_rest_dict,
								 double& best_error, 
								 int& best_idx) const
{
	best_error = DBL_MAX;
	best_idx = -1;
	int m_nDimension = matDictionary.Cols();
	int num_words = matDictionary.Rows();
	for(int i = 0; i < num_words; i++)
	{
		if (!p_rest_dict[i])
		{
			double err = squared_distance(p_residual,
				matDictionary[i], m_nDimension);
			if (err < best_error)
			{
				best_error = err;;
				best_idx = i;
			}
		}
	}
}

void IndexEncoding::SolveOptimized(const double* pz,
								   const SMatrix<double>& matDictionary,
								   short* prepresentation) const
{
	int m_nDimension = matDictionary.Cols();
	Vector<double> vec_residual(m_nDimension);
	memcpy(vec_residual.Ptr(), pz, sizeof(double) * m_nDimension);

	double pre_error = vec_residual.Norm2Squared();

	int num_center = matDictionary.Rows();

	Vector<bool> vec_representation(num_center);
	if (m_nNumberDictionaryEachPartition >= 0)
	{
		vec_representation.SetValueZeros();
	}

	double best_error;
	short best_idx;
	int m_nSubDictionarySize = num_center / m_nNumberDictionaryEachPartition;
	int idx_start = 0;
	for (int i = 0; i < m_nNumberDictionaryEachPartition; i++)
	{
		idx_start = i * m_nSubDictionarySize;

		//BestNextWord(vec_residual.Ptr(), matDictionary, vec_representation.Ptr(), 
		//best_error, best_idx);

		BestNextWord(vec_residual.Ptr(), matDictionary,
			idx_start, idx_start + m_nSubDictionarySize,
			best_error, best_idx);

		if (pre_error > best_error)
		{
			vec_residual -= matDictionary[best_idx];
			pre_error = best_error;

			//int mask_start = best_idx / m_nSubDictionarySize;
			//memset(vec_representation.Ptr() + mask_start, 1, sizeof(bool) * m_nSubDictionarySize);

			prepresentation[i] = best_idx;
		}
		else
		{
			break;
		}
	}
}

void IndexEncoding::BestNextWordsSMart(
	const Vector<double> &vec_x_map, 
	const SMatrix<double> &matInnerProduct, 
	const short* prepresentation,
	int idx, 
	short next_idx[], 
	double next_errors[]) const
{
	Heap<PAIR<double> > pq;
	pq.Reserve(m_nNumBestCan);

	int sub_dic_size = matInnerProduct.Rows() / m_nNumberDictionaryEachPartition;

	int idx_start = idx * sub_dic_size;
	int idx_end = idx_start + sub_dic_size;

	for (int i = idx_start; i < idx_end; i++)
	{
		// compoute the relative error
		double e = -vec_x_map[i];
		const double* p_inner = matInnerProduct[i];
		for (int j = 0; j < idx; j++)
		{
			e += p_inner[prepresentation[j]];
		}
		e += 0.5 * p_inner[i];

		if (pq.size() >= m_nNumBestCan)
		{
			const PAIR<double> &p = pq.Top();
			if (p.distance > e)
			{
				pq.popMin();

				pq.insert(PAIR<double>(i, e));
			}
		}
		else
		{
			pq.insert(PAIR<double>(i, e));
		}
	}

	for (int i = m_nNumBestCan - 1; i >= 0; i--)
	{
		PAIR<double> p;
		pq.popMin(p);
		next_idx[i] = p.index;
		next_errors[i] = p.distance;
	}
}


void IndexEncoding::SolveOptimizedAdv2Recursive(
	const Vector<double> &vec_x_map, 
	const SMatrix<double> &matInnerProduct, 
	int idx, 
	short* prepresentation, 
	double &error) const
{
	if (idx >= m_nNumberDictionaryEachPartition)
	{
		return;
	}
	short next_idx[MAX_NUM_CANDIDATE];
	double next_errors[MAX_NUM_CANDIDATE];
	BestNextWordsSMart(vec_x_map, matInnerProduct, 
		prepresentation, idx, 
		next_idx, next_errors);

	double min_error = DBL_MAX;
	short min_representation[MAX_SPARSITY];

	for (int i = 0; i < m_nNumBestCan; i++)
	{
		*(prepresentation + idx) = next_idx[i];

		next_errors[i] += error;
		SolveOptimizedAdv2Recursive(vec_x_map, 
			matInnerProduct, 
			idx + 1, 
			prepresentation, 
			next_errors[i]);

		if (min_error > next_errors[i])
		{
			min_error = next_errors[i];
			memcpy(min_representation, prepresentation, sizeof(short) * m_nNumberDictionaryEachPartition);
		}
	}
	error = min_error;
	memcpy(prepresentation, min_representation, sizeof(short) * m_nNumberDictionaryEachPartition);
}

void IndexEncoding::SolveOptimizedAdvRecursive(const double* pz,
											   const SMatrix<double>& matDictionary,
											   int idx,
											   short* prepresentation,
											   double &error) const
{
	int sub_dic_size_each_partition = matDictionary.Rows() / m_nNumberDictionaryEachPartition;

	int idx_start = idx * sub_dic_size_each_partition;

	if (idx == m_nNumberDictionaryEachPartition - 1) // just finding the best case is OK
	{
		BestNextWord(pz, matDictionary, idx_start, idx_start + sub_dic_size_each_partition,
			error, prepresentation[0]);
	}
	else
	{
		Vector<short> vec_idx;
		SMatrix<double> mat_diff;
		double best_error = DBL_MAX;
		int best_idx;

		BestNextWords(pz, matDictionary, idx_start, idx_start + sub_dic_size_each_partition,
			mat_diff, vec_idx);

		//SMART_ASSERT(0)(vec_idx).Exit();

		double tmp_error;
		Vector<short> vec_representation(m_nNumberDictionaryEachPartition - idx - 1);
		Vector<short> vec_best_representation(m_nNumberDictionaryEachPartition - idx - 1);

		for (int i = 0; i < m_nNumBestCan; i++)
		{
			SolveOptimizedAdvRecursive(mat_diff[i],
				matDictionary,
				idx + 1,
				vec_representation.Ptr(),
				tmp_error);

			if (tmp_error < best_error)
			{
				best_error = tmp_error;
				prepresentation[0] = vec_idx[i];

				//SMART_ASSERT (prepresentation[0] >= 0 
				//	&& prepresentation[0] < 512)(prepresentation[0]).Exit();


				vec_representation.CopyTo(vec_best_representation);
			}
		}
		error = best_error;
		memcpy(prepresentation + 1, 
			vec_best_representation.Ptr(), 
			sizeof(short) * (m_nNumberDictionaryEachPartition - idx - 1));
	}
}

//void IndexEncoding::SolveOptimizedAdv(const double* pz,
//											   const SMatrix<double>& matDictionary,
//											   short* prepresentation) const
//{
//	double e;
//
//	SolveOptimizedAdvRecursive(pz, matDictionary, 0, prepresentation, e);
//}

void IndexEncoding::PrecomputeA(
	int idx_dic_a, int idx_dic_b, 
	const Vector<double> &vec_x_map, 
	const SMatrix<double> &matPrecomputed, 
	int num_dic_each_partition, 
	short* prepresentation, 
	Vector<double> &vec_pre_a, int sub_dic_size) const
{
	vec_pre_a.AllocateSpace(sub_dic_size);
	int off_set_a = idx_dic_a * sub_dic_size;
	int off_set_b = idx_dic_b * sub_dic_size;

	for (int i = 0; i < sub_dic_size; i++)
	{
		int idx_word = off_set_a + i;
		double s = matPrecomputed[idx_word][idx_word];
		for (int j = 0; j < num_dic_each_partition; j++)
		{
			if (j == idx_dic_a || j == idx_dic_b)
			{
				continue;
			}
			int curr_idx = prepresentation[j];
			if (curr_idx != -1)
			{
				s += matPrecomputed[idx_word][curr_idx];
			}
		}
		s -= vec_x_map[idx_word];
		vec_pre_a[i] = s;
	}
}

bool IndexEncoding::SolveOptimizedAdv3Twice(
	int idx_dic_a, int idx_dic_b,
	const Vector<double> &vec_x_map, 
	const SMatrix<double> &matPrecomputed, 
	int num_dic_each_partition, 
	short* prepresentation) const
{
	Vector<double> vec_pre_a;
	Vector<double> vec_pre_b;

	const int sub_dic_size = matPrecomputed.Rows() / num_dic_each_partition;
	int off_set_a = idx_dic_a * sub_dic_size;
	int off_set_b = idx_dic_b * sub_dic_size;

	PrecomputeA(idx_dic_a, idx_dic_b, vec_x_map, matPrecomputed, num_dic_each_partition, prepresentation, 
		vec_pre_a, sub_dic_size);

	PrecomputeA(idx_dic_b, idx_dic_a, vec_x_map, matPrecomputed, num_dic_each_partition, prepresentation, 
		vec_pre_b, sub_dic_size);

	double best_s = DBL_MAX;
	int best_i = -1, best_j = -1;
	for (int i = 0; i < sub_dic_size; i++)
	{
		double ai = vec_pre_a[i];
		int off_ai = off_set_a + i;
		double* pdic = matPrecomputed[off_ai];
		for (int j = 0; j < sub_dic_size; j++)
		{
			int off_bj = off_set_b + j;
			double s = ai + vec_pre_b[j] + pdic[off_bj];
			if (s < best_s)
			{
				best_s = s;
				best_i = off_ai;
				best_j = off_bj;
			}
		}
	}


	if (prepresentation[idx_dic_a] == best_i && prepresentation[idx_dic_b] == best_j)
	{
		return false;
	}
	prepresentation[idx_dic_a] = best_i;
	prepresentation[idx_dic_b] = best_j;
	return true;
}

bool IndexEncoding::SolveOptimizedAdv3Twice(
	const Vector<double> &vec_x_map, 
	const SMatrix<double> &matPrecomputed, 
	int num_dic_each_partition, 
	short* prepresentation) const
{
	bool is_changed_return = false;

	const int sub_dic_size = matPrecomputed.Rows() / num_dic_each_partition;

	for (int i = 0; i < num_dic_each_partition; i++)
	{
		int j = i + 1;
		if (j >= num_dic_each_partition)
		{
			j = 0;
		}
		bool is_changed = SolveOptimizedAdv3Twice(i, j, 
			vec_x_map, matPrecomputed, 
			num_dic_each_partition, prepresentation);
		if (is_changed)
		{
			is_changed_return = true;
		}
	}

	return is_changed_return;
}

bool IndexEncoding::SolveOptimizedAdv3Once(
	const Vector<double> &vec_x_map, 
	const SMatrix<double> &matPrecomputed, 
	int num_dic_each_partition, 
	short* prepresentation) const
{
	int num_sub_centers = matPrecomputed.Rows() / num_dic_each_partition;

	bool is_changed = false;

	int idx_start_center = 0;
	for (int idx_sub_cluster = 0; idx_sub_cluster < num_dic_each_partition; idx_sub_cluster++)
	{
		int idx_end_center = idx_start_center + num_sub_centers;

		double best_value = DBL_MAX;
		short best_idx = -1;

		for (int idx_center = idx_start_center; idx_center < idx_end_center; idx_center++)
		{
			double s = 0;
			for (int idx_sub_cluster2 = 0; idx_sub_cluster2 < num_dic_each_partition; idx_sub_cluster2++)
			{
				if (idx_sub_cluster == idx_sub_cluster2)
				{
					s += matPrecomputed[idx_center][idx_center];
				}
				else
				{
					int curr_idx = prepresentation[idx_sub_cluster2];
					if (curr_idx != -1)
					{
						s += matPrecomputed[idx_center][curr_idx];
					}
				}
			}
			s -= vec_x_map[idx_center];
			if (s < best_value)
			{
				best_value = s;
				best_idx = idx_center;
			}
		}
		SMART_ASSERT(best_idx != -1).Exit();
		if (prepresentation[idx_sub_cluster] != best_idx)
		{
			is_changed = true;
			prepresentation[idx_sub_cluster] = best_idx;
		}
		idx_start_center = idx_end_center;
	}

	return is_changed;
}



double residual_distance(const Vector<double> &vec_x_map, int idx_center, int idx_sub_cluster, 
						 const short* prepresentation, int num_dic_each_partition,
						 const SMatrix<double> &matPrecomputed)
{
	//static int x = 0;

	/*if (x == 256 * 4)
	{
		PRINT << Vector<short>((short*)prepresentation, 4) << "\n" << idx_center << "\n";
	}*/

	double s = 0;
	for (int i = 0; i < num_dic_each_partition; i++)
	{
		int i_idx = prepresentation[i];
		if (i == idx_sub_cluster && i_idx == -1)
		{
			i_idx = idx_center;
		}
		else if (i_idx == -1)
		{
			continue;
		}
		s += matPrecomputed[i_idx][i_idx];
		/*if (x == 256 * 4)
		PRINT << matPrecomputed[i_idx][i_idx] << "\n";*/
		s -= vec_x_map[i_idx];
		//if (x == 256 * 4)
		//PRINT << vec_x_map[i_idx] << "\n";
		for (int j = i + 1; j < num_dic_each_partition; j++)
		{
			int j_idx = prepresentation[j];
			if (j_idx == -1 && j == idx_sub_cluster)
			{
				j_idx = idx_center;
			}
			else if (j_idx == -1)
			{
				continue;
			}
			s += matPrecomputed[i_idx][j_idx];
			/*if (x == 256 * 4)
			PRINT << matPrecomputed[i_idx][j_idx] << "\n";*/
		}
	}
	//if (x == 256 * 4)
	//	PRINT << s << "\t" << vec_x_map[idx_center] << "\n";
	//x++;
	return s;
}

int IndexEncoding::SolveAdditiveQuantization(const Vector<double> &vec_x_map, 
											 const SMatrix<double> &matPrecomputed, 
											 int num_dic_each_partition, 
											 short* prepresentation) const
{
	int num = 0;

	bool is_changed;
	int num_sub_centers = matPrecomputed.Rows() / num_dic_each_partition;

	SMatrix<short>* p_curr;
	SMatrix<short>* p_aux;
	p_curr = new SMatrix<short>(m_nNumBestCan, num_dic_each_partition);
	p_aux = new SMatrix<short>(m_nNumBestCan, num_dic_each_partition);

	p_curr->SetValue(-1);

	{ // find the best m_aq_candidate number of points. 
		int idx_center = 0;
		Heap<GreatPair<double> > heap;
		heap.Reserve(m_nNumBestCan);
		for (int idx_sub_cluster = 0; idx_sub_cluster < num_dic_each_partition; idx_sub_cluster++)
		{
			for (int i = 0; i < num_sub_centers; i++)
			{
				double s = residual_distance(vec_x_map, idx_center, idx_sub_cluster, 
					(*p_curr)[0], num_dic_each_partition, matPrecomputed);

				if (heap.size() < m_nNumBestCan)
				{
					heap.insert(GreatPair<double>(idx_center, s));
				}
				else
				{
					const GreatPair<double> &top = heap.Top();
					if (top.distance > s)
					{
						heap.popMin();
						heap.insert(GreatPair<double>(idx_center, s));
					}
				}
				idx_center++;
			}
		}
		SMART_ASSERT(heap.size() == m_nNumBestCan).Exit();
		for (int i = 0; i < m_nNumBestCan; i++)
		{
			GreatPair<double> v;
			heap.popMin(v);
			//PRINT << v.index << "\t" << v.distance << "\n";
			(*p_curr)[i][v.index / num_sub_centers] = v.index;
		}
		//PRINT << *p_curr << "\n";
	}

	int idx_start_center = 0;
	for (int idx_sub_cluster1 = 1; idx_sub_cluster1 < num_dic_each_partition; idx_sub_cluster1++)
	{
		priority_queue<Triplet<int, int, double>, 
			vector<Triplet<int, int, double> >,  
			LessTripletThird<int, int, double> > heap;

		for (int idx_can = 0; idx_can < m_nNumBestCan; idx_can++)
		{
			short* curr_presentation = (*p_curr)[idx_can];
			for (int idx_sub_cluster2 = 0; idx_sub_cluster2 < num_dic_each_partition; idx_sub_cluster2++)
			{
				if (curr_presentation[idx_sub_cluster2] == -1)
				{
					int idx_start_center = idx_sub_cluster2 * num_sub_centers;
					int idx_end_center = idx_start_center + num_sub_centers;
					for (int idx_center = idx_start_center; idx_center < idx_end_center; idx_center++)
					{

						double s = residual_distance(vec_x_map, idx_center, idx_sub_cluster2, curr_presentation, 
							num_dic_each_partition, matPrecomputed);

						if (heap.size() < m_nNumBestCan)
						{
							heap.push(Triplet<int, int, double>(idx_can, idx_center, s));
						}
						else
						{
							const Triplet<int, int, double> &top = heap.top();
							if (top.third > s)
							{
								heap.pop();
								heap.push(Triplet<int, int, double>(idx_can, idx_center, s));
							}
						}
					}
				}
			}
		}
		SMART_ASSERT(m_nNumBestCan == heap.size())(heap.size()).Exit();
		for (int idx_can = 0; idx_can < m_nNumBestCan; idx_can++)
		{
			const Triplet<int, int, double> &top = heap.top();
			int idx_origin_can = top.first;
			const short* p_origin = p_curr->operator[](idx_origin_can);
			short* p_now = p_aux->operator[](idx_can);
			//PRINT << top.first << "\t" << top.second << "\t" << top.third << "\n";
			memcpy(p_now, p_origin, sizeof(short) * num_dic_each_partition);
			p_now[top.second / num_sub_centers] = top.second; 
			heap.pop();
		}
		swap(p_curr, p_aux);
		//PRINT << *p_curr;
	}
	memcpy(prepresentation, p_curr->operator[](m_nNumBestCan - 1), sizeof(short) * num_dic_each_partition);
	//PRINT << *p_curr;

	delete p_curr;
	delete p_aux;

	return false;
}

int IndexEncoding::SolveOptimizedAdv3(
	const Vector<double> &vec_x_map, 
	const SMatrix<double> &matPrecomputed, 
	int num_dic_each_partition, 
	short* prepresentation) const
{
	int num = 0;
	if (m_isInitialize)
	{
		for (int i = 0; i < num_dic_each_partition; i++)
		{
			prepresentation[i] = -1;
		}
	}

	bool is_changed;

	////VIPjjjkkk
	if (m_nNumberOfGroup == 1)
	{
		do
		{
			num++;
			is_changed = SolveOptimizedAdv3Once(vec_x_map, 
				matPrecomputed, 
				num_dic_each_partition, 
				prepresentation);
		} 
		while(is_changed);
	}
	else if (m_nNumberOfGroup == 2)
	{
		do
		{
			num++;
			is_changed = SolveOptimizedAdv3Twice(
				vec_x_map, 
				matPrecomputed, 
				num_dic_each_partition, 
				prepresentation);
		}while(is_changed);
	}
	else
	{
		SMART_ASSERT(0).Exit();
	}
	return num;
}

int IndexEncoding::SolveAdditiveQuantization(
	const double* pz, 
	const SMatrix<double> &matDictionary, 
	int num_dic_each_partition, 
	const SMatrix<double> &matPrecomputed, 
	short* prepresentation)
{
	int num_centers = matDictionary.Rows();
	int dim = matDictionary.Cols();
	Vector<double> vec_x_map(num_centers);
	for (int i = 0; i < num_centers; i++)
	{
		vec_x_map[i] = dot(pz, matDictionary[i], dim);
	}

	return SolveAdditiveQuantization(vec_x_map, matPrecomputed, num_dic_each_partition, prepresentation);
}

int IndexEncoding::SolveOptimizedAdv3(
	const double* pz,
	const SMatrix<double> &matDictionary, 
	int num_dic_each_partition,
	const SMatrix<double> &matPrecomputed,
	short* prepresentation) const
{
	int num_centers = matDictionary.Rows();
	int dim = matDictionary.Cols();
	Vector<double> vec_x_map(num_centers);
	for (int i = 0; i < num_centers; i++)
	{
		vec_x_map[i] = dot(pz, matDictionary[i], dim);
	}

	return SolveOptimizedAdv3(vec_x_map, matPrecomputed, num_dic_each_partition, prepresentation);
}

int IndexEncoding::SolveOptimizedAdv2(const double* pz,
									  const SMatrix<double>& matDictionary, 
									  const SMatrix<double>& matInnerProduct,
									  short* prepresentation) const
{
	double e = 0;

	int num_centers = matDictionary.Rows();

	Vector<double> vec_x_map(num_centers);
	for (int i = 0; i < num_centers; i++)
	{
		vec_x_map[i] = dot(pz, matDictionary[i], matDictionary.Cols());
	}

	Vector<short> vec_pre(m_nNumberDictionaryEachPartition);
	if (!m_isInitialize)
	{
		memcpy(vec_pre.Ptr(), prepresentation, sizeof(short) * m_nNumberDictionaryEachPartition);
	}

	SolveOptimizedAdv2Recursive(vec_x_map, matInnerProduct, 0, prepresentation, e);

	int is_not_changed = 0;
	if (!m_isInitialize)
	{
		Vector<double> vec_residual(matDictionary.Cols());
		memcpy(vec_residual.Ptr(), pz, sizeof(double) * matDictionary.Cols());
		for (int i = 0; i < m_nNumberDictionaryEachPartition; i++)
		{
			VectorMinus(vec_residual.Ptr(), matDictionary[vec_pre[i]], matDictionary.Cols());
		}
		double pre_value = vec_residual.Norm2Squared();


		memcpy(vec_residual.Ptr(), pz, sizeof(double) * matDictionary.Cols());
		for (int i = 0; i < m_nNumberDictionaryEachPartition; i++)
		{
			VectorMinus(vec_residual.Ptr(), matDictionary[prepresentation[i]], matDictionary.Cols());
		}
		double after_value = vec_residual.Norm2Squared();

		if (after_value > pre_value)
		{
			is_not_changed = 1;
			memcpy(prepresentation, vec_pre.Ptr(), sizeof(short) * m_nNumberDictionaryEachPartition);
		}
	}
	return is_not_changed;
}

void IndexEncoding::SolveKMeans(const double* pz,
								const SMatrix<double>& matDictionary,
								short* prepresentation) const
{
	double best_error;
	int best_idx;

	BestNextWord(pz, matDictionary, best_error, best_idx);
	prepresentation[0] = best_idx;
}

void IndexEncoding::SetNumberGroup(int num_grouped)
{
	m_nNumberOfGroup = num_grouped;
	SMART_ASSERT(m_nNumberOfGroup == 1 || m_nNumberOfGroup == 2).Exit();
}

void IndexEncoding::SetEncodingType(TypeEncoding type_encoding)
{
	m_type_encoding = type_encoding;

	PRINT << m_type_encoding << "\n";

}

//void IndexEncoding::SetIsOCK(int is_ock)
//{
//	SMART_ASSERT(0)("use SetEncodingType()");
//	m_is_ockmeans = is_ock;
//	SMART_ASSERT(m_is_ockmeans == 1 || m_is_ockmeans == 0).Exit();
//}

void IndexEncoding::SetIsInitialize(bool is_initialize)
{
	m_isInitialize = is_initialize;
}

void IndexEncoding::SetCandidateNumber(int num_can)
{
	m_nNumBestCan = num_can;
}

void IndexEncoding::GKMeansPreprocess(
	const SMatrix<double> &matDictionary, 
	int num_dic_each_partition, 
	SMatrix<double>  &matPrecomputed) const
{
	int num_all_centers = matDictionary.Rows();

	matPrecomputed.AllocateSpace(num_all_centers, num_all_centers);

	int dim = matDictionary.Cols();

	for (int i = 0; i < num_all_centers; i++)
	{
		for (int j = 0; j < num_all_centers; j++)
		{
			const double* pi = matDictionary[i];
			const double* pj = matDictionary[j];

			matPrecomputed[i][j] = dot(pi, pj, dim);
		}
	}

	for (int i = 0; i < num_all_centers; i++)
	{
		matPrecomputed[i][i] = matPrecomputed[i][i] * 0.5;
	}

}

void IndexEncoding::GKMeansPreprocess(
	const Vector<SMatrix<double> >& vecmatDictionary,
	int num_dic_each_partition,
	Vector<SMatrix<double> > & vecmatPrecomputed) const
{
	int num_partitions = vecmatDictionary.size();
	vecmatPrecomputed.AllocateSpace(num_partitions);

	for (int i = 0; i < num_partitions; i++)
	{
		const SMatrix<double> &matDictionary = vecmatDictionary[i];
		SMatrix<double > &matPrecomputed = vecmatPrecomputed[i];

		GKMeansPreprocess(matDictionary, num_dic_each_partition, matPrecomputed);
	}

}

void IndexEncoding::MultiDictionaryPreprocess(
	const Vector<SMatrix<double> > &vecmatDictionary, 
	int num_dic_each_partition, 
	Vector<SMatrix<double> > &vecmatPrecomputed)  const
{
	int num_partitions = vecmatDictionary.size();

	vecmatPrecomputed.AllocateSpace(num_partitions);

	for (int i = 0; i < num_partitions; i++)
	{
		const SMatrix<double> &matDictionary = vecmatDictionary[i];
		SMatrix<double> &matPrecomputed = vecmatPrecomputed[i];

		MultiDictionaryPreprocess(matDictionary, num_dic_each_partition, matPrecomputed);
	}
}

void IndexEncoding::MultiDictionaryPreprocess(const SMatrix<double> &matDictionary, 
											  int num_dic_each_partition, 
											  SMatrix<double> &matPrecomputed) const
{
	int num_all_words = matDictionary.Rows();
	int dim = matDictionary.Cols();

	matPrecomputed.AllocateSpace(num_all_words, num_all_words);
	for (int i = 0; i < num_all_words; i++)
	{
		for (int j = 0; j <= i; j++)
		{
			matPrecomputed[i][j] = dot(
				matDictionary[i], 
				matDictionary[j], 
				dim);
		}
	}
	for (int i = 0; i < num_all_words; i++)
	{
		for (int j = i + 1; j < num_all_words; j++)
		{
			matPrecomputed[i][j] = matPrecomputed[j][i];
		}
	}
}

//void IndexEncoding::Solve(const SMatrix<double>& matZ,
//						  const Vector<SMatrix<double> >& vecmatDictionary,
//						  int num_dic_each_partition,
//						  SMatrix<short>& matRepresentation)
//{
//	SMART_ASSERT(0)("should be carefully check").Exit();
//	//m_pvecMatDictionary = &vecmatDictionary;
//	m_nNumPoint = matZ.Rows();
//
//	int num_partitions = vecmatDictionary.size();
//
//	SMART_ASSERT(m_nNumPoint == matRepresentation.Rows())(matRepresentation.Rows()).Exit();
//
//	Vector<SMatrix<double> > vecmatPrecomputed;
//
//	if (num_dic_each_partition > 1) // for ock-means in the TKDE paper
//	{
//		switch (m_type_encoding)
//		{
//		case Type_ock:
//			MultiDictionaryPreprocess(vecmatDictionary, num_dic_each_partition, vecmatPrecomputed);
//			break;
//		case Type_gk_means:
//		case Type_additive_quantization:
//			GKMeansPreprocess(vecmatDictionary, num_dic_each_partition, vecmatPrecomputed);
//			break;
//		default:
//			break;
//		}
//		//
//		//m_vecNumberIterativeAssignment.AllocateSpace(m_nNumPoint);
//		//m_vecNumberIterativeAssignment.SetValueZeros();
//	}
//	m_nNumberDictionaryEachPartition = num_dic_each_partition;
//	m_nSubDicSizeEachPartition = vecmatDictionary[0].Rows() / num_dic_each_partition;
//	for (int idx_partition = 1; idx_partition < num_partitions; idx_partition++)
//	{
//		SMART_ASSERT(vecmatDictionary[idx_partition].Rows() == vecmatDictionary[0].Rows());
//	}
//	int num_not_changed = 0;
//#pragma omp parallel for
//	for (int i = 0; i < m_nNumPoint; i++)
//	{
//		const double* pz = matZ[i];
//		short* prepresentation = matRepresentation[i];
//
//		for (int idx_partition = 0; idx_partition < num_partitions; idx_partition++)
//		{
//			const SMatrix<double> &matDictionary = vecmatDictionary[idx_partition];
//
//			for (int j = 1; j < num_dic_each_partition; j++)
//			{
//				*(prepresentation + j) += j * m_nSubDicSizeEachPartition;
//			}
//
//			if (num_dic_each_partition == 1)
//			{
//				SolveKMeans(pz, matDictionary, prepresentation);
//			}
//			else
//			{
//				const SMatrix<double> &matPrecomputed = vecmatPrecomputed[idx_partition];
//				switch (m_type_encoding)
//				{
//				case Type_ock:
//					{
//						int is_not_changed = SolveOptimizedAdv2(pz, matDictionary, 
//							matPrecomputed, 
//							prepresentation);
//						if (is_not_changed)
//						{
//#pragma omp atomic 
//							num_not_changed++;
//						}
//						break;
//					}
//				case Type_gk_means:
//					SolveOptimizedAdv3(pz, 
//						matDictionary, num_dic_each_partition, matPrecomputed, prepresentation);
//					break;
//				default:
//					SMART_ASSERT(0).Exit();
//				}
//			}
//
//			for (int j = 1; j < num_dic_each_partition; j++)
//			{
//				*(prepresentation + j) -= j * m_nSubDicSizeEachPartition;
//			}
//
//			pz += matDictionary.Cols();
//			prepresentation += num_dic_each_partition;
//		}
//	}
//
//	if (num_not_changed)
//	{
//		PRINT << "not changed: " << num_not_changed << "\n";
//	}
//}

void IndexEncoding::Solve(const SMatrix<double>& matZ,
						  const Vector<SMatrix<double> >& vecmatDictionary,
						  int num_dic_each_partition,
						  SMatrix<unsigned char>& matRepresentation)
{
	//m_pvecMatDictionary = &vecmatDictionary;
	m_nNumPoint = matZ.Rows();

	int num_partitions = vecmatDictionary.size();

	SMART_ASSERT(m_nNumPoint == matRepresentation.Rows())(matRepresentation.Rows()).Exit();

	Vector<SMatrix<double> > vecmatPrecomputed;

	if (num_dic_each_partition > 1) // for ock-means in the TKDE paper
	{
		switch(m_type_encoding)
		{
		case Type_ock:
			MultiDictionaryPreprocess(vecmatDictionary, num_dic_each_partition, vecmatPrecomputed);
			break;
		case Type_gk_means:
		case Type_additive_quantization:
			GKMeansPreprocess(vecmatDictionary, num_dic_each_partition, vecmatPrecomputed);
			break;
		}
		//
		//m_vecNumberIterativeAssignment.AllocateSpace(m_nNumPoint);
		//m_vecNumberIterativeAssignment.SetValueZeros();
	}
	m_nNumberDictionaryEachPartition = num_dic_each_partition;
	m_nSubDicSizeEachPartition = vecmatDictionary[0].Rows() / num_dic_each_partition;
	for (int idx_partition = 1; idx_partition < num_partitions; idx_partition++)
	{
		SMART_ASSERT(vecmatDictionary[idx_partition].Rows() == vecmatDictionary[0].Rows());
	}
	int num_not_changed = 0;
#pragma omp parallel for
	for (int i = 0; i < m_nNumPoint; i++)
	{
		const double* pz = matZ[i];
		unsigned char* prepresentation_org = matRepresentation[i];

		Vector<short> vec_representation(m_nSubDicSizeEachPartition);
		short* prepresentation = vec_representation.Ptr();

		for (int idx_partition = 0; idx_partition < num_partitions; idx_partition++)
		{
			const SMatrix<double> &matDictionary = vecmatDictionary[idx_partition];

			for (int j = 0; j < num_dic_each_partition; j++)
			{
				*(prepresentation + j) = (short)(*(prepresentation_org + j)) + j * m_nSubDicSizeEachPartition;
			}

			if (num_dic_each_partition == 1)
			{
				SolveKMeans(pz, matDictionary, prepresentation);
			}
			else
			{
				const SMatrix<double> &matPrecomputed = vecmatPrecomputed[idx_partition];
				switch(m_type_encoding)
				{
				case Type_ock:
					{
						int is_not_changed = SolveOptimizedAdv2(pz, matDictionary, 
							matPrecomputed, 
							prepresentation);
						if (is_not_changed)
						{
#pragma omp atomic 
							num_not_changed++;
						}
						break;
					}
				case Type_gk_means:
					//m_vecNumberIterativeAssignment[i] = 
					SolveOptimizedAdv3(pz, 
						matDictionary, num_dic_each_partition, matPrecomputed, prepresentation);
					break;
				case Type_additive_quantization:
					SolveAdditiveQuantization(pz, 
						matDictionary, num_dic_each_partition, matPrecomputed, prepresentation);
					break;
				}
			}

			for (int j = 0; j < num_dic_each_partition; j++)
			{
				*(prepresentation_org + j) = *(prepresentation + j) - j * m_nSubDicSizeEachPartition;
			}

			pz += matDictionary.Cols();
			prepresentation_org += num_dic_each_partition;
		}
	}

	if (num_not_changed)
	{
		PRINT << "not changed: " << num_not_changed << "\n";
	}
	//for (int i = 0; i < m_vecNumberIterativeAssignment.size(); i++)
	//{
	//	if (m_vecNumberIterativeAssignment[i] > 1)
	//	{
	//		PRINT << m_vecNumberIterativeAssignment[i] << "\n";
	//	}
	//}



}

