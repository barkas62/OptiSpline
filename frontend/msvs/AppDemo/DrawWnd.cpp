// DrawWnd.cpp : implementation file
//

#include "stdafx.h"
#include "AppDemo.h"
#include "AppDemoDlg.h"
#include "DrawWnd.h"

#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//IMPLEMENT_DYNCREATE(CDrawWnd, CWTWnd)

CDrawWnd::CDrawWnd( CWnd * pHost )
{                  
   m_fDraw    = DRAW_ALL;
   m_bErase   = TRUE;

   m_pHostWnd = pHost;

   m_pBmp     = 0;

   m_X        = 0;
   m_Y        = 0;
   m_Z        = 0;

   m_BScale   = 1.0f;
   m_BShiftX  = 0.0f;
   m_BShiftY  = 0.0f;
}

CDrawWnd::~CDrawWnd()
{
    if( m_pBmp != 0 )
        delete  m_pBmp;
}

BEGIN_MESSAGE_MAP(CDrawWnd, CWTWnd)
	//{{AFX_MSG_MAP(CDrawWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
	ON_WM_ERASEBKGND()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

#ifdef _DEBUG
void CDrawWnd::AssertValid() const
{
	CView::AssertValid();
}

void CDrawWnd::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

void CDrawWnd::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

int CDrawWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if( CWTWnd::OnCreate(lpCreateStruct) == -1 )
		return -1;
	
    CreateBmp( );

	return 0;
}

void CDrawWnd::ClearStroke() {
	m_Strk.Clear();
}

void CDrawWnd::SetStroke(std::vector<float>& vPoints, int nOrd, int nReSam)
{
	m_InkData.RemoveAll();
	ClearBmp();

	m_Strk.Init(vPoints, nOrd, nReSam);

	InvalidateRect(NULL, FALSE);
	UpdateWindow();
}

void CDrawWnd::ResetStroke(int nOrd, int nReSam)
{
	m_InkData.RemoveAll();
	ClearBmp();

	m_Strk.Reset(nOrd, nReSam);

	InvalidateRect(NULL, FALSE);
	UpdateWindow();
}

void  CDrawWnd::ResetSource()
{
	m_Strk.Clear();

	//if( m_pHostWnd != NULL )
	//	m_pHostWnd->SendMessage( WM_SOURCE_RESET );

	m_InkData.RemoveAll();
	ClearBmp();

    InvalidateRect( NULL, FALSE );
	UpdateWindow();
}

void CDrawWnd::AppStep()
{
	m_Strk.AppStep();
	InvalidateRect(NULL, FALSE);
	UpdateWindow();
}

void  CDrawWnd::InkerStart()
{
	SetCapture();
	ResetSource();
}

void  CDrawWnd::InkerEnd()
{
	ReleaseCapture();

	if( m_InkData.GetSize() > 3 )
	{
		if( m_pHostWnd != NULL )
			m_pHostWnd->SendMessage( WM_SOURCE_READY, 0, 0 );
	}
	else
	{
		m_InkData.RemoveAll();
		ClearBmp();

	    InvalidateRect( NULL, FALSE );
		UpdateWindow();
	}
}

void CDrawWnd::CreateBmp( )
{
    CRect BmpRect;

	GetClientRect( &BmpRect );

	CPoint p = CPoint(0,0); // GetScrollPosition();
    BmpRect.OffsetRect( p );

    CRect R = m_Strk.GetBBox();
    BmpRect |= R;

    if( BmpRect.top  > 0 ) BmpRect.top  = 0;
    if( BmpRect.left > 0 ) BmpRect.left = 0;

    ResizeBmp( BmpRect );

    ClearBmp();

    SetScrollSizes( MM_TEXT, CSize(m_BmpRect.Width(), m_BmpRect.Height()) );
}

