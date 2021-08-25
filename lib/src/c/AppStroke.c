#include <math.h>
#include <memory.h>
#include <stdlib.h>

#include "types.h"
#include "AppStroke.h"
#include "Stroke.h"
#include "Basis.h"

#define _ABS(x) ((x) > 0 ? (x) :(-(x)))

typedef struct
{
	INT     m_ReSam;

	FLOAT* m_pApp;
	FLOAT* m_pRsm;

	FLOAT* m_pCfs;

	RSDATA* m_pAppRS;

	FLOAT   m_Lam;
	FLOAT   m_RMSErr;
	FLOAT   m_MaxErr;

	BASIS   m_Basis;
	STROKE  m_Stroke;
} APPSTROKE;

VOID AppStroke_ZeroInit(APPSTROKE* pAS);
VOID AppStroke_Clear   (APPSTROKE* pAS);

FLOAT  AppStroke_Repar(INT Dim, INT Sam, FLOAT* pOrg, RSDATA* pOrgRS,
	INT ReSam, FLOAT* pRsm, FLOAT* pApp, RSDATA* pAppRS);

VOID AppStroke_Approx(INT Dim, INT ReSam, FLOAT* pRsm, FLOAT* pApp,
	INT Ord, FLOAT* pBasis, FLOAT* pCfs);

FLOAT AppStroke_AppErr(INT Dim, INT ReSam, FLOAT* pRsm, FLOAT* pApp, FLOAT* pMaxErr);

VOID AppStroke_Tracing(INT Dim, INT ReSam, FLOAT* pApp, RSDATA* pAppRS);

hAPPSTROKE AppStroke_Create()
{
	APPSTROKE* pAS = malloc(sizeof(APPSTROKE));
	if (pAS != NULL);
		AppStroke_ZeroInit(pAS);

	return (hAPPSTROKE)pAS;
}

VOID AppStroke_Delete(hAPPSTROKE hAS)
{
	if (hAS == NULL)
		return;

	APPSTROKE* pAS = (APPSTROKE*)hAS;
	AppStroke_Clear(pAS);
	free(pAS);
}

VOID AppStroke_ZeroInit(APPSTROKE * pAS)
{
	if (pAS == NULL)
		return;

	pAS->m_ReSam = 0;

	pAS->m_pApp = NULL;
	pAS->m_pRsm = NULL;

	pAS->m_pCfs = NULL;

	pAS->m_pAppRS = NULL;

	pAS->m_Lam    = (FLOAT)0.0;
	pAS->m_RMSErr = (FLOAT)0.0;
	pAS->m_MaxErr = (FLOAT)0.0;

	Basis_ZeroInit ( &(pAS->m_Basis ) );
	Stroke_ZeroInit( &(pAS->m_Stroke) );

	return;
}

VOID AppStroke_Clear(APPSTROKE* pAS)
{
	if (pAS == NULL)
		return;

	if (pAS->m_pApp != 0)
		free(pAS->m_pApp);
	if (pAS->m_pRsm != 0)
		free(pAS->m_pRsm);

	if (pAS->m_pCfs != 0)
		free(pAS->m_pCfs);

	if (pAS->m_pAppRS != 0)
		free(pAS->m_pAppRS);


	Basis_Clear ( &(pAS->m_Basis)  );
	Stroke_Clear( &(pAS->m_Stroke) );

	AppStroke_ZeroInit(pAS);
}

