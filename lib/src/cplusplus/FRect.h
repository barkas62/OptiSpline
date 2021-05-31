#ifndef FRECT_INCLUDED
#define FRECT_INCLUDED

//void ZoomRect( CRect * pR, CPoint P, float Sc );

class FRect  
{

public:
	float top, bottom, left, right;

public:

// Constructors
	FRect(){}

	FRect( int   l, int   t, int   r, int   b ) :
		top ((float)t), bottom((float)b), 
		left((float)l), right ((float)r){}

	FRect( float l, float t, float r, float b) :
		top (t), bottom(b), 
		left(l), right (r) {}

	FRect( const FRect& R ) :
		top (R.top ), bottom(R.bottom), 
		left(R.left), right (R.right ) {}

	FRect( const RECT& R ) :
		top ((float)(R.top )), bottom((float)(R.bottom)), 
		left((float)(R.left)), right ((float)(R.right )) {}

	FRect( LPCRECT pR ) :
		top ((float)(pR->top )), bottom((float)(pR->bottom)), 
		left((float)(pR->left)), right ((float)(pR->right )) {} 

		FRect( POINT TL, POINT BR ) :
		top ((float)(TL.y)), bottom((float)(BR.y)), 
		left((float)(TL.x)), right ((float)(BR.x)) {}

// Attributes (in addition to RECT members)
	float   Width (){ return (right  - left); }
	float   Height(){ return (bottom -  top); } 

	CPoint&  TopLeft    ( )
	{
		CPoint * p = new CPoint( (int)left, (int)top );
		return *p;
	}

	CPoint  BottomRight()
	{
		CPoint * p = new CPoint( (int)right, (int)bottom );
		return *p;
	}

	CPoint CenterPoint()
	{
		CPoint p(0,0);
		p.x = (int)((left + right ) * 0.5f);
		p.y = (int)((top  + bottom) * 0.5f);
		return p;
	}

	BOOL IsRectEmpty()
	{ 
		if( right <= left || bottom <= top )
			return TRUE;
		else
			return FALSE;
	} const

	BOOL IsRectNull ()
	{ 
		if( right == 0.0f && left == 0.0f && top == 0.0f && bottom == 0.0 )
			return TRUE;
		else
			return FALSE;
	}

	BOOL PtInRect( POINT p )
	{
		if( (float)(p.x) <= right && (float)(p.x) >= left &&
			(float)(p.y) >= top   && (float)(p.y) <= bottom )
			return TRUE;
		else
			return FALSE;
	}

// Operations
	void SetRect( float x1, float y1, float x2, float y2 )
	{
		left  = x1; top    = y1; 
		right = x2; bottom = y2; 
	}

	void SetRect(int x1, int y1, int x2, int y2)
	{
		 SetRect( (float)x1, (float)y1, (float)x2, (float)y2 );
	}

	void SetRect( POINT TL, POINT BR )
	{
		 SetRect( (float)(TL.x), (float)(TL.y), (float)(BR.x), (float)(BR.y) );
	}

	void SetRect( CRect& R )
	{
		 SetRect( R.left, R.top, R.right, R.bottom );
	}

	void SetRectEmpty(){ top = bottom = left = right = 0.0f;}

	void CopyRect( LPCRECT pR )
	{
		top  = (float)pR->top ; bottom = (float)pR->bottom;
		left = (float)pR->left; right  = (float)pR->right ;
	}

	void NormalizeRect()
	{
		float Tmp;								 
		if( left > right )
		{
			Tmp   = left;
			left  = right;
			right = Tmp;
		}
		if( top > bottom )
		{
			Tmp    = top;
			top    = bottom;
			bottom = Tmp;
		}			
	}

	void InflateRect( LPCRECT lpRect )
	{
		left   -= lpRect->left;
		top    -= lpRect->top;
		right  += lpRect->right;
		bottom += lpRect->bottom;
	}

	void InflateRect( float l, float t, float r, float b )
	{
		left   -= l;
		top    -= t;
		right  += r;
		bottom += b;
	}

	void InflateRect( int l, int t, int r, int b )
	{
		 InflateRect( (float)l, (float)t, (float)r, (float)b );
	}

	void InflateRect( float dx, float dy )
	{
		 InflateRect( dx, dy, dx, dy );
	}	 

	void InflateRect( int dx, int dy )
	{
		 InflateRect( (float)dx, (float)dy );
	}

