/*****************************************************************************/
/*  Licensed Materials - Property of IBM                                     */
/*                                                                           */
/*                                                                           */
/* Copyright (C) International Business Machines Corp., 1994.                */
/* Copyright (C) Apple Computer, Inc., 1994                                  */
/*                                                                           */
/*  US Government Users Restricted Rights -                                  */
/*  Use, duplication, or disclosure restricted                               */
/*  by GSA ADP Schedule Contract with IBM Corp.                              */
/*                                                                           */
/*  File:    AltPoint.h                                                      */
/*                                                                           */
/*  Contains:  C++ savvy points and rects (alternate ODPoint, ODRect)        */
/*                                                                           */
/*  Notes:                                                                   */
/*                                                                           */
/*  These are alternate definitions of the ODPoint and ODRect structs.       */
/*  These definitions have the same size and data format and can be used     */
/*  interchangeably with the basic definitions; but they're much more C++    */
/*  savvy, with constructors, operators, conversions, and utility methods.   */
/*                                                                           */
/*  To use these classes instead of the defaults, just include "AltPoint.h"  */
/*  as the first thing in your source file. It has to be included first so   */
/*  it can override the default struct definitions in PlfmType.h.            */
/*                                                                           */
/*  This API and implementation are **NOT** an official part of the OpenDoc  */
/*  API, just handy utilities for C++ programmers.                           */
/*                                                                           */
/*****************************************************************************/

#ifndef _ALTPOINT_
#define _ALTPOINT_

#ifndef __OS2DEF__
#include <os2def.h>
#endif

#ifndef INCL_ODDTS // include non-DTS C++ headers

// Make sure that built-in structs do not get re-defined
#ifndef SOM_Module_GeoTypes_OpenDoc_GeoTypes_defined
  #define SOM_Module_GeoTypes_OpenDoc_GeoTypes_defined 2
#else
  #error "Must include AltPoint.h *before* os2.h!"
#endif

#ifndef SOM_Module_OpenDoc_Global_TypesB_defined
#include "ODTypesB.xh"
#endif

#else // include DTS C++ headers

// Make sure that built-in structs do not get re-defined
#ifndef _DTS_HH_INCLUDED_GeoTypes
  #define _DTS_HH_INCLUDED_GeoTypes 2
#else
  #error "Must include AltPoint.h *before* os2.h!"
#endif

#ifndef SOM_HH_DTS_Included
#include <som.hh>
#endif
#ifndef _DTS_HH_INCLUDED_ODTypesB
#include <ODTypesB.hh>
#endif

#endif // ! INCL_ODDTS

//==============================================================================
// ODCoordinate
//==============================================================================

typedef ODFixed ODCoordinate;

//==============================================================================
// ODPoint
//==============================================================================

struct ODPoint {
  public:

  // CONTENTS:

  ODCoordinate x, y;

  // CONSTRUCTORS:

  ODPoint( ) { }

  ODPoint( ODCoordinate xx, ODCoordinate yy )
          {x=xx; y=yy;}

  ODPoint( const ODPoint& );        // Copy constructor

  // ASSIGNMENT:

  ODPoint& operator= ( const ODPoint& );  // Copy from another pt

  // MODIFICATION:

  inline void  Clear( )
          {x=y=0;}
  inline void  Set( ODCoordinate xx, ODCoordinate yy )
          {x=xx; y=yy;}
  void  Offset( ODCoordinate x, ODCoordinate y );
  void  operator+=( const ODPoint& );
  void  operator-=( const ODPoint& );

  // ACCESSORS:

  ODSShort  IntX( )    const;    // Returns X-coord as (16bit) integer
  ODSShort  IntY( )    const;    // Returns Y-coord as (16bit) integer

  // COMPARISON:

  ODBoolean  operator==( const ODPoint& )  const;
  ODBoolean  operator!=( const ODPoint& )  const;
  ODBoolean  ApproxEquals( const ODPoint& )  const;    // to within roundoff error

  // CONVENIENCES:

  ODPoint( POINTL ptl);                       // Construct from GPI POINTL
  ODPoint& operator= ( const POINTL& ptl);    // Copy from a GPI POINTL
  POINTL  AsPOINTL( )          const;         // Convert to integer POINTL
  void  operator+=( const    POINTL& ptl);    // Add/subtract POINTL
  void  operator-=( const    POINTL& ptl);
};


//==============================================================================
// ODRect
//==============================================================================

struct ODRect {
  public:

  // CONTENTS:

  ODCoordinate left, top, right, bottom;

  // CONSTRUCTORS:

  ODRect( )    { }
  ODRect( ODCoordinate l, ODCoordinate t,
          ODCoordinate r, ODCoordinate b )
      {left=l; top=t; right=r; bottom=b; }
  ODRect( const ODPoint&, const ODPoint& );  // Any 2 opposite pts
  ODRect( const ODPoint &topLeft, ODCoordinate width, ODCoordinate height );
  ODRect( const RECTL& r);

  // ASSIGNMENT:

  ODRect& operator= ( const RECTL& r);

  // MODIFICATION:

  void  Clear( );
  void  Set( ODCoordinate l, ODCoordinate t, ODCoordinate r, ODCoordinate b );
  void  Set( const ODPoint&, ODCoordinate width, ODCoordinate height );
  void  SetInt( short l, short t, short r, short b );
  void  Offset( ODCoordinate x, ODCoordinate y );
  void  Offset( const ODPoint& );
  void  Inset( ODCoordinate x, ODCoordinate y );

  void  operator&= ( const ODRect& );  // Intersect with rectangle
  void  operator|= ( const ODRect& );  // Union with rectangle
  void  operator|= ( const ODPoint& );    // Expand to fit point

  // ACCESSORS

  ODPoint         BotLeft( )                    const
                { return ODPoint(left, bottom);}
  ODPoint         TopRight( )                    const
                {return ODPoint(right, top);}
  ODCoordinate  Width( )                    const
                {return right-left;}
  ODCoordinate  Height( )                    const
                {return bottom-top;}
  void          AsRECTL( RECTL& r ) const;

  // TESTING

  ODBoolean  operator==( const ODRect& )            const;
  ODBoolean  operator!=( const ODRect &r )            const
                {return !(*this==r);}
  ODBoolean  ApproxEquals( const ODRect &r )            const;

  ODBoolean IsEmpty( )                      const;
  ODBoolean Contains( const ODPoint& )              const;
  ODBoolean Contains( const ODRect& )              const;
  ODBoolean  ApproxContains( const ODRect& )            const;
  ODBoolean Intersects( const ODRect& )            const;
};

struct ODToolSpaceRect {
public:
       ODCoordinate    left;
       ODCoordinate    top;
       ODCoordinate    right;
       ODCoordinate    bottom;
       ODRect          floatRect;

       ODToolSpaceRect( )              { }

       ODToolSpaceRect(ODCoordinate l, ODCoordinate t,
                       ODCoordinate r, ODCoordinate b, ODRect* aFloatRect);

       ODBoolean IsEmpty() const;

       void Set(ODCoordinate l, ODCoordinate t,
                ODCoordinate r, ODCoordinate b, ODRect* aFloatRect);

       void Clear(void);
};

#endif //_ALTPOINT_
