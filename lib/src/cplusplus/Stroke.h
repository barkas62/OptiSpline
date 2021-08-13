#ifndef _STROKE_HPP_INCLUDED_
#define _STROKE_HPP_INCLUDED_

#include <math.h>
#include <memory.h>
#include <vector>

typedef struct tag_RSDATA{
	double    s = 0.0;
	double    r = 0.0;
	tag_RSDATA()
	{
		s = 0.0;
		r = 0.0;
	}
	tag_RSDATA(double _r, double _s) 
	{
		r = _r;
		s = _s;
	}
} _RSDATA, * p_RSDATA;

template<int DIM> class Point
{
	double m_p[DIM];
public:
	Point(double* pPnt)
	{
		memcpy(m_p, pPnt, DIM*sizeof(double));
	}

	double operator [] (int k) { return m_p[k]; }
};

template<int DIM> class Stroke
{
public:

	double m_Resolution = 1.0;         
 
	std::vector<Point<DIM>>  m_Org;    // original points  
	std::vector<_RSDATA>     m_OrgRS;  // curve length info

public:
	int	Size() { return m_Org.size(); }
	void SetResolution(double Res) { m_Resolution = Res; }

	void  Init(int nPoints, double* pPoints)
	{
		m_Org.reserve(nPoints);
		m_OrgRS.reserve(nPoints);
		
		double* pPnt = pPoints;
		for (int i = 0; i < nPoints; i++, pPnt+=DIM)
			AddPoint(Point<DIM>(pPnt));
	}

	void AddPoint(Point<DIM> Pnt)
	{
		if (Size() == 0)
		{
			m_Org.push_back(Pnt);
			m_OrgRS.push_back(_RSDATA());
		}
		else
		{
			Point<DIM>& LastPnt = m_Org.back();

			double ds = 0.0;
			for (int k = 0; k < DIM; k++)
			{
				double d = (double)(Pnt[k] - LastPnt[k]);
				ds += d * d;
			}
			if (ds >= m_Resolution)
			{ // New point differs from the last one
				m_Org.push_back(Pnt);

				ds = sqrt(ds);
				_RSDATA& LastRS = m_OrgRS.back();
				m_OrgRS.push_back(_RSDATA(LastRS.r + ds, ds));
			}
		}
	}
};

#endif