ERR_CODE AppStroke_Init(hAPPSTROKE hAS, INT Dim, INT Ord, INT ReSam, INT Sam, FLOAT * pPnt)
{
	ERR_CODE err_code = err_code_OK;

	FLOAT* pApp = NULL;
	FLOAT* pRsm = NULL;
	FLOAT* pCfs = NULL;

	RSDATA* pAppRS = NULL;

	if (hAS == NULL)
		return err_code_ZERO_POINTER_PASSED;

	if (Dim <= 0 || Sam <= 1 || Ord <= 0 || ReSam <= 1)
		return err_code_WRONG_PARAMETER;;

	APPSTROKE* pAS = (APPSTROKE*)hAS;

	AppStroke_Clear(pAS);
	
	pAS->m_pApp = (FLOAT*)malloc(Dim*ReSam*sizeof(FLOAT));
	pAS->m_pRsm = (FLOAT*)malloc(Dim*ReSam*sizeof(FLOAT));
	pAS->m_pCfs = (FLOAT*)malloc(Dim*ReSam*sizeof(FLOAT));

	pAS->m_pAppRS = (RSDATA*)malloc(ReSam*sizeof(RSDATA));

	if (pAS->m_pApp == NULL || pAS->m_pRsm == NULL || pAS->m_pCfs == NULL || pAS->m_pAppRS == NULL)
	{
		AppStroke_Clear(pAS);
		return err_code_OUT_OF_MEMORY;
	}

	memset(pAS->m_pApp,   0, Dim*ReSam*sizeof(float) );
	memset(pAS->m_pRsm,   0, Dim*ReSam*sizeof(float) );
	memset(pAS->m_pCfs,   0, Dim*ReSam*sizeof(float) );
	memset(pAS->m_pAppRS, 0, ReSam*sizeof(RSDATA) );
		
	pAS->m_ReSam = ReSam;

	err_code = Basis_Init(&(pAS->m_Basis), Ord, ReSam);
	if (err_code != err_code_OK)
	{
		AppStroke_Clear(pAS);
		return err_code;
	}

	err_code = Stroke_Init(&(pAS->m_Stroke), Dim, Sam);
	if (err_code != err_code_OK)
	{
		AppStroke_Clear(pAS);
		return err_code;
	}

	err_code = Stroke_AddPoints(&(pAS->m_Stroke), Dim, Sam, pPnt);
	if (err_code != err_code_OK)
	{
		AppStroke_Clear(pAS);
		return err_code;
	}

	err_code = AppStroke_ResetParam(pAS);
	if (err_code != err_code_OK)
	{
		AppStroke_Clear(pAS);
		return err_code;
	}

	err_code = AppStroke_ParamApp(pAS, (INT)1, (FLOAT)(-1.0), NULL);
	if (err_code != err_code_OK)
	{
		AppStroke_Clear(pAS);
		return err_code;
	}

	return err_code_OK;
}

ERR_CODE AppStroke_ParamApp(hAPPSTROKE hAS, INT nItr, FLOAT TargetErr, INT * pFinalItr)
{
	ERR_CODE err_code = err_code_OK;

	INT it;
	INT Dim, Sam, ReSam, Ord;
	FLOAT* pOrg, * pApp, * pRsm, * pCfs, *pBasis;
	RSDATA* pOrgRS, * pAppRS;

	if (hAS == NULL)
		return err_code_ZERO_POINTER_PASSED;

	APPSTROKE* pAS = (APPSTROKE*)hAS;

	Dim    = pAS->m_Stroke.m_Dim;
	Sam    = pAS->m_Stroke.m_Sam;
	pOrg   = pAS->m_Stroke.m_pOrg;
	pOrgRS = pAS->m_Stroke.m_pOrgRS;

	if (Dim <= 0 || Sam <= 1 || pOrg == NULL || pOrgRS == NULL)
		return err_code_BAD_STRUCTURE_CONTENT;

	ReSam  = pAS->m_ReSam;
	pRsm   = pAS->m_pRsm;
	pApp   = pAS->m_pApp;
	pAppRS = pAS->m_pAppRS;

	if (ReSam <= 1 || pRsm == NULL || pApp == NULL || pOrgRS == NULL)
		return err_code_BAD_STRUCTURE_CONTENT;

	Ord    = pAS->m_Basis.m_Ord;
	pCfs   = pAS->m_pCfs;
	pBasis = pAS->m_Basis.m_pBasis;

	if (Ord < 1 || pCfs == NULL || pBasis == NULL)
		return err_code_BAD_STRUCTURE_CONTENT;
	
	if(_ABS(pOrgRS[Sam - 1].r) < ZERO_PRECISION)
		return err_code_BAD_STRUCTURE_CONTENT;

	it = 0;
	for( ; it < nItr; it++ )
	{
		AppStroke_Repar  (Dim, Sam, pOrg, pOrgRS, ReSam, pRsm, pApp, pAppRS);
		AppStroke_Approx (Dim, ReSam, pRsm, pApp, Ord, pBasis, pCfs);
		AppStroke_Tracing(Dim, ReSam, pApp, pAppRS);
		if (_ABS(pAppRS[Sam - 1].r) < ZERO_PRECISION)
		{
			err_code = err_code_ALGORITHM_FAILURE;
			break;
		}

		pAS->m_RMSErr = AppStroke_AppErr(Dim, ReSam, pRsm, pApp, &(pAS->m_MaxErr));
		pAS->m_Lam    = (_ABS(pAppRS[Sam - 1].r) > ZERO_PRECISION) ? pOrgRS[Sam-1].r / pAppRS[ReSam-1].r : (FLOAT)0.0;

		if (pAS->m_RMSErr < TargetErr)
			break;
	} 
	
	if (pFinalItr != NULL)
		*pFinalItr = it;

	return err_code;
}

