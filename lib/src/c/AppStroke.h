#if !defined(AFX_APPSTROKE_H__1536C282_C445_11D1_B2F6_006008332431__INCLUDED_)
#define AFX_APPSTROKE_H__1536C282_C445_11D1_B2F6_006008332431__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "Stroke.h"
#include "Basis.h"
 
class AppStroke : public Stroke
{
public:
	int        m_ReSam = 0;

	float    * m_pApp = 0;
	float    * m_pRsm = 0;
	float    * m_pBck = 0;

	float    * m_pCfs = 0;

	p_RSDATA   m_pAppRS = 0;

	float      m_Lam = 1.0f;
	float      m_Err = 0.0f;

	Basis      m_Basis;

public:
	AppStroke() : Stroke() {};
	AppStroke( AppStroke& S );
	virtual   ~AppStroke();

	void Clear();
	void Init(int Dim, int Sam, int Ord, int ReSam, float* pPnt);

	void	Decompose ();
	void	Restore   ();
	void	Approx    ();

	int	ParamApp  ( int nItr, float MaxErr = -1.0f);

	void	ResetParam();
	void	Repar     ();
	void	Tracing   ();
	void	AppErr    ();

	void    SetNaturalParam();

	void	SaveRsmState();
	void	LoadRsmState();

	float   GetErr() { return m_Err; }

	float   PolyError2( float * pDat = 0 );
	void    Rsm2App   ();
	void    App2Rsm   ();
	bool	GetRsmAt( int Idx, float *pX, float *pY, float *pZ = 0 );
	bool	GetAppAt( int Idx, float *pX, float *pY, float *pZ = 0 );
};

#endif // !defined(AFX_APPSTROKE_H__1536C282_C445_11D1_B2F6_006008332431__INCLUDED_)
