// StrokeAttr.h: interface for the StrokeAttr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STROKEATTR_H__5E704681_C995_11D1_B2F6_006008332431__INCLUDED_)
#define AFX_STROKEATTR_H__5E704681_C995_11D1_B2F6_006008332431__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "Stroke.h"
#include "AppStroke.h"

class StrokeAttr  
{
public:
	int        m_Dim;
	int        m_Ord;
	int        m_ReSam;

	float    * m_pCfs;
	float    * m_pShift;
	float      m_Norm;

	unsigned 
		long   m_Color;

	unsigned 
		long   m_Width;

public:
	StrokeAttr();
	StrokeAttr( int Dim  );
	StrokeAttr( StrokeAttr& AD );

	StrokeAttr( AppStroke&  AS, int Ord );

	bool   InitAttr( int ReSam );

	void   SetColor( unsigned long Color ){	m_Color = Color; }
	void   SetWidth( unsigned long Width ){	m_Width = Width; }

	unsigned long  GetColor( ){	return m_Color; }
	unsigned long  GetWidth( ){	return m_Width; }

	virtual ~StrokeAttr();
};

#endif // !defined(AFX_STROKEATTR_H__5E704681_C995_11D1_B2F6_006008332431__INCLUDED_)
