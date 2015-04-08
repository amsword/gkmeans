#pragma once
#include <omp.h>
#include <time.h>
#include <utility>
#include <cfloat>
#include <queue>

#include "utility_macro.h"
#include "VirtualData.h"
#include "PriorityQueue.h"
#include "utility_memory.h"
#include "utility_math_matrix.h"
#include "utility_sort.h"
#include "utility_math.h"

#include "FileHeadReader.h"

namespace utility
{
	template <class typeData>
	// not fully test
	class BruteforceKNNer
	{
	public:
		BruteforceKNNer() {}
		~BruteforceKNNer() {}

	public:
		void NearestEuclidean(
			const double* p_query, 
			const SMatrix<double>& matDataBase,
			int &best_idx,
			double &best_error);

		void LargetInnerProduct(
			const double* p_query, 
			const SMatrix<double>& matDataBase,
			int &best_idx,
			double &best_value);

		void RunHugeDataBase(const string& str_query_file_name, 
			const string& str_data_base_file_name,
			int k_nn,
			const string& str_save_file_name);

	public:		

		void RunHeap(const SMatrix<typeData> &mat_query,
			const SMatrix<typeData> &mat_data_base,
			int offset_data_base,
			int k,
			Heap<PAIR<typeData> >* p_top_knn);

		void Heap2SortedArray(
			Heap<PAIR<typeData> >* &p_top_knn, 
			int** &pp_idx, 
			typeData** &pp_dist, 
			int num_heap, 
			int k_nn);

		void Heap2SaveFile(Heap<PAIR<typeData> >* p_top_knn, int heap_num, const int k_nn, const string& str_save_file);
	};

	template <class typeData>
	void BruteforceKNNer<typeData>::NearestEuclidean(
		const double* p_query, const SMatrix<double>& matDataBase, 
		int &best_idx,
		double &best_error)
	{
		int num_point = matDataBase.Rows();
		int dim = matDataBase.Cols();

		best_error = FLT_MAX;
		best_idx = -1;
		double d;
		for (int i = 0; i < num_point; i++)
		{
			d = squared_distance(p_query, matDataBase[i], dim);
			if (d < best_error)
			{
				best_error = d;
				best_idx = i;
			}
		}

		SMART_ASSERT(best_idx != -1)(d)(best_error).Exit();


	}

	template <class typeData>
	void BruteforceKNNer<typeData>::LargetInnerProduct(
		const double* p_query, 
		const SMatrix<double>& matDataBase,
		int &best_idx,
		double &best_value)
	{
		int num_point = matDataBase.Rows();
		int dim = matDataBase.Cols();

		best_value = -10000000;
		best_idx = -1;
		double d;
		for (int i = 0; i < num_point; i++)
		{
			d = dot(p_query, matDataBase[i], dim);
			if (d > best_value)
			{
				best_value = d;
				best_idx = i;
			}
		}
		SMART_ASSERT(best_idx != -1)(best_idx).Exit();
	}


	template <class typeData>
	void RunOneHeap(
		int i,
		const typeData** pp_query, int num_query,
		const typeData** pp_data_base, int num_data_base,
		int offset_data_base,
		int dim,
		const int k_nn,
		Heap<PAIR<typeData> >* p_top_knn)
	{
		Heap<PAIR<typeData> >& heap_knn = p_top_knn[i];

		const typeData* tmp_p_query = pp_query[i];
		for (int j = 0; j < num_data_base; j++)
		{
			PAIR<typeData> node;

#ifdef BFKNNSimNegative
			node.distance = -utility::squared_distance(tmp_p_query,
				pp_data_base[j], dim);
#else
			node.distance = utility::squared_distance(tmp_p_query,
				pp_data_base[j], dim);
#endif
			if (heap_knn.size() < k_nn)
			{
				node.index = j + offset_data_base;
				heap_knn.insert(node);
			}
			else
			{
				const PAIR<typeData>& top = heap_knn.Top();
				if (top < node)
				{
					node.index = j + offset_data_base;
					heap_knn.popMin();
					heap_knn.insert(node);
				}
			}
		}
	}


