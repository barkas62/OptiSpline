#if !defined(AFX_STROKE_H__16A00085_C41B_11D1_B2F6_006008332431__INCLUDED_)
#define AFX_STROKE_H__16A00085_C41B_11D1_B2F6_006008332431__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

//#include "rsdata.h"

typedef struct {
	float    s;
	float    r;
	float    alf;
	int      idx;
} _RSDATA, * p_RSDATA;

class Stroke  
{
public:
	int        m_Dim    = 0;     // Number of components (dimensions)
	int        m_Sam    = 0;     // actual number of points
	int        m_MaxSam = -1;	 // capacity 
	float    * m_pOrg   = 0;     // points buffer

	p_RSDATA   m_pOrgRS = 0;     // curve length info

public:
	Stroke() {}
	Stroke( Stroke& S );
	//Stroke(int Dim, int Sam, float * pPoints);
	virtual ~Stroke();

	void Init(int Dim, int Sam);

	int		GetDim (){ return m_Dim; }
	int		GetSize(){ return m_Sam; }

	void	Clear();
	void	Resize( int MaxSam, bool bCopy = true );

	bool	GetAt( int Idx,   int *pX,   int *pY, int   *pZ = 0 );
	bool	GetAt( int Idx, float *pX, float *pY, float *pZ = 0 );
	bool	GetAt( int Idx,   int *pX ); // pX must be of m_Dim length
	bool	GetAt( int Idx, float *pX ); // pX must be of m_Dim length

	void    Add  ( float  x, float y );
	void    Add  ( int    x,   int y );

	void    Add  ( float  x, float y, float z );
	void    Add  ( int    x,   int y,   int z );
	bool    Add  ( int  Dim, float * pX );

	void	Tracing();
};

#endif // !defined(AFX_STROKE_H__16A00085_C41B_11D1_B2F6_006008332431__INCLUDED_)
