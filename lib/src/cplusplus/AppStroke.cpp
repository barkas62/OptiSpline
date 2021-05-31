#include "AppStroke.h"
#include <math.h>
#include <memory.h>


void AppStroke::Clear()
{
	if (m_pApp != 0)
		delete m_pApp;
	if (m_pRsm != 0)
		delete m_pRsm;
	if (m_pBck != 0)
		delete m_pBck;

	if (m_pCfs != 0)
		delete m_pCfs;

	if (m_pAppRS != 0)
		delete m_pAppRS;

	m_ReSam = 0;
	m_pApp = 0;
	m_pRsm = 0;
	m_pBck = 0;
	m_pCfs = 0;

	m_Err = -1.0f;
	m_Lam = -1.0f;

	m_pAppRS = 0;

	Stroke::Clear();
}

void AppStroke::Init( int Dim, int Sam, int Ord, int ReSam, float * pPnt) 
{
	Clear();

	if( Dim <= 0 || Sam <= 0 || Ord <= 0)
		return;

	Stroke::Init(Dim, ReSam);

	float* pP = pPnt;
	for (int i = 0; i < Sam; i++, pPnt += Dim)
		Add(Dim, pPnt);

	m_Basis.Init(Ord, ReSam);  

	m_pApp = new float[Dim*ReSam];
	memset( m_pApp, 0, Dim*ReSam*sizeof(float) ); 

	m_pRsm = new float[Dim*ReSam];
	memset( m_pRsm, 0, Dim*ReSam*sizeof(float) ); 

	m_pBck = new float[Dim*ReSam+1];
	memset( m_pBck, 0, (Dim*ReSam+1)*sizeof(float) );
		
	m_pCfs = new float[Dim*ReSam];
	memset( m_pCfs, 0, Dim*ReSam*sizeof(float) );  

	m_pAppRS = new _RSDATA[ReSam];
	memset( m_pAppRS, 0, ReSam*sizeof(_RSDATA) );
		
	m_ReSam = ReSam;

	SetNaturalParam();
	Approx();
	AppErr();
	Tracing();

}


AppStroke::AppStroke( AppStroke& S ) : Stroke( S )
{
	int  Dim =  S.m_Dim;

	m_ReSam  =  0;

	m_pApp   =  0;
	m_pRsm   =  0;
	m_pBck   =  0;

	m_pCfs    =  0;

	m_Err    = S.m_Err;
	m_Lam    = S.m_Lam;

	m_pAppRS =  0;

	m_Basis = S.m_Basis;


	if( S.m_ReSam != 0 )
	{
		if( S.m_pApp != 0 )
		{
			m_pApp = new float[Dim * S.m_ReSam];
			if( m_pApp != 0 && S.m_pApp != 0 )
				memcpy( m_pApp, S.m_pApp, Dim*S.m_ReSam*sizeof(float) ); 
		}

		if( S.m_pRsm != 0 )
		{
			m_pRsm = new float[Dim * S.m_ReSam];
			if( m_pRsm != 0 && S.m_pRsm != 0)
				memcpy( m_pRsm, S.m_pRsm, Dim*S.m_ReSam*sizeof(float) ); 
		}

		if( S.m_pBck != 0 )
		{
			m_pBck = new float[Dim*S.m_ReSam + 1];
			if( m_pBck != 0 && S.m_pBck != 0)
				memcpy( m_pBck, S.m_pBck, (Dim*S.m_ReSam+1)*sizeof(float) ); 
		}

		if( S.m_pAppRS != 0 )
		{
			m_pAppRS = new _RSDATA[S.m_ReSam];
			if( m_pAppRS != 0 && S.m_pAppRS )
				memcpy( m_pAppRS, S.m_pAppRS, S.m_ReSam*sizeof(_RSDATA) ); 
		}

		if( S.m_pCfs != 0 )
		{
			m_pCfs = new float[Dim*S.m_ReSam];
			if( m_pCfs != 0 && S.m_pCfs != 0)
				memcpy( m_pCfs, S.m_pCfs, Dim*S.m_ReSam*sizeof(float) ); 
		}

		m_ReSam = S.m_ReSam;
	}
}

AppStroke::~AppStroke()
{
	Clear();
}


void   AppStroke::SaveRsmState()
{
	int cb = m_Dim*m_ReSam*sizeof(float);
	memcpy( (void *)m_pBck, m_pRsm, cb );
}