	template <class typeData>
	void RunOneHeap(
		const typeData* p_query,
		const SMatrix<typeData> &mat_data_base,
		int offset_data_base,
		const int k_nn,
		Heap<PAIR<typeData> >& heap_knn)
	{
		int num_data_base = mat_data_base.Rows();
		int dim = mat_data_base.Cols();
		for (int j = 0; j < num_data_base; j++)
		{
			PAIR<typeData> node;

#ifdef BFKNNSimNegative
			node.distance = -utility::squared_distance(tmp_p_query,
				pp_data_base[j], dim);
#else
			node.distance = utility::squared_distance(p_query,
				mat_data_base[j], dim);
#endif
			if (heap_knn.size() < k_nn)
			{
				node.index = j + offset_data_base;
				heap_knn.insert(node);
			}
			else
			{
				const PAIR<typeData>& top = heap_knn.Top();
				if (top < node)
				{
					node.index = j + offset_data_base;
					heap_knn.popMin();
					heap_knn.insert(node);
				}
			}
		}
	}


	template<class typeData>
	void BruteforceKNNer<typeData>::RunHeap(const SMatrix<typeData> &mat_query,
		const SMatrix<typeData> &mat_data_base,
		int offset_data_base,
		int k_nn,
		Heap<PAIR<typeData> >* p_top_knn)
	{
		int num_query = mat_query.Rows();
		int count = 0;
#pragma omp parallel for 
		for (int i = 0; i < num_query; ++i)
		{
#pragma omp atomic
			count++;

			if ((count % 500) == 0)
			{
				PRINT << ::omp_get_thread_num() << ": " << count << "\t" << "offset: " << offset_data_base << "\n";
			}

			RunOneHeap(mat_query[i], mat_data_base, offset_data_base, k_nn, p_top_knn[i]);
		}
	}

	template <class typeData>
	void InitializeHeap(Heap<PAIR<typeData> >* &p_top_knn, int num_query, int k_nn)
	{
		p_top_knn = new Heap<PAIR<typeData> >[num_query];
		EXIT_ASSERT(p_top_knn);
		for (int i = 0; i < num_query; i++)
		{
			p_top_knn[i].Reserve(k_nn);
		}
	}

		template <class T>
	void InitializeHeap(Heap<T >* &p_top_knn, int num_query, int k_nn)
	{
		p_top_knn = new Heap<T >[num_query];
		EXIT_ASSERT(p_top_knn);
		for (int i = 0; i < num_query; i++)
		{
			p_top_knn[i].Reserve(k_nn);
		}
	}

	template <class typeData>
	void UninitializeHeap(Heap<PAIR<typeData> >* &p_top_knn)
	{
		if (p_top_knn)
		{
			delete[] p_top_knn;
			p_top_knn = NULL;
		}
	}
	template <class T>
	void UninitializeHeap(Heap<T >* &p_top_knn)
	{
		if (p_top_knn)
		{
			delete[] p_top_knn;
			p_top_knn = NULL;
		}
	}



	template<class typeData>
	void BruteforceKNNer<typeData>::Heap2SaveFile(
		Heap<PAIR<typeData> >* p_top_knn, int heap_num, const int k_nn, const string& str_save_file)
	{
		int* p_ulti_idx = new int[k_nn];
		EXIT_ASSERT(p_ulti_idx);
		typeData* p_ulti_dist = new typeData[k_nn];
		EXIT_ASSERT(p_ulti_dist);

		FILE* fp = fopen(str_save_file.c_str(), "wb");
		EXIT_ASSERT(fp);
		fwrite(&heap_num, sizeof(int), 1, fp);
		fwrite(&k_nn, sizeof(int), 1, fp);
		for (int i = 0; i < heap_num; i++)
		{
			Heap<PAIR<typeData> >& heap_knn = p_top_knn[i];
			int k = k_nn - 1;
			while (!heap_knn.empty())
			{
				PAIR<typeData> node;
				heap_knn.popMin(node);
				p_ulti_idx[k] = node.index;
				p_ulti_dist[k] = node.distance;
				k--;
			}	

			SMART_ASSERT(k == -1)(k).Exit();
			fwrite(&k_nn, sizeof(int), 1, fp);
			fwrite(p_ulti_idx, sizeof(int), k_nn, fp); // index of the NN
			fwrite(p_ulti_dist, sizeof(typeData), k_nn, fp); // squared distance to the query
		}
		fclose(fp);

		delete[] p_ulti_dist;
		delete[] p_ulti_idx;
	}

