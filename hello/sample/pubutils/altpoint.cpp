/********************************************************************/
/*  Licensed Materials - Property of IBM                            */
/*                                                                  */
/*                                                                  */
/* Copyright (C) International Business Machines Corp., 1994.       */
/* Copyright (C) Apple Computer, Inc., 1994                         */
/*                                                                  */
/*  US Government Users Restricted Rights -                         */
/*  Use, duplication, or disclosure restricted                      */
/*  by GSA ADP Schedule Contract with IBM Corp.                     */
/*                                                                  */
/*  File:    AltPoint.cpp                                           */
/*                                                                  */
/*  Contains:  Alternate ODPoint, ODRect (C++ savvy)                */
/*                                                                  */
/*  Change History                                                  */
/*    <1>  141114 10/27/95 pfe Added ODToolSpaceRect class          */
/*                                                                  */
/********************************************************************/

#define INCL_GPI
#include <os2.h>

#ifndef _ALTPOINT_
#include "AltPoint.h"
#endif

#ifndef _ODTYPES_
#include "ODTypes.h"
#endif

#ifndef _EXCEPT_
#include "Except.h"
#endif

#ifndef _ODDEBUG_
#include "ODDebug.h"
#endif

#ifndef _LINEOPS_
#include "LineOps.h"    /* For kFixedEpsilon */
#endif

#ifndef _ODMATH_
#include "ODMath.h"
#endif

//==============================================================================
// ODPoint
//==============================================================================


ODPoint::ODPoint( POINTL ptl)
{
  x = ODIntToFixed(ptl.x);
  y = ODIntToFixed(ptl.y);
}


ODPoint::ODPoint( const ODPoint &pt )
{
  x = pt.x;
  y = pt.y;
}


ODPoint& ODPoint::operator= ( const ODPoint &pt )
{
  x = pt.x;
  y = pt.y;
  return *this;
}


ODPoint& ODPoint::operator= ( const POINTL& ptl)
{
  x = ODIntToFixed(ptl.x);
  y = ODIntToFixed(ptl.y);
  return *this;
}


void ODPoint::Offset ( ODCoordinate xx, ODCoordinate yy )
{
  x += xx;
  y += yy;
}


void ODPoint::operator+= ( const ODPoint &pt )
{
  x += pt.x;
  y += pt.y;
}


void ODPoint::operator-= ( const ODPoint &pt )
{
  x -= pt.x;
  y -= pt.y;
}


void ODPoint::operator+=( const    POINTL& ptl)
{
  x += ODIntToFixed(ptl.x);
  y += ODIntToFixed(ptl.y);
}

void ODPoint::operator-=( const    POINTL& ptl)
{
  x -= ODIntToFixed(ptl.x);
  y -= ODIntToFixed(ptl.y);
}

POINTL ODPoint::AsPOINTL( ) const
{
  POINTL ptl;
  ptl.x = ODFixedRound(x);
  ptl.y = ODFixedRound(y);
  return ptl;
}


ODSShort
ODPoint::IntX( ) const
{
  return ODFixedRound(x);
}


ODSShort
ODPoint::IntY( ) const
{
  return ODFixedRound(y);
}


ODBoolean ODPoint::operator==( const ODPoint &pt ) const
{
  return (ODBoolean)(x==pt.x && y==pt.y);
}


ODBoolean ODPoint::operator!=( const ODPoint &pt ) const
{
  return (ODBoolean)(x!=pt.x || y!=pt.y);
}


ODBoolean
ODPoint::ApproxEquals( const ODPoint& pt ) const
{
  ODFixed delta;
  delta = x-pt.x;
  if( delta>kFixedEpsilon || delta<-kFixedEpsilon )
    return kODFalse;
  delta = y-pt.y;
  if( delta>kFixedEpsilon || delta<-kFixedEpsilon )
    return kODFalse;
  return kODTrue;
}


//==============================================================================
// ODRect
//==============================================================================


#define ASSERTVALID()    WASSERT(right>=left && top>=bottom)


//------------------------------------------------------------------------------
// ::ODRect( corner1, corner2 )
//
// Construct a rectangle given two opposite corners (not necessarily topLeft,botRight)
//------------------------------------------------------------------------------

ODRect::ODRect( const ODPoint &a, const ODPoint &b )
{
  if( a.x<b.x ) {
    left = a.x;
    right= b.x;
  } else {
    left = b.x;
    right= a.x;
  }
  if( a.y>b.y) {
    top   = a.y;
    bottom= b.y;
  } else {
    top   = b.y;
    bottom= a.y;
  }
}


//------------------------------------------------------------------------------
// ::ODRect( topLeft, width, height )
//
// Construct a rectangle given its origin, width and height.
//------------------------------------------------------------------------------

