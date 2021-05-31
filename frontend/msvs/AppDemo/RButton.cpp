// RButton.cpp : implementation file
//

#include "stdafx.h"
#include "RButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRButton

CRButton::CRButton()
{
	m_BUttonID = 0;
	m_Timing   = 1000;

	m_Timer    = 0;
	m_nFired   = 0;
}

CRButton::~CRButton()
{
}

void CRButton::SetID(UINT id) 
{
	m_BUttonID = id;
}

void CRButton::SetTiming( UINT Timing ) 
{
	m_Timing = Timing;
}

BEGIN_MESSAGE_MAP(CRButton, CButton)
	//{{AFX_MSG_MAP(CRButton)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRButton message handlers

void CRButton::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CWnd * pParent = GetParent();
	if( pParent != NULL )
	{
		m_nFired = 0;
		m_Timer = SetTimer ( ID_TIMER, m_Timing, NULL ); 
	}

	CButton::OnLButtonDown(nFlags, point);
}

void CRButton::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if( m_Timer )
	{
		KillTimer( m_Timer );
		m_Timer = 0;  
	}
	CButton::OnLButtonUp(nFlags, point);
}

void CRButton::OnTimer(UINT nIDEvent) 
{
	CWnd * pParent = GetParent();

	if( pParent != NULL )
		GetParent()->SendMessage( WM_COMMAND, m_BUttonID );
	
	if( m_nFired == 5 )
	{
		KillTimer( m_Timer );
		m_Timer = 0;
		m_Timer = SetTimer ( ID_TIMER, m_Timing / 3, NULL );
	}
	m_nFired++;

	CButton::OnTimer(nIDEvent);
}

void CRButton::OnKillFocus(CWnd* pNewWnd) 
{
	if( m_Timer )
	{
		KillTimer( ID_TIMER );
		m_Timer = 0;
	}
	CButton::OnKillFocus(pNewWnd);	
}
