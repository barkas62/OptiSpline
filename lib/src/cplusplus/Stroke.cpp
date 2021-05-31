// Stroke.cpp: implementation of the Stroke class.

#include <memory.h>
#include <math.h>

#include "Stroke.h"

void Stroke::Init( int Dim, int Sam )
{
	Clear();

	m_pOrg   =  new float  [Dim * Sam];
	m_pOrgRS =  new _RSDATA[      Sam];

	if( m_pOrg  != 0  && m_pOrgRS  != 0 )
	{
		m_Dim = Dim;

		m_MaxSam = Sam;
		memset( m_pOrg  , 0, Dim*m_MaxSam*sizeof(float)   );
		memset( m_pOrgRS, 0,     m_MaxSam*sizeof(_RSDATA) );
	}
}


Stroke::Stroke( Stroke& S )
{
	m_Dim =  S.m_Dim;
	if( m_Dim == 0 )
		return;

	if (S.m_MaxSam > 0)
	{
		m_pOrg   = new float[m_Dim * S.m_MaxSam];
		m_pOrgRS = new _RSDATA[S.m_MaxSam];
		m_MaxSam = S.m_MaxSam;
		memset(m_pOrg, 0, m_Dim * m_MaxSam * sizeof(float));
		memset(m_pOrgRS, 0, m_MaxSam * sizeof(_RSDATA));
	}


	if (S.m_Sam > 0)
	{
		m_Sam = S.m_Sam;
		memcpy(m_pOrg,   S.m_pOrg,   m_Dim * m_Sam * sizeof(float));
		memcpy(m_pOrgRS, S.m_pOrgRS, m_Sam * sizeof(_RSDATA));
	}

}

Stroke::~Stroke()
{
	if( m_pOrg != 0 )
		delete m_pOrg;

	if( m_pOrgRS != 0 )
		delete m_pOrgRS;
}

void Stroke::Resize( int MaxSam, bool bCopy )
{
	if( MaxSam <= m_MaxSam )
		return;

	float  * pOrg   = new float  [m_Dim * MaxSam];
	memset( pOrg, 0, m_Dim*MaxSam*sizeof(float) );
	if( m_pOrg != 0 )
	{
		if( bCopy )
			memcpy( pOrg, m_pOrg, m_Dim* m_Sam*sizeof(float) );

		delete  m_pOrg;
	}
	m_pOrg = pOrg;

	p_RSDATA pOrgRS = new _RSDATA[        MaxSam];
	memset( pOrgRS,        0, MaxSam*sizeof(_RSDATA) );

	if( m_pOrgRS != 0 )
	{
		if( bCopy )
			memcpy( pOrgRS, m_pOrgRS,  m_Sam*sizeof(_RSDATA) );

		delete  m_pOrgRS;
	}
	m_pOrgRS = pOrgRS;

	m_MaxSam = MaxSam;
}

void Stroke::Clear()
{
	if( m_pOrg != 0   )
		delete m_pOrg;

	if( m_pOrgRS != 0 )
		delete m_pOrgRS;

	m_Dim    =  0;
	m_Sam    =  0;
	m_MaxSam = -1;

	m_pOrg   =  0;
	m_pOrgRS =  0;
}

bool  Stroke::GetAt( int Idx, int *pX )
{
	if( Idx > m_Sam-1 || pX == 0 )
		return false;

	float * pCur = m_pOrg + m_Dim*Idx;

	for( int k = 0; k < m_Dim; k++, pCur++, pX++ )
		pX[0] = (int)pCur[0];

	return true;
}

bool  Stroke::GetAt( int Idx, float *pX )
{
	if( Idx > m_Sam-1 || pX == 0 )
		return false;

	float * pCur = m_pOrg + m_Dim*Idx;

	for( int k = 0; k < m_Dim; k++, pCur++, pX++ )
		pX[0] = pCur[0];

	return true;
}

