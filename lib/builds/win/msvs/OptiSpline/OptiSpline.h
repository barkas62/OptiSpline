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
		void* CreateOptiSpline(int nDim, int nSam, int nOrd, int nResam, float* pPnt);

	OPTISPLINE_API
		void DestroyOptiSpline(void* pInstance);

	OPTISPLINE_API
		void* CopyOptiSpline(void* pInstance);

	OPTISPLINE_API
		void OptiSplineApprox(void* pInstance, int nStep, float MaxErr = -1.0f);

	OPTISPLINE_API
		int GetOptiSplineApp(void* pInstance, float* pApp, int Dim, int ReSam);

	OPTISPLINE_API
		int GetOptiSplineRsm(void* pInstance, float* pRsm, int Dim, int ReSam);

	OPTISPLINE_API
		float GetOptiSplineErr(void* pInstance);

	OPTISPLINE_API
		float GetOptiSplineLam(void* pInstance);

#ifdef __cplusplus
}
#endif