void CDrawWnd::ResizeBmp( CRect R ) 
{
    CClientDC  dc(this);

    if( R.Width() != m_BmpRect.Width() || R.Height() != m_BmpRect.Height() )
    {
    	CBitmap * pNewBmp = new CBitmap;
        pNewBmp->CreateCompatibleBitmap( &dc, R.Width(), R.Height() );

        delete m_pBmp; 
	    m_pBmp = pNewBmp;

        m_BmpRect = R;
    }
}

void CDrawWnd::ClearBmp() 
{
    CBrush brush(RGB(255, 255, 255));

	CDC * pMemDC = CreateMemDC();

    CBitmap * pOldBmp = pMemDC->SelectObject( m_pBmp );
    pMemDC->FillRect( &m_BmpRect, &brush );
    pMemDC->SelectObject(pOldBmp);

	delete pMemDC;
}

CDC * CDrawWnd::CreateMemDC() 
{
    CClientDC  dc(this);
	CDC * pMemDC = new CDC;
    pMemDC->CreateCompatibleDC( &dc );
	return pMemDC;
}

void CDrawWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

    CreateBmp();
    DrawStrk ();

	CDC * pMemDC = CreateMemDC(  );

	CBitmap* pOldBitmap = pMemDC->SelectObject(m_pBmp); 

    CPoint p( 0,0 );

    p = GetScrollPosition();

    dc.StretchBlt( m_BmpRect.left - p.x, 
                   m_BmpRect.top  - p.y, 
                   m_BmpRect.Width(),
                   m_BmpRect.Height(),
                   pMemDC, 
	               m_BmpRect.left,
                   m_BmpRect.top,
                   m_BmpRect.Width(), m_BmpRect.Height(),
                   SRCCOPY );

    pMemDC->SelectObject( pOldBitmap);

	delete pMemDC;
}

void CDrawWnd::SetBScale( CRect& Bound )
{
    CRect ZBox = m_Strk.GetZBox();

    CPoint C1 = ZBox. CenterPoint();
    CPoint C2 = Bound.CenterPoint();

    float Sc1 =  (float)Bound.Width ()/(float)ZBox.Width ();
    float Sc2 =  (float)Bound.Height()/(float)ZBox.Height();

    m_BScale  =  (Sc1 < Sc2) ? Sc1 : Sc2;
    m_BShiftX =  (float)C2.x - (float)C1.x * m_BScale;
    m_BShiftY =  (float)C2.y - (float)C1.y * m_BScale;
}

