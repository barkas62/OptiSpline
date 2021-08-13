#pragma once

#ifdef OPTISPLINE_EXPORTS
	#define OPTISPLINE_API __declspec(dllexport)
#else
	#define OPTISPLINE_API __declspec(dllimport)
#endif


#ifdef __cplusplus
extern "C" {
#endif
	OPTISPLINE_API
		void* CreateOptiSpline(int nDim, int nSam, int nOrd, int nResam, double* pPnt);

	OPTISPLINE_API
		void DestroyOptiSpline(void* pInstance);

	OPTISPLINE_API
		void OptiSplineApprox(void* pInstance, int nStep, double MaxErr = -1.0f);

	OPTISPLINE_API
		int GetOptiSplineApp(void* pInstance, double* pApp, int Dim, int ReSam);

	OPTISPLINE_API
		int GetOptiSplineRsm(void* pInstance, double* pRsm, int Dim, int ReSam);

	OPTISPLINE_API
		double GetOptiSplineErr(void* pInstance);

	OPTISPLINE_API
		double GetOptiSplineLam(void* pInstance);

#ifdef __cplusplus
}
#endif