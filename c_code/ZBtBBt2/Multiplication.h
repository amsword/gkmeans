#pragma once
#include "utility.h"

using namespace utility;

void computeZBt(const SMatrix<double> &matZ, const SMatrix<short> &matB, 
		 SMatrix<double> &matZBt);

void computeZBt(const SMatrix<double> &matZ, const SMatrix<unsigned char> &matB, 
		 SMatrix<double> &matZBt);

void computeBBt(const SMatrix<short> &matB, 
		 SMatrix<int> &matBBt);

void computeBBt(const SMatrix<unsigned char> &matB, 
		 SMatrix<int> &matBBt);