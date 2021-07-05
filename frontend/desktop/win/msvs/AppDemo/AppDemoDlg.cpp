#include "stdafx.h"
#include "AppDemo.h"
#include "DrawWnd.h"
#include "AppDemoDlg.h"
#include "BStroke.h"

#include <math.h>
#include <vector>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
 
/////////////////////////////////////////////////////////////////////////////
// CAppDemoDlg dialog

CAppDemoDlg::CAppDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAppDemoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAppDemoDlg)
	m_Source = _T("");
	//}}AFX_DATA_INIT
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_pDrawWnd = NULL;

	m_SrcMode  = FreeHand;


	m_Dim      =  2;
	m_ReSam    = 32;
	m_Order    = 4;
    m_BOrder   = m_Order;

	m_Iter     = 0;

	m_CfsNorm  = -1.0f;

	m_WhatDraw = DRAW_ALL;
}

void CAppDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAppDemoDlg)
	DDX_Control(pDX, IDC_STEP_BTN, m_StepBtn);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAppDemoDlg, CDialog)
	//{{AFX_MSG_MAP(CAppDemoDlg)
	ON_MESSAGE( WM_SOURCE_READY, OnSourceReady )
	ON_MESSAGE( WM_SOURCE_RESET, OnSourceReset )
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_CBN_SELCHANGE(IDC_SOURCE_COMBO, OnSelchangeSourceCombo)
	ON_EN_KILLFOCUS(IDC_RESAM_EDIT, OnKillfocusResamEdit)
	ON_EN_KILLFOCUS(IDC_ORDER_EDIT, OnKillfocusOrderEdit)
	ON_BN_CLICKED(IDC_STEP_BTN, OnStepBtn)
	ON_BN_CLICKED(IDC_RESET_BTN, OnResetBtn)
	ON_BN_CLICKED(IDC_CLEAR_BTN, OnClearBtn)
	ON_BN_CLICKED(IDC_DRAW_APPROX_CHK, OnDrawApproxChk)
	ON_BN_CLICKED(IDC_DRAW_BEZIER_CHK, OnDrawBezierChk)
	ON_BN_CLICKED(IDC_DRAW_RESAM_CHK, OnDrawResamChk)
	ON_BN_CLICKED(IDC_DRAW_SOURCE_CHK, OnDrawSourceChk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAppDemoDlg message handlers

BOOL CAppDemoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here

	CRect  R;
	CWnd * pHost = GetDlgItem( IDC_DRAW );
	pHost->GetClientRect( &R );

	R.DeflateRect(1,1);

	m_pDrawWnd = new CDrawWnd( this );
	m_pDrawWnd->Create( NULL, NULL, WS_CHILD | WS_VISIBLE, R, pHost, 1 );


	CComboBox * pSrc = (CComboBox *)GetDlgItem( IDC_SOURCE_COMBO );
	pSrc->SetCurSel( m_SrcMode );

	SetDlgItemText( IDC_SAM_EDIT    , "n/a"   );
	SetDlgItemInt ( IDC_RESAM_EDIT  , m_ReSam );
	SetDlgItemInt ( IDC_ORDER_EDIT  , m_Order -1 );

	SetDlgItemText( IDC_ERR_EDIT  , "n/a" );
	SetDlgItemText( IDC_LAM_EDIT  , "n/a" );

	CButton * pB = (CButton *)GetDlgItem( IDC_DRAW_SOURCE_CHK );
	pB->SetCheck( m_WhatDraw & DRAW_SRC );

	pB = (CButton *)GetDlgItem( IDC_DRAW_APPROX_CHK );
	pB->SetCheck( m_WhatDraw & DRAW_APP );

	pB = (CButton *)GetDlgItem( IDC_DRAW_RESAM_CHK );
	pB->SetCheck( m_WhatDraw & DRAW_RSM );

	pB = (CButton *)GetDlgItem( IDC_DRAW_BEZIER_CHK );
	pB->SetCheck( m_WhatDraw & DRAW_BEZ );


	ApproxEnable( FALSE );

	m_StepBtn.SetID    ( IDC_STEP_BTN );
	m_StepBtn.SetTiming( 250 );

	return TRUE;  // return TRUE  unless you set the focus to a control
} 

void CAppDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CAppDemoDlg::OnPaint() 
{
	if( IsIconic() )
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width () - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

HCURSOR CAppDemoDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CAppDemoDlg::OnOK() 
{
	CDialog::OnOK();
}

BOOL CAppDemoDlg::DestroyWindow() 
{
	delete m_pDrawWnd;

	return CDialog::DestroyWindow();
}

void CAppDemoDlg::OnSelchangeSourceCombo() 
{
	CComboBox * pSrc = (CComboBox *)GetDlgItem( IDC_SOURCE_COMBO );
	SrcMode  NewMode = (SrcMode)pSrc->GetCurSel( );
	if( m_SrcMode != NewMode )
	{
		m_pDrawWnd->ResetSource(); 
		OnSourceReset(0,0);         

		SetSource( NewMode );
	}
}

LONG CAppDemoDlg::OnSourceReset(UINT wParam, LONG lParam)
{
	SetDlgItemInt(IDC_SAM_EDIT, 0);
	SetDlgItemText(IDC_STEP_BTN, "->");
	SetDlgItemText(IDC_SAM_EDIT, "n/a");

	m_pDrawWnd->ClearStroke();

	m_pDrawWnd->RedrawWindow();

	ApproxEnable(FALSE);

	m_CfsNorm = -1.0f;

	return 0L;
}

LONG CAppDemoDlg::OnSourceReady( UINT wParam, LONG lParam )
{
	if( (SrcMode)wParam == FreeHand  )	 // Take ink from DrawWnd
	{
		int Sam = m_pDrawWnd->m_InkData.GetSize() / 3; 
		
		std::vector<float> Points;
		Points.reserve(2 * Sam);

		int    idx = 0;
		float OldX = -1.0f;
		float OldY = -1.0f;

		for( int i = 0; i < Sam; i++  )
		{
			float x = (float)m_pDrawWnd->m_InkData.GetAt( idx++ );
			float y = (float)m_pDrawWnd->m_InkData.GetAt( idx++ );
			float p = (float)m_pDrawWnd->m_InkData.GetAt( idx++ );
			if( p < 0.0f || p > 255.0f )
				p = 0.0f;

			if( fabs(OldX - x) + fabs(OldY - y) <= 1 && (SrcMode)wParam == FreeHand )
				continue;

			Points.push_back(x);
			Points.push_back(y);

			OldX = x;
			OldY = y;
		}

		m_pDrawWnd->SetStroke(Points, m_Order, m_ReSam);
	}

	SetDlgItemInt( IDC_SAM_EDIT, m_pDrawWnd->m_Strk.m_Sam);
		
	m_Iter = 0;
	ApproxEnable( TRUE );

	m_Iter = 0;
	ShowData();

    m_pDrawWnd->RedrawWindow();

	return 0L;
}

void CAppDemoDlg::OnKillfocusResamEdit() 
{
	m_ReSam = GetDlgItemInt( IDC_RESAM_EDIT );
	
	if( m_ReSam < m_Order )
	{
		AfxMessageBox( "The number of approximation points may not be less than the approximation order" );
		m_ReSam = m_Order;
		SetDlgItemInt( IDC_RESAM_EDIT, m_ReSam );
	}
	
	m_pDrawWnd->ResetStroke(m_Order, m_ReSam);

	m_Iter = 0;
	ShowData();
}

void CAppDemoDlg::OnKillfocusOrderEdit() 
{
	m_Order = GetDlgItemInt( IDC_ORDER_EDIT ) + 1;

	if( m_ReSam < m_Order )
	{
		AfxMessageBox( "The number of approximation points may not be less than the approximation order" );
		m_Order = m_ReSam;
		SetDlgItemInt( IDC_ORDER_EDIT, m_Order-1 );
	}

	m_pDrawWnd->ResetStroke(m_Order, m_ReSam);

	m_Iter = 0;
	ShowData();
}

void CAppDemoDlg::SetSource( SrcMode Mode )
{
	CRect R;
	m_pDrawWnd->GetClientRect( &R );

	int W = R.Width();
	int L = R.Height();

	float D = (W>L ? L : W) / 4.0f;

	std::vector<float> vSquare = {
		  D,   D,
		3*D,   D,
		3*D, 3*D,
		  D, 3*D,
		  D,   D
	};

	std::vector<float> vTriangle = {
		2*D,   D,
		3*D, 3*D,
		  D, 3*D,
		2*D,   D
	};

	std::vector<float> vLambda = {
		  D/2, 3*D,
		3*D/2, 3*D,
		2*D,     D,
		5*D/2, 3*D,
		7*D/2, 3*D
	};

	switch( Mode )
	{
		case FreeHand:
			m_pDrawWnd->InkerOn( TRUE );
		break;

		case Square:
			m_pDrawWnd->InkerOn( FALSE );
			m_pDrawWnd->SetStroke(vSquare, m_Order, m_ReSam);
			OnSourceReady( (UINT)Mode, 0 );
		break;

		case Triangle:
			m_pDrawWnd->InkerOn( FALSE );
			m_pDrawWnd->SetStroke(vTriangle, m_Order, m_ReSam);
			OnSourceReady( (UINT)Mode, 0 );
		break;

		case Lambda:
			m_pDrawWnd->InkerOn( FALSE );
			m_pDrawWnd->SetStroke(vLambda, m_Order, m_ReSam);;
			OnSourceReady( (UINT)Mode, 0 );
		break;
	}

	m_SrcMode = Mode;
}

void CAppDemoDlg::ApproxEnable( BOOL bEnable )
{
	CWnd * pS = GetDlgItem( IDC_STEP_BTN );
	CWnd * pR = GetDlgItem( IDC_RESET_BTN );
	if( bEnable )
	{
		pS->ModifyStyle( WS_DISABLED, 0 );
		pR->ModifyStyle( WS_DISABLED, 0 );
	}
	else
	{
		pS->ModifyStyle( 0, WS_DISABLED );
		pR->ModifyStyle( 0, WS_DISABLED );
	}

	pS->RedrawWindow();
	pR->RedrawWindow();
}

void CAppDemoDlg::OnStepBtn() 
{
	m_pDrawWnd->AppStep();

	//m_pStrk->FindBPoints( m_Order, m_pBasis->m_pNorms );

	m_pDrawWnd->RedrawWindow();
	m_Iter++;

	ShowData();
}

void CAppDemoDlg::ShowData()
{
	SetDlgItemInt(IDC_RESAM_EDIT, m_ReSam);
	SetDlgItemInt(IDC_ORDER_EDIT, m_Order - 1);

	CString S;
	S.Format("%5.2f", m_pDrawWnd->m_Strk.m_Err);
	SetDlgItemText(IDC_ERR_EDIT, S);
	S.Format("%5.3f", m_pDrawWnd->m_Strk.m_Lam);
	SetDlgItemText(IDC_LAM_EDIT, S);
	S.Format("->%3d", m_Iter);
	SetDlgItemText(IDC_STEP_BTN, S);
}

void CAppDemoDlg::OnResetBtn() 
{
	m_pDrawWnd->ResetStroke(m_Order, m_ReSam);
	m_Iter = 0;
	ShowData();
}

void CAppDemoDlg::OnClearBtn() 
{
	OnSourceReset(0,0);
}

void CAppDemoDlg::OnDrawApproxChk() 
{
	m_WhatDraw ^= DRAW_APP;

    m_pDrawWnd->WhatDraw( m_WhatDraw );
	m_pDrawWnd->RedrawWindow();
}

void CAppDemoDlg::OnDrawBezierChk() 
{
	m_WhatDraw ^= DRAW_BEZ;

    m_pDrawWnd->WhatDraw( m_WhatDraw );
	m_pDrawWnd->RedrawWindow();
}

void CAppDemoDlg::OnDrawResamChk() 
{
	m_WhatDraw ^= DRAW_RSM;

    m_pDrawWnd->WhatDraw( m_WhatDraw );
	m_pDrawWnd->RedrawWindow();
}

void CAppDemoDlg::OnDrawSourceChk() 
{
	m_WhatDraw ^= DRAW_SRC;

    m_pDrawWnd->WhatDraw( m_WhatDraw );
	m_pDrawWnd->RedrawWindow();
}

void	CAppDemoDlg::FileNew()
{
	OnClearBtn();
}

void	CAppDemoDlg::FileSaveas( CString Path )
{
	CStdioFile * pF = pF = new CStdioFile( Path, CFile::modeCreate | CFile::modeWrite | CFile::typeText  );

	CString S;
	/*
//	S.Format( "<Order>\r\n" );
	pF->WriteString( "<Order>\r\n" );
	S.Format( "%d\r\n", m_Order-1 );
	pF->WriteString( S );

	pF->WriteString( "<ReSam>\r\n" );
	S.Format( "%d\r\n", m_ReSam );
	pF->WriteString( S );

	pF->WriteString( "<Dim>\r\n" );
	S.Format( "%d\r\n", m_Dim );
	pF->WriteString( S );

	BStroke& Strk = m_pDrawWnd->m_Strk;

	pF->WriteString( "<Stroke>\r\n" );
	S.Format( "%d\r\n", Strk.m_Sam );
	pF->WriteString( S );

	for( int i = 0; i < Strk.m_Sam; i++ )
	{
		float  x, y, z;

		if( m_Dim == 3 )
		{
			Strk.GetAt( i, &x, &y, &z );
			S.Format( "%5.1f ,  %5.1f ,  %5.1f\r\n", x, y, z );
		}
		else
		{
			m_pStrk->GetAt( i, &x, &y );
			S.Format( "%5.1f ,  %5.1f\r\n", x, y );
		}
		pF->WriteString( S ); // Point
	}
	*/
	pF->Close();
	delete pF;
}

void	CAppDemoDlg::FileOpen( CString Path )
{
	CStdioFile * pF = new CStdioFile( Path, CFile::modeRead | CFile::typeText  );

	FileNew();
	/*
	CString S;

	pF->ReadString( S );  // Order
	pF->ReadString( S );  // Order
	m_Order = atoi( S ) + 1;

	pF->ReadString( S );  // ReSam
	pF->ReadString( S );  // ReSam
	m_ReSam = atoi( S );

	pF->ReadString( S );  // Dim
	pF->ReadString( S );  // Dim
	m_Dim = atoi( S );

	if( pF->ReadString( S ) )
	{
		pF->ReadString( S );  // Sam
		int Sam = atoi( S );

		m_pStrk = new BStroke( m_Dim, Sam );

		for( int i = 0; i < Sam; i++ )
		{
			int        Idx;
			float  x, y, z;
			CString   Part;

			pF->ReadString( S  );  // Point

			Idx  = S.Find( ',' );
			Part = S.Left( Idx );
			x = (float)atof( Part );
			S    = S.Right( S.GetLength() - Idx - 1 );

			if( m_Dim == 3 )
			{
				Idx  = S.Find( ',' );
				Part = S.Left( Idx );
				y = (float)atof( Part );
				S    = S.Right( S.GetLength() - Idx - 1 );

				z = (float)atof( S );
				m_pStrk->Add( x, y, z );
			}
			else
			{
				y = (float)atof( S );
				m_pStrk->Add( x, y );
			}
		}
		
	}
	*/
	pF->Close();
	delete pF;

	//OnSourceReady( -1,0 );
}


