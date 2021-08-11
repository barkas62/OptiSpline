#include <math.h>
#include <memory.h>
#include <malloc.h>
#include "Basis.h"

BOOL Basis_Validate(BASIS* pB);

ERR_CODE Basis_Zero(BASIS* pB);
ERR_CODE Basis_SetCosBasis(BASIS* pB);
ERR_CODE Basis_Normalize(BASIS* pB);

ERR_CODE Basis_ZeroInit(BASIS * pB)
{
	if (pB == NULL)
		return err_code_ZERO_POINTER_PASSED;
	
	pB->m_Ord    = 0; 
	pB->m_ReSam  = 0;

	pB->m_pBasis = NULL;
	pB->m_pNorms = NULL;

	return err_code_OK;
}


ERR_CODE Basis_Init( BASIS* pB, INT Ord, INT ReSam )
{
	FLOAT* pBasis = NULL;
	FLOAT* pNorms = NULL;

	if (pB == NULL)
		return err_code_ZERO_POINTER_PASSED;

	if (Ord <= 0 || ReSam <= 0 || Ord > ReSam)
		return err_code_WRONG_PARAMETER;

	Basis_Clear(pB);
		
	pBasis = (FLOAT *)malloc(Ord*ReSam*sizeof(FLOAT));
	pNorms = (FLOAT *)malloc(Ord*sizeof(FLOAT));

	if (pBasis == NULL || pNorms == NULL)
	{
		if (pBasis != NULL)
			free(pBasis);
		if (pNorms != NULL)
			free(pNorms);
		return err_code_OUT_OF_MEMORY;
	}

	pB->m_Ord = Ord;
	pB->m_ReSam = ReSam;

	pB->m_pBasis = pBasis;
	pB->m_pNorms = pNorms;

	return Basis_SetCosBasis(pB);
}


ERR_CODE Basis_Clear(BASIS* pB)
{
	if (pB == NULL)
		return err_code_ZERO_POINTER_PASSED;

	if( pB->m_pBasis != NULL)
		free(pB->m_pBasis);

	if(pB->m_pNorms != NULL)
		free(pB->m_pNorms);

	return Basis_ZeroInit(pB);
}

BOOL Basis_Validate(BASIS* pB)
{
	if (pB == NULL)
		return FALSE;

	if (pB->m_Ord <= 0 || pB->m_ReSam <= 1 || pB->m_Ord > pB->m_ReSam ||
		pB->m_pBasis == NULL || pB->m_pNorms == NULL )
		return FALSE;

	return TRUE;
}

ERR_CODE Basis_SetCosBasis(BASIS* pB)
{
	INT i, j;

	if (pB == NULL)
		return err_code_ZERO_POINTER_PASSED;

	if (!Basis_Validate(pB))
		return err_code_BAD_STRUCTURE_CONTENT;

	FLOAT  dt   = M_PI / pB->m_ReSam;

	FLOAT* pBas = pB->m_pBasis;
	for( j = 0; j < pB->m_ReSam; j++, pBas++ )
	    *pBas = (FLOAT)1.0f;

	for(i = 1; i < pB->m_Ord; i++)
	{
		FLOAT t = dt / (FLOAT)2.0;
		for(j = 0; j < pB->m_ReSam; j++, t += dt, pBas++ )
			*pBas = (float)cos(i * t);
	} 

	return Basis_Normalize(pB);
 }

ERR_CODE Basis_Normalize( BASIS* pB)
{
	INT     i, j;
	FLOAT * pBas;

	if (pB == NULL)
		return err_code_ZERO_POINTER_PASSED;

	if (!Basis_Validate(pB))
		return err_code_BAD_STRUCTURE_CONTENT;

	for( i = 0; i < pB->m_Ord; i++ )
	{
		FLOAT  N = (FLOAT)0.0;

		pBas = pB->m_pBasis + i*pB->m_ReSam;
		for( j = 0; j < pB->m_ReSam; j++, pBas++ )
			N += (*pBas) * (*pBas);

		N = (FLOAT)sqrt( N );
		pB->m_pNorms[i] = N;

		pBas = pB->m_pBasis + i* pB->m_ReSam;
		for( j = 0; j < pB->m_ReSam; j++, pBas++ )
			*pBas /= N;
    } // for(i)

	return err_code_OK;
 }



