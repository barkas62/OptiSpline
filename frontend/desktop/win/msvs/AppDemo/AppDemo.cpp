// AppDemo.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "AppDemo.h"
#include "AppDemoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAppDemoApp

BEGIN_MESSAGE_MAP(CAppDemoApp, CWinApp)
	//{{AFX_MSG_MAP(CAppDemoApp)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_COMMAND(ID_FILE_SAVEAS, OnFileSaveas)
	ON_COMMAND(ID_EXIT, OnExit)
	ON_COMMAND(ID_HELP_ABOUT, OnHelpAbout)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAppDemoApp construction

CAppDemoApp::CAppDemoApp()
{
	m_Path.Empty();
	m_pDlg = 0;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CAppDemoApp object

CAppDemoApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CAppDemoApp initialization

BOOL CAppDemoApp::InitInstance()
{
	// Standard initialization

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
//	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	m_pDlg = new CAppDemoDlg;
//	CAppDemoDlg   dlg;
	m_pMainWnd = m_pDlg;
	int nResponse = m_pDlg->DoModal();
	if (nResponse == IDOK)
	{
	}
	else if (nResponse == IDCANCEL)
	{
	}

	delete m_pDlg;
	m_pDlg = 0;

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

void CAppDemoApp::OnFileNew() 
{
	m_Path.Empty();
	if( m_pDlg )
		m_pDlg->FileNew();
}

void CAppDemoApp::OnFileOpen() 
{
  CFileDialog *pDlg ;
  CString      szFilter;
  CString      S, Ext;

  if( m_pDlg == 0 )
	  return;

  szFilter = CString("Animation Files (*.apr)|*.apr|")  +
             CString("All Files(*.*)|*.*||");

  pDlg =  new CFileDialog( TRUE, "apr", NULL, OFN_FILEMUSTEXIST, LPCSTR(szFilter), NULL ) ;
  if( pDlg == NULL ) 
      return;

  if( pDlg->DoModal() == IDOK )
  {
      m_Path = pDlg->GetPathName();
      m_pDlg->FileOpen( m_Path );
  }

  delete pDlg;
}

void CAppDemoApp::OnFileSave() 
{
    if( m_pDlg == 0 )
	    return;

	if( m_Path.IsEmpty() )
		OnFileSaveas();
	else
	    m_pDlg->FileSaveas( m_Path );
}

void CAppDemoApp::OnFileSaveas() 
{
  CFileDialog *pDlg ;
  CString      szFilter;
  CString      S, Ext;

  if( m_pDlg == 0 )
	  return;

  szFilter = CString("Animation Files (*.apr)|*.apr|")  +
             CString("All Files(*.*)|*.*||");

  pDlg =  new CFileDialog( FALSE, "apr", NULL, OFN_FILEMUSTEXIST, LPCSTR(szFilter), NULL ) ;
  if ( pDlg == NULL ) 
    return;

  if( pDlg->DoModal() == IDOK )
  {
     m_Path = pDlg->GetPathName();
     m_pDlg->FileSaveas( m_Path );
  }

  delete pDlg;
}

void CAppDemoApp::OnExit() 
{
  if( m_pDlg == 0 )
	  return;

  m_pDlg->OnOK();
}

void CAppDemoApp::OnHelpAbout() 
{
  if( m_pDlg != 0 )
	m_pDlg->OnSysCommand(IDM_ABOUTBOX, 0) ;

}