ERR_CODE AppStroke_ResetParam(hAPPSTROKE hAS)
{
	INT i;
	FLOAT ds;

	if (hAS == NULL)
		return err_code_ZERO_POINTER_PASSED;

	APPSTROKE* pAS = (APPSTROKE*)hAS;

	INT Sam = pAS->m_Stroke.m_Sam;
	INT ReSam = pAS->m_ReSam;
	RSDATA* pOrgRS = pAS->m_Stroke.m_pOrgRS;
	RSDATA* pAppRS = pAS->m_pAppRS;

	if (Sam <= 1 || ReSam <= 1 || pOrgRS == NULL || pAppRS == NULL)
		return err_code_BAD_STRUCTURE_CONTENT;

	ds = pOrgRS[Sam - 1].r / (FLOAT)(ReSam - 1);

	pAppRS[0].s = (FLOAT)0.0;
	pAppRS[0].r = (FLOAT)0.0;
	for (i = 1; i < ReSam; i++)
	{
		pAppRS[i].s = ds;
		pAppRS[i].r = pAppRS[i - 1].r + ds;
	}

	return err_code_OK;
}

FLOAT  AppStroke_Repar(INT Dim, INT Sam, FLOAT* pOrg, RSDATA* pOrgRS,
	                   INT ReSam, FLOAT* pRsm, FLOAT* pApp, RSDATA* pAppRS)
{
	INT       i, j, k;
	FLOAT     x1, x2, dx;
	FLOAT     Alfa, Curr;
	FLOAT     Lam = (FLOAT)1.0;

	for (k = 0; k < Dim; k++)
	{
		pRsm[k] = pOrg[k];
		pRsm[Dim*(ReSam - 1) + k] = pOrg[Dim * (Sam - 1) + k];
	}

	Lam = pOrgRS[Sam - 1].r / pAppRS[ReSam - 1].r;

	for (i = j = 1; i < (ReSam - 1); i++)
	{
		Curr = Lam * pAppRS[i].r;

		while (Curr > pOrgRS[j].r + (FLOAT)0.001)
		{
			j++;

			if (j == Sam)
			{
				for (; i < ReSam; i++)
				{
					for (k = 0; k < Dim; k++)
						pRsm[Dim*i + k] = pOrg[Dim*(Sam - 1) + k] / Lam;
				}
				goto END;
			}	 // if( j==Sam )
		}	// while

		if (pOrgRS[j].s == (FLOAT)0.0)
			Alfa = (FLOAT)1.0;
		else
			Alfa = 1 - (pOrgRS[j].r - Curr) / pOrgRS[j].s;

		for (k = 0; k < Dim; k++)
		{
			x2 = pOrg[Dim*j+k];
			x1 = pOrg[Dim*(j - 1) + k];
			dx = x2 - x1;

			pRsm[Dim*i + k] = x1 + dx*Alfa;
		}
	}
END:
	return Lam;
}

VOID AppStroke_Approx(INT Dim, INT ReSam, FLOAT* pRsm, FLOAT* pApp, INT Ord, FLOAT* pBasis, FLOAT* pCfs)
{
	INT    i, k;
	FLOAT  C;
	FLOAT* pBas;
	FLOAT* pR, *pA;

	memset(pApp, 0, Dim*ReSam*sizeof(FLOAT));

	for(k = 0; k < Dim; k++) 
	{
		pBas = pBasis;

		for(i = 0; i < Ord; i++)
		{
			C  = (FLOAT)0.0;
			pR = pRsm + k;
			for( int j = 0;  j < ReSam; j++, pR += Dim, pBas++ )
				C += (*pR) * (*pBas);

			pCfs[Ord*k+i] = C;
		}

		pBas   = pBasis;
		for( int i = 0; i < Ord; i++ )
		{
			C  = pCfs[Ord*k+i];
			pA = pApp + k;
			for( int j  = 0; j < ReSam; j++, pA += Dim, pBas++ )
				*pA += C * (*pBas);
		}
	}
}

