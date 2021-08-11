// Stroke.cpp: implementation of the Stroke class.

#include <memory.h>
#include <math.h>
#include <malloc.h>

#include "Stroke.h"

ERR_CODE Stroke_AddPoint(STROKE* pS, INT Dim, FLOAT* pPnt);

ERR_CODE Stroke_Init(STROKE* pS, int Dim, int Sam)
{
	FLOAT  * pOrg   = NULL;
	RSDATA * pOrgRS = NULL;

	if (pS == NULL)
		return err_code_ZERO_POINTER_PASSED;

	if (Dim <= 1 || Sam <= 0)
		return err_code_WRONG_PARAMETER;

	Stroke_Clear(pS);

	pOrg   = (FLOAT *)malloc( Dim*Sam*sizeof(FLOAT) );
	pOrgRS = (RSDATA*)malloc( Sam*sizeof(RSDATA) );

	if (pOrg == NULL || pOrgRS == NULL)
	{
		if (pOrg != NULL)
			free(pOrg);
		if (pOrgRS != NULL)
			free(pOrgRS);
		return err_code_OUT_OF_MEMORY;
	}

	memset(pOrg,   0, Dim* Sam*sizeof(float) );
	memset(pOrgRS, 0,      Sam*sizeof(RSDATA));

	pS->m_pOrg   = pOrg;
	pS->m_pOrgRS = pOrgRS;

	pS->m_Dim    = Dim;
	pS->m_MaxSam = Sam;
	pS->m_Sam    = 0;

	return err_code_OK;
}

ERR_CODE Stroke_ZeroInit(STROKE* pS)
{
	if (pS == NULL)
		return err_code_ZERO_POINTER_PASSED;

	pS->m_Dim = 0;
	pS->m_Sam = 0;
	pS->m_MaxSam = 0;


	pS->m_pOrg   = NULL;
	pS->m_pOrgRS = NULL;

	return err_code_OK;
}


ERR_CODE Stroke_Clear(STROKE* pS)
{
	if (pS == NULL)
		return err_code_ZERO_POINTER_PASSED;

	if( pS->m_pOrg != NULL )
		free( pS->m_pOrg );

	if( pS->m_pOrgRS != 0 )
		free( pS->m_pOrgRS );

	return Stroke_ZeroInit(pS);
}

ERR_CODE Stroke_Resize(STROKE* pS, INT MaxSam)
{
	FLOAT  * pOrg   = NULL;
	RSDATA * pOrgRS = NULL;

	if (pS == NULL)
		return err_code_ZERO_POINTER_PASSED;

	if (MaxSam <= 0)
		return err_code_WRONG_PARAMETER;

	if (MaxSam <= pS->m_MaxSam)
		return err_code_OK;

	if (pS->m_Dim <= 1)
		return err_code_BAD_STRUCTURE_CONTENT;

	pOrg   = (FLOAT *)malloc(pS->m_Dim * MaxSam * sizeof(FLOAT));
	pOrgRS = (RSDATA*)malloc(MaxSam * sizeof(RSDATA));

	if (pOrg == NULL || pOrgRS == NULL)
	{
		if (pOrg != NULL)
			free(pOrg);
		if (pOrgRS != NULL)
			free(pOrgRS);

		return err_code_OUT_OF_MEMORY;
	}

	memset( pOrg, 0, pS->m_Dim*MaxSam*sizeof(float) );

	if (pS->m_pOrg != 0  && pS->m_Sam >= 1)
	{
		memcpy( pOrg, pS->m_pOrg, pS->m_Dim* pS->m_Sam*sizeof(float) );
		free(pS->m_pOrg);
	}
	pS->m_pOrg = pOrg;

	memset( pOrgRS,  0, MaxSam*sizeof(RSDATA) );

	if (pS->m_pOrgRS != 0 && pS->m_Sam >= 1)
	{
		memcpy( pOrgRS, pS->m_pOrgRS, pS->m_Sam*sizeof(RSDATA) );
		free(pS->m_pOrgRS);
	}
	pS->m_pOrgRS = pOrgRS;

	pS->m_MaxSam = MaxSam;

	return err_code_OK;
}

ERR_CODE Stroke_AddPoints(STROKE *pS, INT Dim, INT nPoints, FLOAT* pPoints)
{
	INT i;
	ERR_CODE err_code = err_code_OK;
	FLOAT* pPnt = NULL;

	if (pS == NULL || pPoints == NULL)
		return err_code_ZERO_POINTER_PASSED;
		
	if (pS->m_Dim != Dim || Dim == 0 || nPoints <= 0)
		return err_code_WRONG_PARAMETER;

	if (pS->m_Sam + nPoints > pS->m_MaxSam)
	{
		err_code = Stroke_Resize(pS, pS->m_Sam + nPoints);
		if (err_code != err_code_OK) 
		{
			Stroke_Clear(pS);
			return err_code;
		}

	}

	pPnt = pPoints;
	for (i = 0; i < nPoints; i++, pPnt += Dim)
	{
		err_code = Stroke_AddPoint(pS, Dim, pPnt);
		if (err_code != err_code_OK)
			break;
	}

	return  err_code;
}

ERR_CODE Stroke_AddPoint(STROKE* pS, INT Dim, FLOAT* pPnt)
{
	ERR_CODE err_code = err_code_OK;

	INT k = 0;
	FLOAT* pLastPnt = NULL;
	FLOAT ds = (FLOAT)0.0;
	FLOAT d  = (FLOAT)0.0;
	RSDATA* pRS = NULL;

	if (pS == NULL || pPnt == NULL)
		return err_code_ZERO_POINTER_PASSED;
	if (Dim == 0 || Dim != pS->m_Dim)
		return err_code_WRONG_PARAMETER;

	if (pS->m_Sam >= pS->m_MaxSam)
	{
		err_code = Stroke_Resize(pS, pS->m_Sam + 128);
		if (err_code != err_code_OK)
			return err_code;
	}

	if (pS->m_Sam == 0)
	{
		memcpy(pS->m_pOrg, pPnt, Dim * sizeof(FLOAT));
		pS->m_pOrgRS->r = pS->m_pOrgRS->s = (FLOAT)0.0;
		pS->m_Sam = 1;
	}
	else
	{
		pLastPnt = pS->m_pOrg + pS->m_Dim*(pS->m_Sam-1);
		ds = (FLOAT)0.0;
		for (k = 0; k < Dim; k++)
		{
			d = pPnt[k] - pLastPnt[k];
			ds += d * d;
		}
		if (ds > ZERO_PRECISION)
		{ // New point differs from the last one
			ds = (FLOAT)sqrt(ds);
			memcpy(pLastPnt + Dim, pPnt, Dim*sizeof(FLOAT));
			
			pRS = pS->m_pOrgRS + (pS->m_Sam-1);
			pRS[1].s = ds;
			pRS[1].r = pRS[0].r + ds;
			pS->m_Sam += 1;
		}
	}

	return err_code;
}



