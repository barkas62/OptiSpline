#include <memory.h>

#include "OptiSpline.h"
#include "AppStroke.h"


void* CreateOptiSpline(int nDim, int nSam, int nOrd, int nResam, float* pPnt) {
	AppStroke* pAppStrk = new AppStroke();
	pAppStrk->Init(nDim, nSam, nOrd, nResam, pPnt);
	return (void*)pAppStrk;
}

void DestroyOptiSpline(void* pInstance ) {
	if( pInstance )
		delete (AppStroke*)pInstance;
}

void* CopyOptiSpline(void* pInstance) {
	return pInstance ?
		new AppStroke(*((AppStroke*)pInstance)) :
		nullptr;
}

void OptiSplineApprox(void* pInstance, int nStep, float MaxErr) {
	AppStroke* pAppStrk = (AppStroke*)pInstance;
	
	pAppStrk->ParamApp(nStep, MaxErr);
}

int GetOptiSplineApp(void* pInstance, float* pApp, int Dim, int ReSam) {
	AppStroke* pAppStrk = (AppStroke*)pInstance;

	if (Dim != pAppStrk->m_Dim || ReSam != pAppStrk->m_ReSam || pAppStrk->m_pApp == 0)
		return 0;

	memcpy(pApp, pAppStrk->m_pApp, pAppStrk->m_Dim * pAppStrk->m_ReSam * sizeof(float));

	return pAppStrk->m_ReSam;
}

int GetOptiSplineRsm(void* pInstance, float* pRsm, int Dim, int ReSam) {
	AppStroke* pAppStrk = (AppStroke*)pInstance;

	if (Dim != pAppStrk->m_Dim || ReSam != pAppStrk->m_ReSam || pAppStrk->m_pRsm == 0)
		return 0;

	memcpy(pRsm, pAppStrk->m_pRsm, pAppStrk->m_Dim * pAppStrk->m_ReSam * sizeof(float));

	return pAppStrk->m_ReSam;
}

float GetOptiSplineErr(void* pInstance) {
	AppStroke* pAppStrk = (AppStroke*)pInstance;

	return pAppStrk->m_Err;
}

float GetOptiSplineLam(void* pInstance) {
	AppStroke* pAppStrk = (AppStroke*)pInstance;

	return pAppStrk->m_Lam;
}