FLOAT AppStroke_AppErr(INT Dim, INT ReSam, FLOAT* pRsm, FLOAT* pApp, FLOAT* pMaxErr)
{
	FLOAT Err    = (FLOAT)0.0;
	FLOAT MaxErr = (FLOAT)0.0;

	FLOAT E, T;

	FLOAT* pA = pApp;
	FLOAT* pR = pRsm;

	INT i, k;

	for (i = 0; i < ReSam; i++)
	{
		E = (FLOAT)0.0;
		for (k = 0; k < Dim; k++, pA++, pR++)
		{
			T = *pA - *pR;
			E += T * T;
		}

		Err += E;
		if (E > MaxErr)
			E = MaxErr;
	}

	Err = (FLOAT)sqrt(Err / ReSam);

	if (pMaxErr != NULL)
		*pMaxErr = MaxErr;

	return Err;
}

VOID AppStroke_Tracing(INT Dim, INT ReSam, FLOAT* pApp, RSDATA* pAppRS)
{
	INT i, k;
	FLOAT  ds, dx;
	FLOAT  dl = (FLOAT)0.0;

	FLOAT* pA = pApp;
	RSDATA* pRS = pAppRS + 1;
	
	pAppRS[0].s = pAppRS[0].r = (FLOAT)0.0;

	for (i = 1; i < ReSam; i++, pRS++)
	{
		ds = 0.0;
		for (k = 0; k < Dim; k++, pA++)
		{
			dx = pA[Dim] - pA[0];
			ds += dx * dx;
		}
		ds     = (FLOAT)sqrt( ds );
		pRS->s = ds;
		pRS->r = (pRS-1)->r + ds;
    }

	return;
}

FLOAT AppStroke_GetRSMError(hAPPSTROKE hAS)
{
	return hAS != NULL ? ((APPSTROKE*)hAS)->m_RMSErr : (FLOAT)(-1.0);
}

FLOAT AppStroke_GetMaxError(hAPPSTROKE hAS)
{
	return hAS != NULL ? ((APPSTROKE*)hAS)->m_MaxErr : (FLOAT)(-1.0);
}

FLOAT AppStroke_GetLambda(hAPPSTROKE hAS)
{
	return hAS != NULL ? ((APPSTROKE*)hAS)->m_Lam : (FLOAT)(-1.0);
}

ERR_CODE AppStroke_GetApproximatedPoints(hAPPSTROKE hAS, INT Dim, INT ReSam, FLOAT* pAppPoints)
{
	if (hAS == NULL || pAppPoints == NULL)
		return err_code_ZERO_POINTER_PASSED;

	APPSTROKE* pAS = (APPSTROKE*)hAS;

	if (pAS->m_Stroke.m_Dim <= 1 || pAS->m_ReSam <= 0)
		return err_code_BAD_STRUCTURE_CONTENT;

	if (Dim != pAS->m_Stroke.m_Dim || ReSam != pAS->m_ReSam)
		return err_code_WRONG_PARAMETER;

	memcpy(pAppPoints, pAS->m_pApp, Dim*ReSam*sizeof(FLOAT));

	return err_code_OK;
}

ERR_CODE AppStroke_GetResampledPoints(hAPPSTROKE hAS, INT Dim, INT ReSam, FLOAT* pRsmPoints)
{
	if (hAS == NULL || pRsmPoints == NULL)
		return err_code_ZERO_POINTER_PASSED;

	APPSTROKE* pAS = (APPSTROKE*)hAS;

	if (pAS->m_Stroke.m_Dim <= 1 || pAS->m_ReSam <= 0)
		return err_code_BAD_STRUCTURE_CONTENT;

	if (Dim != pAS->m_Stroke.m_Dim || ReSam != pAS->m_ReSam)
		return err_code_WRONG_PARAMETER;

	memcpy(pRsmPoints, pAS->m_pRsm, Dim * ReSam * sizeof(FLOAT));

	return err_code_OK;
}



