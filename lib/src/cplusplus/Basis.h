// Basis.h: interface for the Basis class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BASIS_H__1536C284_C445_11D1_B2F6_006008332431__INCLUDED_)
#define AFX_BASIS_H__1536C284_C445_11D1_B2F6_006008332431__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class  AppStroke;

class Basis  
{
public:
	int		m_Ord; 
	int		m_ReSam; 
	float * m_pBasis;

	float * m_pNorms;

public:
	Basis();
	Basis( const Basis& Bas );
	virtual ~Basis();

	void    Init ( int Ord, int ReSam );
	void    ClearBasis();
	void	CosBasis  ();
	void    CosBasis( int ReSam0 );
	void    Norm      ();
};

#endif // !defined(AFX_BASIS_H__1536C284_C445_11D1_B2F6_006008332431__INCLUDED_)