ODRect::ODRect( const ODPoint &bottomLeft, ODCoordinate width, ODCoordinate height )
{
  WASSERT(width>=0);
  WASSERT(height>=0);

  left = bottomLeft.x;    right = bottomLeft.x + width;
  bottom = bottomLeft.y;  top = bottomLeft.y + height;
}

//------------------------------------------------------------------------------
// ::ODRect( Rect )
//
// Construct an ODRect from a GPI RECTL
//------------------------------------------------------------------------------

ODRect::ODRect( const RECTL &r )
{
  left = ODIntToFixed(r.xLeft);
  right = ODIntToFixed(r.xRight);
  top  = ODIntToFixed(r.yTop);
  bottom = ODIntToFixed(r.yBottom);
}


//------------------------------------------------------------------------------
// ::= Rect
//
// Set an ODRect from a GPI RECTL
//------------------------------------------------------------------------------

ODRect&
ODRect:: operator= ( const Rect &r )
{
  left = ODIntToFixed(r.xLeft);
  right = ODIntToFixed(r.xRight);
  top  = ODIntToFixed(r.yTop);
  bottom = ODIntToFixed(r.yBottom);
  ASSERTVALID();
  return *this;
}


//------------------------------------------------------------------------------
// ::Set
//
// Set all four coordinates of a rectangle. Warn if the resulting rect is bogus.
//------------------------------------------------------------------------------

void
ODRect::Set( ODCoordinate l, ODCoordinate t, ODCoordinate r, ODCoordinate b )
{
  left = l;        right = r;
  top  = t;        bottom= b;
  ASSERTVALID();
}


//------------------------------------------------------------------------------
// ::Set
//
// Set all four coordinates of a rectangle. Warn if the resulting rect is bogus.
//------------------------------------------------------------------------------

void
ODRect::Set( const ODPoint &origin, ODCoordinate width, ODCoordinate height )
{
  WASSERT(width>=0);
  WASSERT(height>=0);
  left = origin.x;        right = left+width;
  bottom = origin.y;      top = bottom +height;
}


//------------------------------------------------------------------------------
// ::SetInt
//
// Set coordinates of a rectangle from integers. Warn if the resulting rect is bogus.
//------------------------------------------------------------------------------

void
ODRect::SetInt( short l, short t, short r, short b )
{
  left = ODIntToFixed(l);
  right = ODIntToFixed(r);
  top  = ODIntToFixed(t);
  bottom= ODIntToFixed(b);
  ASSERTVALID();
}


//------------------------------------------------------------------------------
// ::Offset
//
// Move a rectangle.
//------------------------------------------------------------------------------

void
ODRect::Offset( ODCoordinate x, ODCoordinate y )
{
  left += x;        right += x;
  top  += y;        bottom+= y;
}

void
ODRect::Offset( const ODPoint &pt )
{
  left += pt.x;      right += pt.x;
  top  += pt.y;      bottom+= pt.y;
}


//------------------------------------------------------------------------------
// ::Inset
//
// Inset the edges of a rectangle, without collapsing to negative dimensions.
//------------------------------------------------------------------------------

void
ODRect::Inset( ODCoordinate x, ODCoordinate y )
{
  left += x;
  right = Max(left,right-x);
  bottom += y;
  top = Max(bottom, top-y);
}


//------------------------------------------------------------------------------
// ::Clear
//
// Zero all coordinates
//------------------------------------------------------------------------------

void
ODRect::Clear( )
{
  left = right = top = bottom = 0;
}


//------------------------------------------------------------------------------
// :: &=
//
// Intersect me with another rectangle (result stored in me)
//------------------------------------------------------------------------------

void
ODRect:: operator&= ( const ODRect &r )
{
  left = Max(left,r.left);  right = Min(right, r.right);
  top  = Min(top, r.top);    bottom= Max(bottom,r.bottom);
  if( this->IsEmpty() )
    this->Clear();
}


//------------------------------------------------------------------------------
// :: |=
//
// Union me with another rectangle (result stored in me)
//------------------------------------------------------------------------------

void
ODRect:: operator|= ( const ODRect &r )
{
  if( this->IsEmpty() )
    *this = r;
  else if( !r.IsEmpty() ) {
    left = Min(left,r.left);
    right= Max(right,r.right);
    top  = Max(top,r.top);
    bottom=Min(bottom,r.bottom);
  }
}


//------------------------------------------------------------------------------
// :: |= ODPoint
//
// Union me with a point (expand to fit point)
//------------------------------------------------------------------------------

void
ODRect:: operator|= ( const ODPoint &pt )
{
  left = Min(left,pt.x);
  right= Max(right,pt.x);
  top  = Max(top,pt.y);
  bottom=Min(bottom,pt.y);
}

//------------------------------------------------------------------------------
// :: AsRECTL
//
// Turn me into a GPI RECTL
//------------------------------------------------------------------------------
void ODRect::AsRECTL( RECTL& r) const
{
  r.xLeft = ODFixedRound(left);
  r.xRight = ODFixedRound(right);
  r.yTop = ODFixedRound(top);
  r.yBottom = ODFixedRound(bottom);
}

