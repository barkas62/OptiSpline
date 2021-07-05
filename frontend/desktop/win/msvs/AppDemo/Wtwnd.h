#ifndef WTWND_INCLUDED
#define WTWND_INCLUDED

#include "Tablet.h"

/////////////////////////////////////////////////////////////////////////////
// CWTWnd

class CWTWnd : public CScrollView
{
public:
   CWTWnd();           
   DECLARE_DYNAMIC(CWTWnd)

// Attributes
public:

   int         m_ScrW;  
   int         m_ScrH;

   BOOL        m_bInker;
   BOOL        m_bFunct;

   BOOL        m_bCollect;
   BOOL        m_bEnable;

   POINT       m_OrgPnt;
   POINT       m_OldPnt;
   DWORD       m_OldBtn;

   CPen        m_InkPen;
   CWordArray  m_InkData;

// Operations
public:
   void   SetSolidPen        ( int w, COLORREF color  );
   void   ClearInkData       ( void  );

   BOOL   IsAnyDataReady  ( ){ return m_InkData.GetSize(); };

   void   InkerOn( BOOL bInk ) // Switches inker on/off
   {
	   m_bInker = bInk;
   }

   void   FunctOn( BOOL bFunct ) // Switches "function" mode on/off
   {
	   m_bFunct = bFunct;
   }

   virtual void  InkerStart() = 0;
   virtual void  InkerEnd  () = 0;

// Overrides
	// ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CWTWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
        virtual ~CWTWnd();

protected:
     	virtual void OnDraw(CDC* pDC) = 0;    


#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
        //{{AFX_MSG(CWTWnd)
    afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // WTWND_INCLUDED