void CDrawWnd::DrawStrk()
{
    int i;

	CDC * pMemDC = CreateMemDC(  );
	CBitmap   * pOldBmp = pMemDC->SelectObject ( m_pBmp );

	if( m_fDraw & DRAW_BEZ )
	{
        CRect  Bound;
        GetClientRect( &Bound );
        Bound.InflateRect( -64, -64 );
        SetBScale( Bound );

		CPen   BPen(PS_SOLID, 1, RGB(0,0,0)   );
		CPen   APen(PS_SOLID, 2, RGB(0,0,190) );
		CPen * pOldPen = pMemDC->SelectObject ( &BPen );

		float * pP = m_Strk.m_BezPoints.data();

        CRect   ZBox = m_Strk.GetZBox();
        CPoint  C1 = ZBox. CenterPoint();
        CPoint  C2 = Bound.CenterPoint();

		for( i = 0; i < m_Strk.m_Ord; i++ )
		{
            float xf = m_BScale*pP[i+0] + m_BShiftX;
            float yf = m_BScale*pP[i+1] + m_BShiftY;

			int x = (int)(xf+0.5f);
			int y = (int)(yf+0.5f);

			if( i == 0 )
				pMemDC->MoveTo( x, y );
			else
				pMemDC->LineTo( x, y );
		}

		pMemDC->SelectObject( pOldPen );
	} //if( m_fDraw & DRAW_BEZ )

	int Dim = 2;

	CPen      SrcPen(PS_SOLID, 2, RGB(0,0,0) );
	CPen      AppPen(PS_SOLID, 2, RGB(255,0,255) );
	CPen      RsmPen(PS_SOLID, 2, RGB(128,0,0) );
	CPen      RspPen(PS_SOLID, 2, RGB(0,0,255) );
	CPen *    pOldPen;

	if( m_fDraw & DRAW_SRC )
	{
		pOldPen = pMemDC->SelectObject ( &SrcPen );

		float* pP = m_Strk.m_OrgPoints.data();

		for( i = 0; i < m_Strk.m_Sam; i++, pP += 2 )
		{
            int x = (int)(pP[0] + 0.5f);
            int y = (int)(pP[1] + 0.5f);
			
			if( i == 0 )
				pMemDC->MoveTo ( x, y );
			else
				pMemDC->LineTo ( x, y );
		}
		pMemDC->SelectObject ( pOldPen );
	} //if( m_fDraw & DRAW_SRC )

	if( m_fDraw & DRAW_RSM )
	{
		pOldPen = pMemDC->SelectObject ( &RsmPen );

		float* pP = m_Strk.m_RsmPoints.data();
		for( i = 0; i < m_Strk.m_ReSam; i++, pP += 2 )
		{
			int x = (int)(pP[0] + 0.5f);
			int y = (int)(pP[1] + 0.5f);
			pMemDC->Ellipse( x-3, y-3, x+3, y+3 );
		}
		pMemDC->SelectObject( pOldPen );
	} //if( m_fDraw & DRAW_RSM )

	if( m_fDraw & DRAW_APP )
	{
		pOldPen = pMemDC->SelectObject ( &AppPen );

		float* pP = m_Strk.m_AppPoints.data();
		for (i = 0; i < m_Strk.m_ReSam; i++, pP += 2)
		{
			int x = (int)(pP[0] + 0.5f);
			int y = (int)(pP[1] + 0.5f);

			if( i == 0 )
				pMemDC->MoveTo( x, y );
			else
				pMemDC->LineTo( x, y );
		}

		pP = m_Strk.m_AppPoints.data();
   		for( i = 0; i < m_Strk.m_ReSam; i++, pP += 2 )
		{
			int x = (int)(pP[0] + 0.5f);
			int y = (int)(pP[1] + 0.5f);

            pMemDC->SetPixel( x  , y  , RGB(0,0,0) );
            pMemDC->SetPixel( x+1, y  , RGB(0,0,0) );
            pMemDC->SetPixel( x-1, y  , RGB(0,0,0) );
            pMemDC->SetPixel( x  , y+1, RGB(0,0,0) );
            pMemDC->SetPixel( x  , y-1, RGB(0,0,0) );
        }

		pMemDC->SelectObject ( pOldPen );
	} //if( m_fDraw & DRAW_APP )

	pMemDC->SelectObject ( pOldBmp );

	delete 	pMemDC;

//  InvalidateRect(NULL, FALSE );
//	UpdateWindow();
}

BOOL CDrawWnd::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if( ChangeCursor() )
		return TRUE;
	else 
		return CWTWnd::OnSetCursor(pWnd, nHitTest, message);
}


BOOL CDrawWnd::ChangeCursor() 
{
    return TRUE;
}


void CDrawWnd::OnLButtonDown(UINT nFlags, CPoint p ) 
{
	m_PrvPnt = p;

    CWTWnd::OnLButtonDown(nFlags, p);

}

void CDrawWnd::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CWTWnd::OnLButtonUp(nFlags, point);

	m_PrvPnt = point;
}

void CDrawWnd::OnMouseMove(UINT nFlags, CPoint p ) 
{
    CWTWnd::OnMouseMove(nFlags, p);
	m_PrvPnt = p;
}

void CDrawWnd::OnRButtonDown(UINT nFlags, CPoint point) 
{
    CWTWnd::OnRButtonDown(nFlags, point);
}

BOOL CDrawWnd::OnEraseBkgnd(CDC* pDC) 
{
	return 0;
}

void CDrawWnd::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
//    RedrawWindow();
	CWTWnd::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CDrawWnd::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	
	CWTWnd::OnVScroll(nSBCode, nPos, pScrollBar);
}