bool  Stroke::GetAt( int Idx, float *pX, float *pY, float *pZ )
{
	if( Idx > m_Sam-1 )
		return false;

	float * pCur = m_pOrg + m_Dim*Idx;

	*pX = pCur[0];
	*pY = pCur[1];
	if( pZ != 0 )
	   *pZ = pCur[2];

	return   true;
}

bool  Stroke::GetAt( int Idx, int *pX, int *pY, int *pZ )
{
	if( Idx > m_Sam-1 )
		return false;

	float * pCur = m_pOrg + m_Dim*Idx;

	*pX = (int)pCur[0];
	*pY = (int)pCur[1];
	if( pZ != 0 )
	   *pZ = (int)pCur[2];

	return true;
}

void  Stroke::Add( float x, float y )
{
	if( m_Dim != 2 )
		return;

	if( m_Sam >= m_MaxSam )
		Resize( m_MaxSam + 128 );

	float * pCur = m_pOrg + m_Dim*m_Sam;
	pCur[0] = x;
	pCur[1] = y;

	if( m_Sam > 0 )
	{
		pCur -= m_Dim;
		float  dx = x  - pCur[0]; 
		float  dy = y  - pCur[1]; 
		float  ds = (float)sqrt( dx*dx + dy*dy );
		m_pOrgRS[m_Sam].s = ds;
		m_pOrgRS[m_Sam].r = m_pOrgRS[m_Sam-1].r + ds;
	}
		
	m_Sam++;
}

void  Stroke::Add( int x, int y )
{
	Add( (float)x, (float)y );
}

void  Stroke::Add( float x, float y, float z )
{
	if( m_Dim != 3 )
		return;

	if( m_Sam >= m_MaxSam )
		Resize( m_MaxSam + 128 );

	float * pCur = m_pOrg + m_Dim*m_Sam;
	pCur[0] = x;
	pCur[1] = y;
	pCur[2] = z;

	if( m_Sam > 0 )
	{
		pCur -= m_Dim;
		float  dx = x  - pCur[0]; 
		float  dy = y  - pCur[1]; 
		float  dz = z  - pCur[2]; 
		float  ds = (float)sqrt( dx*dx + dy*dy + dz*dz );
		m_pOrgRS[m_Sam].s = ds;
		m_pOrgRS[m_Sam].r = m_pOrgRS[m_Sam-1].r + ds;
	}
		
	m_Sam++;
}

void  Stroke::Add( int x, int y, int z )
{
	Add( (float)x, (float)y, (float)z );
}

bool  Stroke::Add( int Dim, float * pX )
{
	int k;

	if( m_Dim != Dim || pX == 0 )
		return false;

	if( m_Sam >= m_MaxSam )
		Resize( m_MaxSam + 128 );

	float * pCur = m_pOrg + m_Dim*m_Sam;

	for( k = 0; k < m_Dim; k++ )
		pCur[k] = pX[k];

	if( m_Sam > 0 )
	{
		pCur -= m_Dim;

		float  ds = 0.0f;
		for( k =0; k < m_Dim; k++ )
		{
			float  d = pCur[k+m_Dim]  - pCur[k]; 
			ds += d* d;
		}
		ds = (float)sqrt( ds );

		m_pOrgRS[m_Sam].s = ds;
		m_pOrgRS[m_Sam].r = m_pOrgRS[m_Sam-1].r + ds;
	}
		
	m_Sam++;

	return true;
}

void  Stroke::Tracing()
{
	float  dl = m_pOrgRS[0].s = m_pOrgRS[0].r = 0.0f;

	float  * pA  = m_pOrg;
	p_RSDATA pRS = m_pOrgRS + 1;
	for( int i = 1; i < m_Sam; i++, pRS++ )
	{
		double  ds = 0.0;
		for( int k = 0; k < m_Dim; k++, pA++ )
		{
			float dx = pA[m_Dim] - pA[0];
			ds += dx * dx;
		}
		ds     = sqrt(  ds );
		pRS->s = (float)ds;
		pRS->r = (pRS-1)->r + (float)ds;
   }
}