//------------------------------------------------------------------------------
// :: IsEmpty
//
// Do I have no area?
//------------------------------------------------------------------------------

ODBoolean ODRect::IsEmpty( ) const
{
  return (ODBoolean)(right<=left || top<=bottom);
}


//------------------------------------------------------------------------------
// :: Contains( Point )
//
// Do I contain a point? (Remember, I don't contain my right and top edges.)
//------------------------------------------------------------------------------

ODBoolean ODRect::Contains( const ODPoint &pt ) const
{
  return (ODBoolean)(left<=pt.x && pt.x<right
      && bottom <=pt.y && pt.y<top);
}


//------------------------------------------------------------------------------
// :: Contains( Rect )
//
// Do I contain an entire rect?
//------------------------------------------------------------------------------

ODBoolean ODRect::Contains( const ODRect &r ) const
{
  if( r.right<=r.left || r.bottom>=r.top )
    return kODTrue;          // Empty rect contained in anything
  else
    return (ODBoolean)(left<=r.left && r.right<=right
        && top >=r.top  && r.bottom>=bottom);
}

//------------------------------------------------------------------------------
// :: ApproxContains( Rect )
//
// Do I approximately (within epsilon) contain an entire rect?
//------------------------------------------------------------------------------

ODBoolean
ODRect::ApproxContains( const ODRect &r ) const
{
  if( r.right<=r.left || r.bottom<=r.top )
    return kODTrue;          // Empty rect contained in anything
  else
    return left-kFixedEpsilon<=r.left && r.right<=right+kFixedEpsilon
        && bottom-kFixedEpsilon <=r.bottom  && r.top<=top+kFixedEpsilon;
}


//------------------------------------------------------------------------------
// :: ==
//
// Am I equal to another rect?
//------------------------------------------------------------------------------

ODBoolean ODRect::operator==( const ODRect &r ) const
{
  return (ODBoolean)(memcmp(this,&r,sizeof(ODRect)) == 0);
}

//------------------------------------------------------------------------------
// :: ApproxEquals
//
// Am I approximately equal (within epsilon) to another rect?
//------------------------------------------------------------------------------

ODBoolean
ODRect::ApproxEquals( const ODRect &r ) const
{
  return left-r.left<=kFixedEpsilon     && left-r.left>=-kFixedEpsilon
    && right-r.right<=kFixedEpsilon   && right-r.right>=-kFixedEpsilon
    && top-r.top<=kFixedEpsilon       && top-r.top>=-kFixedEpsilon
    && bottom-r.bottom<=kFixedEpsilon && bottom-r.bottom>=-kFixedEpsilon;
}

//------------------------------------------------------------------------------
// :: Intersects
//
// Do I intersect another rectangle?
// (Remember, I don't contain my right and top edges.)
//------------------------------------------------------------------------------

ODBoolean ODRect::Intersects( const ODRect &r ) const
{
  return (ODBoolean)(Max(left,r.left) < Min(right,r.right)
    && Min(top,r.top) > Max(bottom,r.bottom));
}

//start [141114] pfe -- ODToolSpaceRect addition
//==============================================================================
// ODToolSpaceRect
//==============================================================================

//------------------------------------------------------------------------------
// ODToolSpaceRect::ODToolSpaceRect
//------------------------------------------------------------------------------

ODToolSpaceRect::ODToolSpaceRect(ODCoordinate l, ODCoordinate t,
                                 ODCoordinate r, ODCoordinate b, ODRect* aFloatRect)
{
  left = l; top = t; right = r; bottom = b;
  if (aFloatRect) {
    floatRect = *aFloatRect;
  } else {
    floatRect.Clear();
  }
}

//------------------------------------------------------------------------------
// ODToolSpaceRect::IsEmpty
//------------------------------------------------------------------------------

ODBoolean ODToolSpaceRect::IsEmpty() const
{
  if ((left==0) && (top==0) && (right==0) && (bottom==0) && floatRect.IsEmpty()) {
    return(kODTrue);
  } else {
    return(kODFalse);
  }
}

//------------------------------------------------------------------------------
// ODToolSpaceRect::Set
//------------------------------------------------------------------------------

void ODToolSpaceRect::Set(ODCoordinate l, ODCoordinate t,
                          ODCoordinate r, ODCoordinate b, ODRect* aFloatRect)
{
  left=l;
  top=t;
  right=r;
  bottom=b;
  if (aFloatRect) {
    floatRect = *aFloatRect;
  } else {
    floatRect.Clear();
  }
}

//------------------------------------------------------------------------------
// ODToolSpaceRect::Clear
//------------------------------------------------------------------------------

void ODToolSpaceRect::Clear(void)
{
  left=0;
  top=0;
  right=0;
  bottom=0;
  floatRect.Clear();
}
// end [141114] pfe