	template<class typeData>
	void Heap2Vector(Heap<PAIR<typeData> > &heap_knn, Vector<pair<int, typeData> > &vec_nn)
	{
		vec_nn.AllocateSpace(heap_knn.size());
		int k = heap_knn.size() - 1;
		while (!heap_knn.empty())
		{
			PAIR<typeData> node;
			heap_knn.popMin(node);
			vec_nn[k].first = node.index;
			vec_nn[k].second = node.distance;
			k--;
		}	
	}

	template<class typeData>
	void Heap2Vector(Heap<PAIR<typeData> > &heap_knn, Vector<int>* p_vec_idx, Vector<typeData>* p_vec_dist)
	{
		if (p_vec_idx) p_vec_idx->AllocateSpace(heap_knn.size());
		if (p_vec_dist) p_vec_dist->AllocateSpace(heap_knn.size());
		int k = heap_knn.size() - 1;
		while (!heap_knn.empty())
		{
			PAIR<typeData> node;
			heap_knn.popMin(node);
			if (p_vec_idx) (*p_vec_idx)[k] = node.index;
			if (p_vec_dist) (*p_vec_dist)[k] = node.distance;
			k--;
		}	
	}

	template<class typeData>
	void BruteforceKNNer<typeData>::Heap2SortedArray(
		Heap<PAIR<typeData> >* &p_top_knn, 
		int** &pp_idx, 
		typeData** &pp_dist, 
		int num_heap, 
		int k_nn)
	{
		for (int i = 0; i < num_heap; i++)
		{
			int k = k_nn - 1;
			Heap<PAIR<typeData> >& heap_knn = p_top_knn[i];
			while (!heap_knn.empty())
			{
				PAIR<typeData> node;
				heap_knn.popMin(node);
				pp_idx[i][k] = node.index;
				pp_dist[i][k] = node.distance;
				k--;
			}
			EXIT_ASSERT(k == -1);
		}
	}

	template<class typeData>
	void BruteforceKNNer<typeData>::RunHugeDataBase(
		const string& str_query_file_name, 
		const string& str_data_base_file_name,
		int k_nn,
		const string& str_save_file_name)
	{
		SMatrix<typeData> mat_query;
		mat_query.LoadData(str_query_file_name);
		int num_query = mat_query.Rows();

		const int batch_size = 1000000;
		int num_point;
		int dim;
		GetDims(str_data_base_file_name, num_point, dim);

		Heap<PAIR<typeData> >* p_top_knn;

		PRINT << "Begin initializing heap" << "\n";
		InitializeHeap(p_top_knn, num_query, k_nn);
		PRINT << "Finish initializing heap\n";

		clock_t t_begin = clock();

		int batch_num = num_point / batch_size + 1;
		for (int i = 0; i < batch_num; i++)
		{
			int start = i * batch_size;
			int end = start + batch_size;

			if (start >= num_point)
			{
				break;
			}
			if (end > num_point)
			{
				end = num_point;
			}

			PRINT << "Loading data: " << start << "\t" << end << "\n";
			SMatrix<typeData> mat_data_base;
			mat_data_base.LoadData(str_data_base_file_name, start, end);
			PRINT << "loaded data: " << mat_data_base.Rows() << 
				"\t" << mat_data_base.Cols();

			RunHeap(mat_query, mat_data_base, start, k_nn, p_top_knn);
			clock_t t_end = clock();
			PRINT << i << "/" << batch_num << "\t" << (double)(t_end - t_begin) / CLOCKS_PER_SEC << "\n";

		}

		Heap2SaveFile(p_top_knn, num_query, k_nn, str_save_file_name);

		UninitializeHeap(p_top_knn);
	}


}