void   AppStroke::LoadRsmState()
{
	int cb = m_Dim*m_ReSam*sizeof(float); 
	memcpy( m_pRsm, (void *)m_pBck, cb );
}

void   AppStroke::ParamApp( int nItr, float MaxErr )     
{
	for( int it = 0; it < nItr; it++ )
	{
		Repar  ();
		Approx ();
		AppErr ();
		Tracing();
		if (MaxErr > 0.0f && m_Err < MaxErr)
			break;
	} // End of it

}

void   AppStroke::Approx()
{
	int Ord = m_Basis.m_Ord;

	memset( m_pApp, 0, m_Dim*m_ReSam*sizeof(float) );
	for( int k = 0; k < m_Dim; k++ ) 
	{
		float  * pBas;
		pBas   = m_Basis.m_pBasis;
		for( int i = 0; i < Ord; i++ )
		{
			float   C  = 0.0f;
			float * pR = m_pRsm + k;
			for( int j = 0;  j < m_ReSam; j++, pR += m_Dim, pBas++ )
				C += (*pR) * (*pBas);

			m_pCfs[Ord*k+i] = C;
		}

		pBas   = m_Basis.m_pBasis;
		for( int i = 0; i < Ord; i++ )
		{
			float    C  = m_pCfs[Ord*k+i];
			float  * pA = m_pApp + k;
			for( int j  = 0; j < m_ReSam; j++, pA += m_Dim, pBas++ )
				*pA += C * (*pBas);
		}
	}
}

void   AppStroke::Decompose()
{
	int Ord = m_Basis.m_Ord;

	for( int k = 0; k < m_Dim; k++ ) 
	{
		float * pBas = m_Basis.m_pBasis;
		for( int i = 0; i < Ord; i++ )
		{
			float   C  = 0.0f;
			float * pR = m_pRsm + k;
			for( int j = 0;  j < m_ReSam; j++, pR += m_Dim, pBas++ )
				C += (*pR) * (*pBas);

			m_pCfs[Ord*k+i] = C;
		}
	}
}

void   AppStroke::Restore()
{
	int Ord = m_Basis.m_Ord;

	memset( m_pApp, 0, m_Dim*m_ReSam*sizeof(float) );

	for( int k = 0; k < m_Dim; k++ )
	{
		float  * pBas =  m_Basis.m_pBasis;
		for( int i = 0; i < Ord; i++ )
		{
			float    C  = m_pCfs[Ord*k+i];
			float  * pA = m_pApp + k;
			for( int j  = 0; j < m_ReSam; j++, pA += m_Dim, pBas++ )
				*pA += C * (*pBas);
		}
	}  //  for( k );
}

void   AppStroke::ResetParam()
{
	float      ds = m_pOrgRS[m_Sam-1].r / (float)(m_ReSam - 1);

	m_pAppRS[0].s = 0.0f;
	m_pAppRS[0].r = 0.0f;
	for( int i = 1; i < m_ReSam; i++ )
	{
	    m_pAppRS[i].s = ds;
		m_pAppRS[i].r = m_pAppRS[i-1].r + ds;
	}
}

void  AppStroke::Repar( )
{
	int       i, j, k;
	float     Alfa, Curr;

	for( k = 0; k < m_Dim; k++ )
	{
		m_pRsm[k]                   = m_pOrg[k];
		m_pRsm[m_Dim*(m_ReSam-1)+k] = m_pOrg[m_Dim*(m_Sam-1)+k];
	}

	m_pAppRS[0].idx = 0;
	m_pAppRS[0].alf = 0.0f;

	m_Lam  = m_pOrgRS[m_Sam-1].r / m_pAppRS[m_ReSam-1].r;

	for( i = j = 1; i < (m_ReSam - 1); i++ )
	{
		Curr = m_Lam * m_pAppRS[i].r;

		while( Curr  > m_pOrgRS[j].r + 0.001f )
		{ 
			m_pOrgRS[j].idx = i;

			j++;
		 
			if( j == m_Sam )
			{
				for( ; i < m_ReSam; i++ )
				{
					for( k = 0; k < m_Dim; k++ )
						m_pRsm[m_Dim*i+k] = m_pOrg[m_Dim*(m_Sam-1)+k] / m_Lam;
				}
				goto END;
			}	 // if( j==Sam )
		  
		}	// while

		if( m_pOrgRS[j].s == 0.0f )
			Alfa = 1.0f;
		else
			Alfa = 1 - (m_pOrgRS[j].r - Curr) / m_pOrgRS[j].s;

		for( k = 0; k < m_Dim; k++ )
		{
			float  x2 = m_pOrg[m_Dim*(j  )+k];
			float  x1 = m_pOrg[m_Dim*(j-1)+k];
			float  dx = x2 - x1;

			m_pRsm[m_Dim*i+k] = x1 + dx*Alfa;
		}

		m_pAppRS[i].idx = j-1;
		m_pAppRS[i].alf = Alfa;
   }
END:
   m_pAppRS[m_ReSam-1].idx = m_Sam-1;
   m_pAppRS[m_ReSam-1].alf = 1.0f;
}


