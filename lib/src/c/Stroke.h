#ifndef _STROKE_H_INCLUDED_
#define _STROKE_H_INCLUDED_


typedef struct {
	FLOAT    s;
	FLOAT    r;
} RSDATA;

typedef struct 
{
	INT        m_Dim;      // Number of components (dimensions)
	INT        m_Sam;      // actual number of points
	INT        m_MaxSam;   // capacity 
	FLOAT *    m_pOrg;     // points buffer

	RSDATA *   m_pOrgRS;   // curve length info
} STROKE;


ERR_CODE Stroke_ZeroInit(STROKE* pS);
ERR_CODE Stroke_Init    (STROKE* pS, INT Dim, INT Sam);
ERR_CODE Stroke_Clear   (STROKE* pS);

ERR_CODE Stroke_Resize   (STROKE* pS, INT MaxSam);
ERR_CODE Stroke_AddPoints(STROKE* pS, INT Dim, INT nPoints, FLOAT* pPoints);


/*
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
*/

#endif 
