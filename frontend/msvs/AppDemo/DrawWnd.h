#if !defined(AFX_DRAWWND_H__92B22681_F383_11D2_B3B5_006008332431__INCLUDED_)
#define AFX_DRAWWND_H__92B22681_F383_11D2_B3B5_006008332431__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include  "WTWnd.h"  
#include  "BStroke.h" 

#define   WM_SOURCE_READY   (WM_USER+5)
#define   WM_SOURCE_RESET   (WM_USER+6)

#define   DRAW_SRC 0x0001
#define   DRAW_APP 0x0002
#define   DRAW_RSM 0x0004
#define   DRAW_BEZ 0x0008
#define   DRAW_RSP 0x0010	  // Resampled 
#define   DRAW_ALL (DRAW_SRC | DRAW_APP | DRAW_RSM)


class CDrawWnd : public CWTWnd
{
public:

	BStroke   m_Strk;

	int       m_Ord;
	UINT      m_fDraw;
	BOOL      m_bErase;
    

    float     m_BScale;
    float     m_BShiftX;
    float     m_BShiftY;

	float     m_X;
	float     m_Y;
	float     m_Z;

	CWnd    * m_pHostWnd;
	CBitmap * m_pBmp; 

    CRect     m_BmpRect;

	CPoint    m_PrvPnt;

	CDrawWnd( CWnd * pHost );           
//	DECLARE_DYNCREATE(CDrawWnd)

	void ClearStroke();
	void SetStroke(std::vector<float>& vPoints, int nOrd, int nReSam);
	void ResetStroke(int nOrd, int nReSam);
	void AppStep();

	void          ResetSource();

	CDC         * CreateMemDC(); 

	void          CreateBmp  (); 
	void          ResizeBmp  ( CRect R ); 
	void          ClearBmp   (); 

	BOOL		  ChangeCursor();

    void          SetBScale( CRect& Bound );
	void		  DrawStrk(); 
	
	virtual void  InkerStart();
	virtual void  InkerEnd  ();

    void  WhatDraw( UINT fDraw ){ m_fDraw = fDraw; } 

	virtual ~CDrawWnd();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDrawWnd)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

	// Generated message map functions
public:
	//{{AFX_MSG(CDrawWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DRAWWND_H__92B22681_F383_11D2_B3B5_006008332431__INCLUDED_)
