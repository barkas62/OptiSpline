#pragma once

#ifdef OPTISPLINE_EXPORTS
	#define OPTISPLINE_API __declspec(dllexport)
#else
	#define OPTISPLINE_API __declspec(dllimport)
#endif


extern "C" OPTISPLINE_API
void* CreateOptiSpline(int nDim, int nSam, int nOrd, int nResam, float * pPnt);

extern "C" OPTISPLINE_API
void DestroyOptiSpline(void* pInstance);

extern "C" OPTISPLINE_API
void* CopyOptiSpline(void* pInstance);

extern "C" OPTISPLINE_API
void OptiSplineApprox(void* pInstance, int nStep, float MaxErr = -1.0f);

extern "C" OPTISPLINE_API
int GetOptiSplineApp(void* pInstance, float* pApp, int Dim, int ReSam);

extern "C" OPTISPLINE_API
int GetOptiSplineRsm(void* pInstance, float* pRsm, int Dim, int ReSam);

extern "C" OPTISPLINE_API
float GetOptiSplineErr(void* pInstance);

extern "C" OPTISPLINE_API
float GetOptiSplineLam(void* pInstance);