#if !defined(AFX_RBUTTON_H__ABE6B091_3F79_11D1_B2B0_006008332431__INCLUDED_)
#define AFX_RBUTTON_H__ABE6B091_3F79_11D1_B2B0_006008332431__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// RButton.h : header file
//
#define   ID_TIMER         8973
#define   ELAPSED_TIME     1000

/////////////////////////////////////////////////////////////////////////////
// CRButton window

class CRButton : public CButton
{
// Construction
public:
	CRButton();

	void SetID    ( UINT id ); 
	void SetTiming( UINT Timing );
// Attributes
public:
	UINT  m_Timer;
	int   m_nFired;

	UINT  m_Timing;
	UINT  m_BUttonID;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRButton)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CRButton();

	// Generated message map functions
protected:
	//{{AFX_MSG(CRButton)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RBUTTON_H__ABE6B091_3F79_11D1_B2B0_006008332431__INCLUDED_)
