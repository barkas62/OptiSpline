// AppDemo.h : main header file for the APPDEMO application
//

#if !defined(AFX_APPDEMO_H__92B22677_F383_11D2_B3B5_006008332431__INCLUDED_)
#define AFX_APPDEMO_H__92B22677_F383_11D2_B3B5_006008332431__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "AppDemoDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CAppDemoApp:
// See AppDemo.cpp for the implementation of this class
//

class CAppDemoApp : public CWinApp
{
public:
	CString       m_Path;
	CAppDemoDlg * m_pDlg;

	CAppDemoApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAppDemoApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CAppDemoApp)
	afx_msg void OnFileNew();
	afx_msg void OnFileOpen();
	afx_msg void OnFileSave();
	afx_msg void OnFileSaveas();
	afx_msg void OnExit();
	afx_msg void OnHelpAbout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_APPDEMO_H__92B22677_F383_11D2_B3B5_006008332431__INCLUDED_)
