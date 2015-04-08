#include "TypeConvert.h"
#include "ObjectiveValue.h"
#include "make_mex.h"
#include "Reconstructor.h"
#include <omp.h>
#include "main_point.h"

#define TYPE prhs[0]


#ifdef _WINDOWS
int main()
{
	main_exe_objective_gkmeans();
	//main_exe_objective();
	//main_exe_construct();
}
#endif




//compact B
void mexFunction(int nlhs, mxArray *plhs[], 
				 int nrhs, const mxArray *prhs[])
{
	int num_thd = omp_get_num_procs();
	omp_set_num_threads(num_thd);

	SMART_ASSERT(nrhs >= 1).Exit();

	int type;
	mexConvert(TYPE, type);
	if (type == 0)
	{
		ComputeObjectiveFunctionValue(nlhs, plhs, nrhs, prhs);
	}
	else if (type == 1)
	{
		ComputeMultipleAllDAllB(nlhs, plhs, nrhs, prhs);
	}
	
}

