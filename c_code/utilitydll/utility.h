#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <map>

//#include "utility_logger.h"
//#include "utility_assert.h"



#include "utility_memory.h"
#include "VirtualData.h"
#include "utility_macro.h"
#include "PriorityQueue2.h"
#include "BruteforceKNNer.h"

#include "utility_math.h"

#include "utility_sort.h"

#include "utility_init.h"

#include "ForwardIndex.h"
#include "InvertedIndex.h"

#include "FileName.h"
#include "ParamsBase.h"

#include "CalculateDistance.h"

#include "VarSpMatrix.h"
#include "BitVector.h"

#include "SearchEngine.h"
#include "Criterion.h"
#include "PerformanceOrganizer.h"
#include "LinearSearchEngine.h"
#include "DistanceCalculator.h"
#include "utility_global.h"
#include "PQDistance.h"
#include "PrecisionCriterion.h"
#include "EuclideanDistance.h"

#include "LessPair.h"
#include "Triplet.h"


//#ifdef MATLAB_MEX_FILE
#ifdef _WINDOWS
#pragma comment(lib, "../x64/Release/UtilityDll.lib")
//#pragma comment(lib, "../x64/Release/UtilityDll.lib")
//#else
//#pragma comment(lib, "../x64/Debug/utilityDll.lib")
//#endif
//#else
//#ifdef NDEBUG
//#pragma comment(lib, "../x64/Release/UtilityDllexe.lib")
//#else
//#pragma comment(lib, "../x64/Debug/UtilityDllexe.lib")
//#endif
#endif

#include "utility_types.h"

#include "Dense8Distance.h"

#include "DenseDistance.h"

#include "HammingDistance.h"
#include "DiagonalDistance.h"

#include "BinaryCodePartition.h"

#include "QueryOptDistance.h"
#include "FixedTopksCriterion.h"
#include "VariedTopkCriterion.h"
#include "AverageDistanceRatioCriterion.h"

#include "CKMeansDistance.h"

#include "ANNRetrieval.h"
