#pragma once

#include "make_mex.h"

#define MAT_Z prhs[1] 
#define MAT_DIC prhs[2]
#define BINARY_REPRESENTATION prhs[3]


#define INPUT_FIRST prhs[1]
#define INPUT_SECOND prhs[2]
#define INPUT_THIRD prhs[3]

#define RESULT plhs[0]


void ComputeObjectiveFunctionValue(int nlhs, mxArray *plhs[], 
				 int nrhs, const mxArray *prhs[]);

void ComputeMultipleAllDAllB(int nlhs, mxArray *plhs[], 
				 int nrhs, const mxArray *prhs[]);


int main_exe_construct();

int main_exe_objective_gkmeans();

int main_exe_objective();