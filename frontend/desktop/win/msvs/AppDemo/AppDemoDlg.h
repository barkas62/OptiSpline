// AppDemoDlg.h : header file
//

#if !defined(AFX_APPDEMODLG_H__92B22679_F383_11D2_B3B5_006008332431__INCLUDED_)
#define AFX_APPDEMODLG_H__92B22679_F383_11D2_B3B5_006008332431__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

typedef enum {
	FreeHand = 0,
	Square   = 1,
	Triangle = 2,
	Lambda   = 3
} SrcMode;

#include "DrawWnd.h"
#include "RButton.h"

class CAppDemoDlg : public CDialog
{
// Construction
public:
    int        m_Mode;
	SrcMode    m_SrcMode;

	CDrawWnd * m_pDrawWnd;

// Approx Parameters
	int         m_Dim;
	int         m_ReSam;
	int         m_Order;
  	int         m_BOrder;

	int         m_Iter;

	double		m_CfsNorm;

	UINT        m_WhatDraw;

	void		SetSource   ( SrcMode Mode );
	void		ApproxEnable( BOOL bEnable );

	void        FileNew ();
	void        FileOpen  ( CString Path );
	void        FileSaveas( CString Path );
	
	CAppDemoDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CAppDemoDlg)
	enum { IDD = IDD_APPDEMO_DIALOG };
	CRButton	m_StepBtn;
	CString	m_Source;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAppDemoDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CAppDemoDlg)
	afx_msg LONG OnSourceReady( UINT, LONG );
	afx_msg LONG OnSourceReset( UINT, LONG );
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	afx_msg void OnSelchangeSourceCombo();
	afx_msg void OnKillfocusResamEdit();
	afx_msg void OnKillfocusOrderEdit();
	afx_msg void OnStepBtn();
	afx_msg void OnResetBtn();
	afx_msg void OnClearBtn();
	afx_msg void OnDrawApproxChk();
	afx_msg void OnDrawBezierChk();
	afx_msg void OnDrawResamChk();
	afx_msg void OnDrawSourceChk();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:

	void ShowData();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_APPDEMODLG_H__92B22679_F383_11D2_B3B5_006008332431__INCLUDED_)
