// FRect.cpp : implementation file
//

#include "stdafx.h"
#include "FRect.h"

CArchive& AFXAPI operator<<(CArchive& ar, const FRect& r)
{
	float  L = r.left  ; ar.Write( &L, sizeof(float) );
	float  T = r.top   ; ar.Write( &T, sizeof(float) );
	float  R = r.right ; ar.Write( &R, sizeof(float) );
	float  B = r.bottom; ar.Write( &B, sizeof(float) );

	return ar;
}

CArchive& AFXAPI operator>>(CArchive& ar, FRect& r)
{
	float  L; ar.Read( &L, sizeof(float) ); r.left   = L;
	float  T; ar.Read( &T, sizeof(float) ); r.top    = T;
	float  R; ar.Read( &R, sizeof(float) ); r.right  = R; 
	float  B; ar.Read( &B, sizeof(float) ); r.bottom = B;

	return ar;
}

void ZoomRect( CRect * pR, CPoint P, float Sc )
{
	pR->left   = (int)((pR->left   - P.x) * Sc) + P.x;
	pR->right  = (int)((pR->right  - P.x) * Sc) + P.x;
				 
	pR->top    = (int)((pR->top    - P.y) * Sc) + P.y;
	pR->bottom = (int)((pR->bottom - P.y) * Sc) + P.y;
}

/////////////////////////////////////////////////////////////////////////////
// FRect message handlers
