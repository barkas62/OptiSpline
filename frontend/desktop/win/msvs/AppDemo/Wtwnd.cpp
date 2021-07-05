// WTView.cpp : implementation file
//

#include "stdafx.h"

#include "WTWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWTWnd

IMPLEMENT_DYNAMIC(CWTWnd, CView)

CWTWnd::CWTWnd()
{
    m_ScrW       = 0;  
    m_ScrH       = 0;
    m_OldBtn     = 0;
    m_bCollect   = FALSE;
    m_bEnable    = FALSE;

	m_bInker     = TRUE;
	m_bFunct     = FALSE;

	m_InkData.RemoveAll();
}

CWTWnd::~CWTWnd()
{
	m_InkData.RemoveAll();
}


BEGIN_MESSAGE_MAP(CWTWnd, CView)
        //{{AFX_MSG_MAP(CWTWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CWTWnd diagnostics

#ifdef _DEBUG
void CWTWnd::AssertValid() const
{
        CView::AssertValid();
}

void CWTWnd::Dump(CDumpContext& dc) const
{
        CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CWTWnd message handlers



void CWTWnd::OnLButtonDown(UINT nFlags, CPoint p ) 
{
	if( m_bInker )
	{
		m_bCollect = TRUE;

		m_OldPnt.x  = p.x;
		m_OldPnt.y  = p.y;

		m_InkData.Add ((WORD) p.x );
		m_InkData.Add ((WORD) p.y );
		m_InkData.Add ((WORD)(-1) );

		InkerStart();
	}

    CView::OnLButtonDown( nFlags, p );
}

void CWTWnd::OnLButtonUp(UINT nFlags, CPoint p) 
{
	if( m_bInker )
	{
	    m_bCollect = FALSE;
		m_bEnable  = FALSE;

		InkerEnd();

		ClearInkData();
		CView::OnLButtonUp(nFlags, p);
	}
}

void CWTWnd::OnMouseMove(UINT nFlags, CPoint p) 
{
    if( m_bCollect == TRUE && m_bInker )
	{
       if ( m_OldPnt.x != p.x || 
	        m_OldPnt.y != p.y  )
       {
			if( !m_bEnable )
			{
				if( m_bFunct )
				{
					if( p.x <= m_OldPnt.x )
					{
					    CView::OnMouseMove( nFlags, p );
						return;
					}
				}

				CDC  *pDC = GetDC();
				if( pDC )
				{
					CPen * pPen = pDC->SelectObject ( &m_InkPen );
					pDC->MoveTo( m_OldPnt.x, m_OldPnt.y );
					pDC->LineTo( p.x, p.y );
					pDC->SelectObject( pPen );
					ReleaseDC ( pDC );
				}

				m_OldPnt.x  = p.x;
				m_OldPnt.y  = p.y;

				m_InkData.Add ((WORD) p.x );
				m_InkData.Add ((WORD) p.y );
				m_InkData.Add ((WORD)(-1) );
			} // if( !m_bEnable )
       }
	} // if( m_bCollect == TRUE )

    CView::OnMouseMove( nFlags, p );
}


void   CWTWnd::ClearInkData   ( void  )
{
   m_InkData.RemoveAll();
}

void CWTWnd::SetSolidPen ( int w, COLORREF color )
{
    m_InkPen.CreatePen ( PS_SOLID, w, color );
}


int CWTWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
    if( CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	SetSolidPen( 1, RGB(128,0,0) );

	m_ScrW = GetSystemMetrics ( SM_CXSCREEN );
	m_ScrH = GetSystemMetrics ( SM_CYSCREEN );

	return 0;
 }

void CWTWnd::OnDestroy() 
{
    CView::OnDestroy();
}



