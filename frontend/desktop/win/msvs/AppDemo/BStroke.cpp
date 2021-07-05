// BStroke.cpp: implementation of the BStroke class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include <vector>

#include "appdemo.h"
#include "BStroke.h"
#include "OptiSpline.h"

#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


BStroke::BStroke()
{
	m_BBox.SetRectEmpty();
	m_ZBox.SetRectEmpty();
}

void BStroke::Clear()
{
    m_OrgPoints.clear();
    m_AppPoints.clear();
    m_RsmPoints.clear();
    m_BezPoints.clear();

    m_Sam = 0;
    m_ReSam = 0;
    m_Ord = 0;

    m_Err = 0.0f;
    m_Lam = 1.0f;

    m_BBox.SetRectEmpty();
    m_ZBox.SetRectEmpty();

    DestroyOptiSpline( m_pOptiSpline );
    m_pOptiSpline = 0;
}

void BStroke::Init(std::vector<float>& Points, int nOrd, int nReSam) 
{
    if (Points.size() > 2 && nOrd > 0 && nReSam > nOrd)
    {
        m_OrgPoints = Points;
        UpdateBBox();
        m_Sam = Points.size() / 2;

        SetOptiSpline(nOrd, nReSam);
    }
}

void BStroke::Reset(int nOrd, int nReSam)
{
    if (m_OrgPoints.size() > 2 && nOrd > 0 && nReSam > nOrd)
        SetOptiSpline(nOrd, nReSam);
}

BStroke::BStroke( const BStroke& Strk ) 
{
    if (Strk.m_pOptiSpline)
        m_pOptiSpline = CopyOptiSpline(Strk.m_pOptiSpline);
}

BStroke::~BStroke()
{
    DestroyOptiSpline(m_pOptiSpline);
}

bool  BStroke::SetOptiSpline( int nOrd, int ReSam )
{
    if(m_pOptiSpline)
        DestroyOptiSpline(m_pOptiSpline);

    m_ZBox.SetRectEmpty();

    m_ReSam = ReSam;

    int Dim = 2;
    int Sam = m_OrgPoints.size() / Dim;
    m_pOptiSpline = CreateOptiSpline( Dim, Sam, nOrd, ReSam, m_OrgPoints.data() );

    return GetAppResults();
}

bool BStroke::AppStep()
{
    if (!m_pOptiSpline)
        return false;

    OptiSplineApprox(m_pOptiSpline, 1);

    return GetAppResults();
}

bool BStroke::GetAppResults()
{
    if (!m_pOptiSpline)
        return false;

    int Dim = 2;
    m_AppPoints.resize(Dim * m_ReSam);
    if (GetOptiSplineApp(m_pOptiSpline, m_AppPoints.data(), Dim, m_ReSam) != m_ReSam)
        return false;

    m_RsmPoints.resize(Dim * m_ReSam);
    if (GetOptiSplineRsm(m_pOptiSpline, m_RsmPoints.data(), Dim, m_ReSam) != m_ReSam)
        return false;

    m_Lam = GetOptiSplineLam(m_pOptiSpline);
    m_Err = GetOptiSplineErr(m_pOptiSpline);

    return true;
}

CRect BStroke::GetBBox( bool bBezier )
{
     if( m_BBox.IsRectEmpty() )
         UpdateBBox();

    if( bBezier )
    {
        if( m_ZBox.IsRectEmpty() )
            UpdateZBox();
    }

    return m_BBox | m_ZBox;
}

CRect BStroke::GetZBox()
{
    if( m_ZBox.IsRectEmpty() )
        UpdateZBox();

    return m_ZBox;
}

void BStroke::UpdateBBox()
{
	if( m_OrgPoints.empty() ) 
        return;

	m_BBox.SetRectEmpty();
     
	for( auto it = std::begin(m_OrgPoints); it != std::end(m_OrgPoints);  )
	{
        int  x = (int)(*it); ++it;
        int  y = (int)(*it); ++it;
		m_BBox |= CRect( x-1, y-1, x+1, y+1 );
	}
}

