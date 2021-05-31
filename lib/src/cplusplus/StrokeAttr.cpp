#include <memory.h>
#include "Stroke.h"
#include "StrokeAttr.h"
#include "AppStroke.h"

StrokeAttr::StrokeAttr()
{
	m_Dim    =  0;
	m_Ord    =  0;
	m_ReSam  =  0;
	m_pCfs   =  0;
	m_Norm   =  1.0f;  

	m_pShift =  0;

	m_Color  =  0L;
	m_Width  =  0L;
}

StrokeAttr::StrokeAttr( int Dim )
{
	m_Dim    =  Dim;
	m_ReSam  =  0;
	m_pCfs   =  0;
	m_Norm   =  1.0f;  
	if( m_Dim != 0 )
	{
		m_pShift = new float[m_Dim];
		memset( m_pShift, 0, m_Dim*sizeof(float) ); 
	}

	m_Color  =  0L;
	m_Width  =  0L;
}

StrokeAttr::StrokeAttr( StrokeAttr& SA )
{
	m_Dim = SA.m_Dim;
	m_Ord = SA.m_Ord;

	m_ReSam  =  0;
	m_pCfs   =  0;
	m_pShift =  0;

	if( m_Dim == 0 )
		return;

	if( SA.m_pCfs != 0 && SA.m_ReSam > 0 )
	{
		m_pCfs = new float[m_Dim*SA.m_ReSam];
		memcpy( m_pCfs, SA.m_pCfs, m_Dim*SA.m_ReSam*sizeof(float) ); 
		m_ReSam = SA.m_ReSam;
	}

	if( SA.m_pShift != 0 )
	{
		m_pShift = new float[m_Dim];
		memcpy( m_pShift, SA.m_pShift, m_Dim*sizeof(float) ); 
	}

	m_Norm   =  SA.m_Norm;  

	m_Color  =  SA.m_Color;
	m_Width  =  SA.m_Width;
}

StrokeAttr::StrokeAttr( AppStroke& Strk, int Ord )
{
	m_Dim = Strk.m_Dim;
	m_Ord = Ord;

	m_ReSam  =  0;
	m_pCfs   =  0;
	m_pShift =  0;

	if( m_Dim == 0 )
		return;

	if( Strk.m_pCfs != 0 && Strk.m_ReSam > 0 )
	{
		m_pCfs = new float[m_Dim*Strk.m_ReSam];
		memcpy( m_pCfs, Strk.m_pCfs, m_Dim*Strk.m_ReSam*sizeof(float) ); 
		m_ReSam = Strk.m_ReSam;
	}

	if( Strk.m_pShift != 0 )
	{
		m_pShift = new float[m_Dim];
		memcpy( m_pShift, Strk.m_pShift, m_Dim*sizeof(float) ); 
	}

	m_Norm   =  Strk.m_Norm;  
}

StrokeAttr::~StrokeAttr()
{
	if( m_pCfs    ) delete m_pCfs  ;
	if( m_pShift  ) delete m_pShift;
}

bool   StrokeAttr::InitAttr( int ReSam )
{
	if( ReSam == 0 || ReSam == m_ReSam )
		return true;
	
	if( m_pCfs != 0 )
	{	delete m_pCfs; 	m_pCfs = 0; }

	m_pCfs   = new  float [m_Dim * ReSam];
	if( m_pCfs == 0 )
		return false;

	m_ReSam  =  ReSam;

	return  true;
}

