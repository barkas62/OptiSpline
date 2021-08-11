#ifndef _BASIS_H_INCLUDED_
#define _BASIS_H_INCLUDED_

#include "types.h"

typedef struct{
	INT		m_Ord; 
	INT		m_ReSam; 

	FLOAT * m_pBasis;
	FLOAT * m_pNorms;
} BASIS;


ERR_CODE Basis_ZeroInit(BASIS* pB);
ERR_CODE Basis_Init    (BASIS* pB, INT Ord, INT ReSam);
ERR_CODE Basis_Clear   (BASIS* pB);

#endif