void BStroke::UpdateZBox()
{
	m_ZBox.SetRectEmpty();

    for (auto it = std::begin(m_BezPoints); it != std::end(m_BezPoints); )
	{
        int  x = (int)(*it); ++it;
        int  y = (int)(*it); ++it;
		m_ZBox |= CRect( x-1, y-1, x+1, y+1 );
	}
}

/*
bool BStroke::GetBPoint( int Idx, float *pX0, float *pY0 )
{
    if( Idx < 0 || Idx >= m_BOrd )
        return false;

    float * pX = m_pApp + Idx*m_Dim;

    *pX0  = pX[0];
    *pY0  = pX[1];

    return true;
}

float BStroke::FindBPoints( int Ord, float * pNorms )
{
	int     i, j;

	double * pP  = new double[Ord*Ord];
	memset(  pP, 0, Ord*Ord*sizeof(double) );
	pP[0]       =  1.0;

	double * pQ  = new double[Ord*Ord];
	memset(  pQ, 0, Ord*Ord*sizeof(double) );

	double * pP1 = pP;
	double * pQ1 = pQ;
	double * pP2 = pP1 + Ord;
	double * pQ2 = pQ1 + Ord;
	for( i = 1; i < Ord; i++, pP2+=Ord, pQ2+=Ord, pP1+=Ord, pQ1+=Ord )
	{
		for( j = 0; j <= i; j++ )
		{
			pP2[j]  = - pQ1[j]; 
			pQ2[j]  =   pP1[j];
			if( j  == 0 ) continue;

			pP2[j] +=   pP1[j-1];
			pQ2[j] +=   pQ1[j-1];
			if( j  == 1 ) continue;
			
			pP2[j] +=   pQ1[j-2];
		}
	}

	double * pM  = new double[Ord*Ord];
	memset( pM, 0, Ord*Ord*sizeof(double) );
	pM[0]       =  1.0f;

	double * pMM = pM; // Current  element
	double * pMD = pM; // Diagonal elements

	for( i = 0; i < Ord-1; pMD += Ord+1 )
	{
		for( j = i+1; j < Ord; j++, pMM++ )
			pMM[1] = - (pMM[0]/(double)(j-i))*(double)(Ord-j);

		i++;

		pMD[Ord+1] = (pMD[0]/(double)i)*(double)(Ord-i);

		pMM += i+1;
	}


	double * pC   = new double[Ord];
	float  * pCfs = m_pCfs;
	float  * pB   = m_pBPoints;

	for( int k = 0; k < m_Dim; k++, pB += Ord, pCfs += Ord )
	{
		memset( pC, 0, Ord*sizeof(double) );

		for( i = 0; i < Ord; i++ )
		{
			for( j = 0; j < Ord; j++ )
				pC[j] += ((double)pCfs[i]/ (double)pNorms[i]) * pP[j + i*Ord] ;
		}

//  Substitution cos(t) = 1-2t
		double  Mod = 1.0f;
		for( i = 0; i < Ord; i++ )
		{
			double  S = 0.0f;
			double  C = 1.0f;
			for( j = i; j < Ord; j++ )
			{
				S += C * pC[j];
				C *= (double)(j+1) / (double)(j+1-i);
			}
			pQ[i] = S * Mod;

			Mod *= -2.0;
		}
		memcpy( pC, pQ, Ord*sizeof(double) );

		pMM   = pM;        // Current  element
		pMD   = pM+Ord+1; // Diagonal elements
		pB[0] = (float)pC[0]; 
		for( i = 1; i < Ord; i++, pMD += Ord+1 )
		{
			double S = 0.0f;

			pMM = pM + i; // Current  element
			for( j = 0; j < i; j++,  pMM += Ord )
				S += pB[j] * (pMM[0]/pMD[0]);
			
			pB[i] = (float)(pC[i]/pMD[0] - S);
		}
	}

	delete pC;
	delete pM;
	delete pP;
	delete pQ;

    m_BOrd = Ord;
    UpdateZBox();

	float x1 = m_pBPoints[0];
	float y1 = m_pBPoints[Ord];
	float z1 = 0.0f;
	if( m_Dim == 3 )
		z1 = m_pBPoints[2*Ord];

	float L = 0.0f;
	for( i = 1; i < Ord; i++ )
	{
		float x2 = m_pBPoints[i];
		float y2 = m_pBPoints[i+Ord];
		float z2 = 0.0f;
		if( m_Dim == 3 )
			z2 = m_pBPoints[i+2*Ord];

		float s = 0.0f ;

		float d;
		d  = x2 - x1;
		s += d*d;
		d  = y2 - y1;
		s += d*d;
		d  = z2 - z1;
		s += d*d;

		L += (float)sqrt( s );

		x1 = x2;
		y1 = y2;
		z1 = z2;
	}

	return  L;
}

void BStroke::Casteljau( float T, float * pX0, float * pY0 )
{
    int     j, Ord;

    T = 1.0f - T;

    float * pPnt = new float[m_Dim*m_ReSam];

    float * pX = m_pBPoints ;
   	float * pY = pX + m_BOrd;
   	for( j = 0; j < m_BOrd; j++ )
    {
         pPnt[m_Dim*j  ] = pX[j];
         pPnt[m_Dim*j+1] = pY[j];
    }

    for( Ord = m_BOrd; Ord > 1; Ord-- )
    {
        for( j = 0; j < Ord-1; j++ )
	    {
            float x1 = pPnt[m_Dim*(j+1)  ];
            float y1 = pPnt[m_Dim*(j+1)+1];
            float x0 = pPnt[m_Dim* j     ];
            float y0 = pPnt[m_Dim* j   +1];

            float x  = T*x0 + (1.0f-T)*x1;
            float y  = T*y0 + (1.0f-T)*y1;

            pPnt[m_Dim*j    ] = x;
            pPnt[m_Dim*j + 1] = y;
        }
    }

    *pX0 = pPnt[0];
    *pY0 = pPnt[1];

    delete  pPnt;
}

void BStroke::UpdateBCurve()
{
    int     i, j, Ord;

    float * pPnt = new float[m_Dim*m_ReSam];

    float dT = 1.0f / (m_ReSam-1);
    float  T = 1.0f; 
   	for( i = 0; i < m_ReSam; i++ )
    { 
	    float * pX = m_pBPoints ;
    	float * pY = pX + m_BOrd;
    	for( j = 0; j < m_BOrd; j++ )
	    {
            pPnt[m_Dim*j  ] = pX[j];
            pPnt[m_Dim*j+1] = pY[j];
        }

        for( Ord = m_BOrd; Ord > 1; Ord-- )
        {
        	for( j = 0; j < Ord-1; j++ )
	        {
                float x1 = pPnt[m_Dim*(j+1)  ];
                float y1 = pPnt[m_Dim*(j+1)+1];
                float x0 = pPnt[m_Dim* j     ];
                float y0 = pPnt[m_Dim* j   +1];

                float x  = T*x0 + (1.0f-T)*x1;
                float y  = T*y0 + (1.0f-T)*y1;

                pPnt[m_Dim* j     ] = x;
                pPnt[m_Dim* j   +1] = y;
            }
        }

        m_pApp[m_Dim*i  ] = pPnt[0];
        m_pApp[m_Dim*i+1] = pPnt[1];

        T -= dT;
    }

    delete  pPnt;
}

void    BStroke::Elevate( int nElevate )
{
    int i, j;

    if( m_BOrd+nElevate > m_ReSam )
        return; 

    for( i = 0; i < nElevate; i++ )
    {
	    float * pX = m_pBPoints ;
    	float * pY = pX + m_BOrd;

        float * pT  = new float[ m_Dim*(m_BOrd+1) ];
	    float * pXt = pT ;
    	float * pYt = pXt + m_BOrd + 1;
        pXt[0] = pX[0];
        pYt[0] = pY[0];
        for( j = 1; j < m_BOrd; j++ )
        {
            float t = (float)j/(float)m_BOrd;
            pXt[j]  = t*pX[j-1] + (1.0f-t)*pX[j]; 
            pYt[j]  = t*pY[j-1] + (1.0f-t)*pY[j]; 
        }
        pXt[m_BOrd] = pX[m_BOrd-1];
        pYt[m_BOrd] = pY[m_BOrd-1];
        memcpy( m_pBPoints, pT, m_Dim*sizeof(float)*(m_BOrd+1) );
        delete  pT;
        m_BOrd++;
    }

    UpdateBCurve();
}

float FNorm( float Parm, float Parm0, float EditWidth )
{
    float  Norm = 1.0f;
    float  Sigm = 3.0f / EditWidth; // Norm will be 0.1 on Edit Width

    Norm = Sigm*(Parm - Parm0);
    Norm = 1.0f / (1.0f + Norm*Norm);

    return Norm;
}

float BStroke::EditStroke( float Param, float dx, float dy, float EditWidth )
{
    int     i  ,   j;
    float   dBx, dBy;     
    float   B1 , B2, T;  

    Param = (float)(m_BOrd-1) * (Param / (float)(m_ReSam-1));

    int BIdx1 = (int)Param; 
    if( BIdx1 > m_BOrd-2 )
		BIdx1 = m_BOrd-2;

    int BIdx2 = BIdx1 + 1;

    float   p   = (Param - (float)BIdx1);
    float   q   = 1.0f  - p; 

    if( BIdx1 == 0 )
    {
        if( p < 0.02f )
        { 
			p = 0.0f; q = 1.0f; 
		}
        else
        { 
			p = 1.0f; q = 0.0f; 
		}
    }

    if( BIdx2 == m_BOrd-1 )
    {
        if( p > 0.8f )
        { 
			p = 1.0f; q = 0.0f; 
		}
        else
        {  
			p = 0.0f; q = 1.0f; 
		}
    }

    T = Param / (float)(m_BOrd-1);
    Bernstein( m_BOrd-1, BIdx1, T, &B1, &B2 );

    float          CurrX,  CurrY  ;
    Casteljau( T, &CurrX, &CurrY );

    EPoint * pE0 = new EPoint( 1.0f,
                               CurrX   ,  CurrY, 
                               CurrX+dx,  CurrY+dy,
                               B1      ,  B2,
                               Param, q,  BIdx1 );

    m_EditPoints.Add( pE0 );

    for( i = BIdx2, j = BIdx1; i < m_BOrd || j >= 0; i++, j-- )
    {
         float dXcurr, dYcurr, Norm;
         if( i < m_BOrd )
         {
            T = (float)i / (float)(m_BOrd-1);
            Casteljau( T, &CurrX, &CurrY );
            Bernstein( m_BOrd-1, i, T, &B1, NULL );

            Norm = FNorm( (float)i, Param, EditWidth );
            dXcurr = dx * Norm;
            dYcurr = dy * Norm;

            EPoint * pE  = new EPoint( Norm,
                                       CurrX         ,  CurrY, 
                                       CurrX + dXcurr,  CurrY+dYcurr,
                                       B1, -1.0f,
                                       (float)i, 1.0f,  i );
            m_EditPoints.Add( pE );

//          if( dXcurr*dXcurr + dYcurr*dYcurr < 0.1f )
//                i = m_BOrd;
         }

         if( j >= 0 )
         {
            T = (float)j / (float)(m_BOrd-1);
            Casteljau( T, &CurrX, &CurrY );
            Bernstein( m_BOrd-1, j, T, &B1, &B2 );

            Norm   = FNorm( (float)j, Param, EditWidth );
            dXcurr = dx * Norm;
            dYcurr = dy * Norm;

            EPoint * pE  = new EPoint( Norm,
                                       CurrX         ,  CurrY, 
                                       CurrX + dXcurr,  CurrY+dYcurr,
                                       B1, -1.0f,
                                       (float)j, 1.0f,  j );
            m_EditPoints.Add( pE );

//            if( dXcurr*dXcurr + dYcurr*dYcurr < 0.1f )
//                j = -1;
         } // if( j >= 0 )
    } //for( i,j )

    int    EItr = 0;
    while( 1 )
    {
        EPoint * pE0 = m_EditPoints[0];

        Casteljau( pE0->m_BParam / (float)(m_BOrd-1), &CurrX, &CurrY );

        pE0->m_CurrX = CurrX;
        pE0->m_CurrY = CurrY;
        float dXcurr = pE0->m_TargX - pE0->m_CurrX;
        float dYcurr = pE0->m_TargY - pE0->m_CurrY;

        float   K  = 1.0f/(pE0->m_q*pE0->m_B1 + (1.0f - pE0->m_q)*pE0->m_B2);

        dBx =         pE0->m_q  * K * dXcurr;
        dBy =         pE0->m_q  * K * dYcurr;
        ChangeBPoint( pE0->m_BIdx  , dBx, dBy, FALSE );

        dBx = (1.0f - pE0->m_q) * K * dXcurr;
        dBy = (1.0f - pE0->m_q) * K * dYcurr;
        ChangeBPoint( pE0->m_BIdx+1, dBx, dBy, FALSE );

        float Distort2 = dXcurr*dXcurr + dYcurr*dYcurr;
        if( Distort2 < 0.001f || EItr++ > 15 )
            break;

        for( i = 1; i < m_EditPoints.GetSize(); i++ )
        {
            EPoint * pE = m_EditPoints[i];
            Casteljau( pE->m_BParam / (float)(m_BOrd-1), &CurrX, &CurrY );

            pE->m_CurrX = CurrX;
            pE->m_CurrY = CurrY;
            pE->m_TargX = pE->m_OrigX + pE->m_Norm*(CurrX - pE->m_OrigX);
            pE->m_TargY = pE->m_OrigY + pE->m_Norm*(CurrY - pE->m_OrigY);

            dBx = (pE->m_TargX - pE->m_CurrX) / pE->m_B1;
            dBy = (pE->m_TargY - pE->m_CurrY) / pE->m_B1;

            ChangeBPoint( pE->m_BIdx, dBx, dBy, FALSE );
        }// for( i )
    } // while(1)

    for( i = 0; i < m_EditPoints.GetSize(); i++ )
        delete m_EditPoints[i];
    m_EditPoints.RemoveAll(  );

    UpdateBCurve();

    return Param;
}
*/
/*
float BStroke::EditStroke( float Param, float dx, float dy, float EditWidth )
{
    int     i, j;
    float   dBx, dBy;     

    Param = (float)(m_BOrd-1) * (Param / (float)(m_ReSam-1));

    int     BIdx1 = (int)Param; 
    if( BIdx1 > m_BOrd-2 ) BIdx1 = m_BOrd-2;
    int     BIdx2 = BIdx1 + 1;

    float   p   = (Param - (float)BIdx1);
    float   q   = 1.0f  - p; 

    if( BIdx1 == 0 )
    {
        if( p < 0.02f )
        {   p = 0.0f; q = 1.0f; }
        else
        {   p = 1.0f; q = 0.0f; }
    }

    if( BIdx2 == m_BOrd-1 )
    {
        if( p > 0.8f )
        {   p = 1.0f; q = 0.0f; }
        else
        {   p = 0.0f; q = 1.0f; }
    }

    float   T = Param / (float)(m_BOrd-1);

    float          CurrX,  CurrY  ;
    Casteljau( T, &CurrX, &CurrY );

    EPoint * pE0 = new EPoint( 1.0f,
                               CurrX   ,  CurrY, 
                               CurrX+dx,  CurrY+dy,
                               Param, q,  BIdx1 );

    m_EditPoints.Add( pE0 );

    for( i = BIdx2, j = BIdx1; i < m_BOrd || j >= 0; i++, j-- )
    {
         float dXcurr, dYcurr, Norm;
         if( i < m_BOrd )
         {
            //GetBPoint( i, &CurrX, &CurrY );
            Casteljau( (float)i/(float)(m_BOrd-1), &CurrX, &CurrY );

            Norm = FNorm( (float)i, Param, EditWidth );
            dXcurr = dx * Norm;
            dYcurr = dy * Norm;

            EPoint * pE  = new EPoint( Norm,
                                       CurrX         ,  CurrY, 
                                       CurrX + dXcurr,  CurrY+dYcurr,
                                       (float)i, 1.0f,  i );
            m_EditPoints.Add( pE );

            if( dXcurr*dXcurr + dYcurr*dYcurr < 0.1f )
                i = m_BOrd;
         }

         if( j >= 0 )
         {
            //GetBPoint( j, &CurrX, &CurrY );
            Casteljau( (float)j/(float)(m_BOrd-1), &CurrX, &CurrY );

            Norm   = FNorm( (float)j, Param, EditWidth );
            dXcurr = dx * Norm;
            dYcurr = dy * Norm;

            EPoint * pE  = new EPoint( Norm,
                                       CurrX         ,  CurrY, 
                                       CurrX + dXcurr,  CurrY+dYcurr,
                                       (float)j, 1.0f,  j );
            m_EditPoints.Add( pE );

            if( dXcurr*dXcurr + dYcurr*dYcurr < 0.1f )
                j = -1;
         } // if( j >= 0 )
    } //for( i,j )

    int    EItr = 0;
    while( m_EditPoints.GetSize() )
    {
        float   Distort2 = 0.0f;
        for( i = 0; i < m_EditPoints.GetSize(); i++ )
        {
            EPoint * pE = m_EditPoints[i];

            float   T = pE->m_BParam / (float)(m_BOrd-1);

            float   B1, B2;  
            Bernstein( m_BOrd-1, pE->m_BIdx, T, &B1, &B2 );

            float   K  = 1.0f/(pE->m_q*B1 + (1.0f - pE->m_q)*B2);

            float dXcurr = pE->m_TargX - pE->m_CurrX;
            float dYcurr = pE->m_TargY - pE->m_CurrY;

            dBx =         pE->m_q  * dXcurr * K;
            dBy =         pE->m_q  * dYcurr * K;
            ChangeBPoint( pE->m_BIdx  , dBx, dBy, FALSE );

            dBx = (1.0f - pE->m_q) * dXcurr * K;
            dBy = (1.0f - pE->m_q) * dYcurr * K;
            ChangeBPoint( pE->m_BIdx+1, dBx, dBy, FALSE );

            Distort2 = 0.0f;
            for( j = 0; j < m_EditPoints.GetSize(); j++ )
            {
                EPoint   * pE = m_EditPoints[j];

//                if( j == 0 )  // Main point
                    Casteljau( pE->m_BParam / (float)(m_BOrd-1), &CurrX, &CurrY );
//                else          // All others 
//                    GetBPoint( pE->m_BIdx, &CurrX, &CurrY );

                pE->m_TargX = pE->m_CurrX + pE->m_Norm*(CurrX - pE->m_CurrX);
                pE->m_TargY = pE->m_CurrY + pE->m_Norm*(CurrY - pE->m_CurrY);

//                pE->m_CurrX = CurrX;
//                pE->m_CurrY = CurrY;

                Distort2 += (pE->m_TargX - CurrX)*(pE->m_TargX - CurrX);   
                Distort2 += (pE->m_TargY - CurrY)*(pE->m_TargY - CurrY);   
            } // for( j )
            Distort2 /= (float)m_EditPoints.GetSize();
            if( Distort2 < 0.01f )
                break;
        }// for( i )

        if( m_EditPoints.GetSize() == 1 )
        {   // Delete last point and go out
            delete m_EditPoints[0];
            m_EditPoints.RemoveAll();
        }

        if( (Distort2 < 0.01f || EItr++ > 5) && m_EditPoints.GetSize() > 1 )
        {   // Delete all additional points and repeat for main one
            int  n = m_EditPoints.GetSize();
            for( i = 1; i < n; i++ )
            {
                delete m_EditPoints[n-i];
                m_EditPoints.RemoveAt( n-i );
            }
        }

    } // while( m_EditPoints.GetSize() )

    UpdateBCurve();

    return Param;
}
*/