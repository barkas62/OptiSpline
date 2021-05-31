#include <math.h>
#include <memory.h>
#include "Stroke.h"
#include "AppStroke.h"
#include "Basis.h"

Basis::Basis()
{
	m_Ord    = 0; 
	m_ReSam  = 0; 
	m_pBasis = 0;
	m_pNorms = 0;
}

Basis::Basis( const Basis& Bas )
{
	m_Ord    = 0; 
	m_ReSam  = 0; 
	m_pBasis = 0;
	m_pNorms = 0;
	if( Bas.m_Ord > 0 && Bas.m_ReSam > 0 )
	{
		m_pBasis = new float[Bas.m_Ord*Bas.m_ReSam];
		if( m_pBasis )
		{
			m_Ord    = Bas.m_Ord; 
			m_ReSam  = Bas.m_ReSam; 
			if( Bas.m_pBasis!= 0 )
				memcpy( m_pBasis, Bas.m_pBasis, m_Ord*m_ReSam*sizeof(float) );
		}

		m_pNorms = new float[Bas.m_Ord];
		if( m_pNorms )
		{
			if( Bas.m_pNorms!= 0 )
				memcpy( m_pNorms, Bas.m_pNorms, m_Ord*sizeof(float) );
		}
	}
}

Basis::~Basis()
{
	ClearBasis( );
}

void Basis::Init( int Ord, int ReSam )
{
	if( Ord <= 0 || ReSam <= 0 )
		return; 

	if( Ord  != m_Ord || ReSam != m_ReSam )
	{
		if( m_pBasis != 0 )
		{	delete m_pBasis; m_pBasis = 0; }

		if( m_pNorms != 0 )
		{	delete m_pNorms; m_pNorms = 0; }

		m_pBasis = new float[Ord*ReSam];
		if( m_pBasis  )
		{
			m_Ord    = Ord; 
			m_ReSam  = ReSam; 

			m_pNorms = new float[Ord*ReSam];
		}
	}

	CosBasis();

	return;
}


void Basis::ClearBasis( )
{
	if( m_pBasis )
	{	delete 	m_pBasis;	m_pBasis = 0; }

	if( m_pNorms )
	{	delete 	m_pNorms;	m_pNorms = 0; }
}

void Basis::CosBasis( )
{
	double     dt   = 4.0 * atan(1.0) / m_ReSam;
	float*     pBas = m_pBasis;;
	for( int j = 0; j < m_ReSam; j++, pBas++ )
	    *pBas = 1.0f;

	for( int i = 1; i < m_Ord; i++ )
	{
		double t = dt / 2.0;
		for( int j = 0; j < m_ReSam; j++, t += dt, pBas++ )
			*pBas = (float)cos(i * t);
	} // for(i)

	Norm();
 }

void Basis::CosBasis( int ReSam0 )
{
	double     Pi   = 4.0 * atan(1.0);
	double     dt0  = Pi / (ReSam0<<1);
	double     dt1  = (Pi-2.0f*dt0) / (m_ReSam-1);
	float*     pBas = m_pBasis;;
	for( int j = 0; j < m_ReSam; j++, pBas++ )
	    *pBas = 1.0f;

	for( int i = 1; i < m_Ord; i++ )
	{
		double t = dt0;
		for( int j = 0; j < m_ReSam; j++, t += dt1, pBas++ )
			*pBas = (float)cos(i * t);
	} // for(i)

	Norm();
}

void   Basis::Norm( )
{
	int     i, j;
	float * pBas;

	for( i = 0; i < m_Ord; i++ )
	{
		float  N = 0.0f;

		pBas = m_pBasis + i*m_ReSam;
		for( j = 0; j < m_ReSam; j++, pBas++ )
			N += (*pBas) * (*pBas);

		N = (float)sqrt( N );
		m_pNorms[i] = N;

		pBas = m_pBasis + i*m_ReSam;
		for( j = 0; j < m_ReSam; j++, pBas++ )
			*pBas /= N;
    } // for(i)

 }



