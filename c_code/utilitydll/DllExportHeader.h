#pragma once

#ifdef _WINDOWS
#define UTILITY_DLL __declspec(dllexport)
#else
#define UTILITY_DLL
#endif
