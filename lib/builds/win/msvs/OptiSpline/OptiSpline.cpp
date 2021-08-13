#include <memory.h>

#include "OptiSpline.h"
#include "AppStroke.h"


void* CreateOptiSpline(int nDim, int nSam, int nOrd, int nResam, double* pPnt) {
	hAPPSTROKE hAS = AppStroke_Create();
	AppStroke_Init(hAS, 2, nOrd, nResam, nSam, pPnt);
	return (void*)hAS;
}

void DestroyOptiSpline(void* pInstance ) {
	if( pInstance )
		AppStroke_Delete( (hAPPSTROKE)pInstance );
}

void OptiSplineApprox(void* pInstance, int nStep, double MaxErr)
{
	AppStroke_ParamApp((hAPPSTROKE)pInstance, nStep, MaxErr, NULL);
}

int GetOptiSplineApp(void* pInstance, double* pApp, int Dim, int ReSam) {
	hAPPSTROKE hAS  = (hAPPSTROKE)pInstance;

	AppStroke_GetApproximatedPoints(hAS, Dim, ReSam, pApp);

	return ReSam;
}

int GetOptiSplineRsm(void* pInstance, double* pRsm, int Dim, int ReSam) {
	hAPPSTROKE hAS = (hAPPSTROKE)pInstance;

	AppStroke_GetResampledPoints(hAS, Dim, ReSam, pRsm);

	return ReSam;
}

double GetOptiSplineErr(void* pInstance) {
	hAPPSTROKE hAS = (hAPPSTROKE)pInstance;

	return AppStroke_GetRSMError(hAS);
}

double GetOptiSplineLam(void* pInstance) {
	hAPPSTROKE hAS = (hAPPSTROKE)pInstance;

	return AppStroke_GetLambda(hAS);
}