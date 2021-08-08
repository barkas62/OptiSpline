#ifndef _STROKE_H_INCLUDED_
#define _STROKE_H_INCLUDED_

typedef struct {
	float    s;
	float    r;
} _RSDATA, * p_RSDATA;

typedef struct   
{
	int        m_Dim;     // Number of components (dimensions)
	int        m_Sam;     // actual number of points
	int        m_MaxSam;  // capacity 
	float    * m_pOrg;    // points buffer

	p_RSDATA   m_pOrgRS;  // curve length info
} Stroke, * pStroke;

#endif 