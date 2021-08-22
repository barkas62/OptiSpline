#ifndef _APPSTROKE_H_INCLUDED_
#define _APPSTROKE_H_INCLUDED_

#include "types.h"

typedef VOID* hAPPSTROKE;
 
//#ifdef __cplusplus
extern "C" {
//#endif 
hAPPSTROKE AppStroke_Create();
VOID       AppStroke_Delete(hAPPSTROKE hAS);

ERR_CODE   AppStroke_Init(hAPPSTROKE hAS, INT Dim, INT Ord, INT ReSam, INT Sam, FLOAT* pPnt);

ERR_CODE AppStroke_ResetParam(hAPPSTROKE hAS);
ERR_CODE AppStroke_ParamApp  (hAPPSTROKE hAS, INT nItr, FLOAT TargetErr, INT* pFinalItr);

// Getting Results
FLOAT AppStroke_GetRSMError(hAPPSTROKE hAS);
FLOAT AppStroke_GetMaxError(hAPPSTROKE hAS);
FLOAT AppStroke_GetLambda  (hAPPSTROKE pAS);

ERR_CODE AppStroke_GetApproximatedPoints(hAPPSTROKE pAS, INT Dim, INT ReSam, FLOAT* pAppPoints);
ERR_CODE AppStroke_GetResampledPoints   (hAPPSTROKE pAS, INT Dim, INT ReSam, FLOAT* pRsmPoints);

//#ifdef __cplusplus
}
//#endif

#endif 