void  AppStroke::Tracing()
{
	float  dl = m_pAppRS[0].s = m_pAppRS[0].r = 0.0f;

	float  * pA  = m_pApp;
	p_RSDATA pRS = m_pAppRS + 1;
	for( int i = 1; i < m_ReSam; i++, pRS++ )
	{
		double  ds = 0.0;
		for( int k = 0; k < m_Dim; k++, pA++ )
		{
			float dx = pA[m_Dim] - pA[0];
			ds += dx * dx;
		}
		ds     = sqrt( ds );
		pRS->s = (float)ds;
		pRS->r = (pRS-1)->r + (float)ds;
    }

	m_Lam = m_pOrgRS[m_Sam - 1].r / m_pAppRS[m_ReSam - 1].r;
}

void AppStroke::AppErr()
{
   m_Err  = 0;

   float * pA = m_pApp;
   float * pR = m_pRsm;
   for( int i = 0; i < m_ReSam; i++ )
   {
      float     E = 0;
      for( int  k = 0; k < m_Dim; k++, pA++, pR++ )
      {
         float  T = *pA - *pR;  
		 E  +=  T * T;
      }

      m_Err += E;
   }

   m_Err = (float)sqrt( m_Err / m_ReSam );
}

void  AppStroke::SetNaturalParam()
{
	ResetParam();
	Repar     ();
}


float  AppStroke::PolyError2( float * pDat )
{
	float   MaxErr = 0;
	float   Sin, Cos;

	if( pDat == 0 ) pDat = m_pRsm;

	m_Err         = 0.0f;

	float  * pR   = pDat;
	p_RSDATA pRS  = m_pAppRS;
	for( int i = 1; i < m_ReSam; i++, pR += m_Dim, pRS++ )
	{
	  float   x1  = pR[0      ]; 
	  float   x2  = pR[m_Dim  ]; 
	  float   y1  = pR[1      ]; 
	  float   y2  = pR[m_Dim+1]; 

	  float   t, dd;

	  t   = x1 - x2;
	  dd  = t * t;
	  t   = y1 - y2;
	  dd += t * t;
	  dd  = (float)sqrt(dd); 

	  Cos   = (x2 - x1) / dd;
	  Sin   = (y2 - y1) / dd;

	  int      j1 = pRS[0].idx;
	  int      j2 = pRS[1].idx;

	  float  * pD = m_pOrg + m_Dim*j1;
      for( int  j = j1; j < j2; j++, pD += m_Dim )
      {
		 float  x = pD[0] - x1;
		 float  y = pD[1] - y1;

		 float  H = -x*Sin + y*Cos;
		 if( H < 0 ) H = -H; 

		 if( MaxErr < H ) 
			 MaxErr = H;

		 m_Err += H;
      }
   }

   m_Err /= m_Sam;

   return  MaxErr;
}

void  AppStroke::Rsm2App()
{
	if( m_pApp && m_pRsm )
		memcpy( m_pApp, m_pRsm, m_Dim * m_ReSam * sizeof(float) );
}

void  AppStroke::App2Rsm()
{
	if( m_pApp && m_pRsm )
		memcpy( m_pRsm, m_pApp, m_Dim * m_ReSam * sizeof(float) );
}

bool  AppStroke::GetRsmAt( int Idx, float *pX, float *pY, float *pZ )
{
	if( Idx > m_ReSam-1 )
		return false;

	float * pCur = m_pRsm + m_Dim*Idx;

	*pX = pCur[0];
	*pY = pCur[1];
	if( pZ != 0 )
	   *pZ = pCur[2];


	return true;
}

bool  AppStroke::GetAppAt( int Idx, float *pX, float *pY, float *pZ )
{
	if( Idx > m_ReSam-1 )
		return false;

	float * pCur = m_pApp + m_Dim*Idx;

	*pX = pCur[0];
	*pY = pCur[1];
	if( pZ != 0 )
	   *pZ = pCur[2];

	return true;
}