	void DeflateRect( float l, float t, float r, float b )
	{
		left   += l; top    += t;
		right  -= r; bottom -= b;
	}

	void DeflateRect( int l, int t, int r, int b )
	{
		 DeflateRect( (float)l, (float)t, (float)r, (float)b );
	}

	void DeflateRect( float dx, float dy )
	{
		 DeflateRect( dx, dy, dx, dy );
	}

	void DeflateRect( int dx, int dy )
	{
		 DeflateRect( (float)dx, (float)dy );
	}

	void OffsetRect( float x, float y )
	{
		left += x; right  += x;
		top  += y; bottom += y;
	}

	void OffsetRect( int   x, int    y )
	{
		OffsetRect( (float)x, (float)y );
	}

	void OffsetRect( POINT  p )
	{
		left += p.x; right  += p.x;
		top  += p.y; bottom += p.y;
	}

	void  Zoom( CPoint& P, float Sc )
	{
		left   = ((left   - (float)(P.x)) * Sc) + (float)(P.x);
		right  = ((right  - (float)(P.x)) * Sc) + (float)(P.x);

		top    = ((top    - (float)(P.y)) * Sc) + (float)(P.y);
		bottom = ((bottom - (float)(P.y)) * Sc) + (float)(P.y);
	}

	void  Zoom( float x, float y, float Sc )
	{
		left   = (left   - x)*Sc + x;
		right  = (right  - x)*Sc + x;

		top    = (top    - y) * Sc + y;
		bottom = (bottom - y) * Sc + y;
	}
// Additional Operations
	void operator =( const FRect& R )
	{
		left = R.left; right  = R.right;
		top  = R.top ; bottom = R.bottom;
	}

	void operator =( const RECT& R )
	{
		left = (float)R.left; right  = (float)R.right;
		top  = (float)R.top ; bottom = (float)R.bottom;
	}

	BOOL operator==(const FRect& R )	const
	{
		if( left == R.left && right  == R.right &&
			top  == R.top  && bottom == R.bottom )
			return TRUE;
		else
			return FALSE;
	} 

	BOOL operator==(const RECT& R )	const
	{
		if( left == (float)R.left && right  == (float)R.right &&
			top  == (float)R.top  && bottom == (float)R.bottom )
			return TRUE;
		else
			return FALSE;
	} 

	BOOL operator!=(const FRect& R) const
	{
		if( left != R.left || right  != R.right ||
			top  != R.top  || bottom != R.bottom )
			return TRUE;
		else
			return FALSE;
	} 
	BOOL operator!=(const RECT& R) const
	{
		if( left != (float)R.left || right  != (float)R.right ||
			top  != (float)R.top  || bottom != (float)R.bottom )
			return TRUE;
		else
			return FALSE;
	} 

	operator CRect()
	{
		CRect  R( (int)left, (int)top, (int)right, (int)bottom );
		return R;
	}

	void operator|=(const FRect& R)
	{
		if( R.left   < left   || left   == 0.0f ) left   = R.left  ;
		if( R.right  > right  || right  == 0.0f ) right  = R.right ;
		if( R.top    < top    || top    == 0.0f ) top    = R.top   ;
		if( R.bottom > bottom || bottom == 0.0f ) bottom = R.bottom;
	}

	void operator|=(const RECT& R)
	{
		if( (float)(R.left  ) < left   || left   == 0.0f ) left   = (float)(R.left  );
		if( (float)(R.right ) > right  || right  == 0.0f ) right  = (float)(R.right );
		if( (float)(R.top   ) < top    || top    == 0.0f ) top    = (float)(R.top   );
		if( (float)(R.bottom) > bottom || bottom == 0.0f ) bottom = (float)(R.bottom);
	}

	void operator+=(POINT point);
	void operator-=(POINT point);
	void operator-=(LPCRECT lpRect);
	void operator&=(const RECT& rect);

// Operators returning CRect values
	CRect operator+(POINT point) const;
	CRect operator-(POINT point) const;
	CRect operator+(LPCRECT lpRect) const;
	CRect operator+(SIZE size) const;
	CRect operator-(SIZE size) const;
	CRect operator-(LPCRECT lpRect) const;
	CRect operator&(const RECT& rect2) const;
	CRect operator|(const RECT& rect2) const;

};
#endif// FRECT_INCLUDED

/////////////////////////////////////////////////////////////////////////////
