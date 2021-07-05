// BStroke.h: interface for the BStroke class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BSTROKE_H__711BD0F2_F9AC_11D2_B3BA_006008332431__INCLUDED_)
#define AFX_BSTROKE_H__711BD0F2_F9AC_11D2_B3BA_006008332431__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <vector>


class BStroke  
{
public:
	BStroke();
	BStroke( const BStroke& pStrk );

    void Init(std::vector<float>& Points, int nOrd, int nReSam);
    void Reset(int nOrd, int nReSam);
    bool AppStep();
    bool GetAppResults();

    std::vector<float> m_OrgPoints;
    std::vector<float> m_AppPoints;
    std::vector<float> m_RsmPoints;
    std::vector<float> m_BezPoints;

    int m_Sam   = 0;
    int m_ReSam = 0;
    int m_Ord   = 0;

    float m_Err = 0.0f;
    float m_Lam = 1.0f;

    CRect    m_BBox;
    CRect    m_ZBox;

    void* m_pOptiSpline = 0;

    CRect   GetBBox(bool bBezier = true);
    CRect   GetZBox();

    void    UpdateBBox();
    void    UpdateZBox();

    virtual ~BStroke();

    void Clear();

	bool	SetOptiSpline(int nOrd, int ReSam);
/*
    float   GetADist2( int Idx, float x, float y );
    float   GetADistR( float * pP, int Idx, float x, float y );

    bool    GetBPoint( int Idx, float *pX0, float *pY0 );
	float	FindBPoints     ( int Ord, float * pNorms );

    void    UpdateBCurve();
    void    Casteljau   ( float T, float * pX0, float * pY0 );

    void    ChangeBPoint( int   Idx  , float dx, float dy, BOOL bUpdate = TRUE );

*/

};

#endif // !defined(AFX_BSTROKE_H__711BD0F2_F9AC_11D2_B3BA_006008332431__INCLUDED_)
