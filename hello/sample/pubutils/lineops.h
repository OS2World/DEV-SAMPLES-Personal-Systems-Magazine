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
/********************************************************************/
/*
  File:    LineOps.h

  Contains:  Geometric operations on lines in 2-space.

  Theory of Operation:
  
  Endpoints of lines or ranges need not be given in any particular order.
  
  Ranges include both endpoints, unline rectangles which don't include right and bottom.
  (This is necessary to make segments that share an endpoint intersect, and to make e.g.
  the horizontal range of a vertical line be non-empty.)
  
*/


#ifndef _LINEOPS_
#define _LINEOPS_

#ifndef INCL_ODDTS // include non-DTS C++ headers

#ifndef SOM_Module_OpenDoc_Global_TypesB_defined
#include "ODTypesB.xh"    
#endif

#else // include DTS C++ headers

#ifndef SOM_HH_DTS_Included
#include <som.hh>
#endif
#ifndef _DTS_HH_INCLUDED_ODTypesB
#include <ODTypesB.hh>
#endif

#endif // ! INCL_ODDTS

enum{
  kIntersection,        // Segments intersect
  kOutsideIntersection,    // Lines intersect, but past ends of segments
  kNoIntersection        // Lines are parallel or degenerate
};
typedef short IntersectionStatus;


const ODCoordinate kFixedEpsilon = 7;    // This is about 0.0001 pixels


extern "C" {


inline ODCoordinate Min( ODCoordinate a, ODCoordinate b )  {return a<b ?a :b;}
inline ODCoordinate Max( ODCoordinate a, ODCoordinate b )  {return a>b ?a :b;}

inline ODCoordinate Abs( ODCoordinate n )          {return n>=0 ?n :-n;}

ODBoolean WithinEpsilon( ODCoordinate a, ODCoordinate b );

ODCoordinate    Distance( const ODPoint &p0, const ODPoint &p1 );

void        GetLineShift( ODPoint p0, ODPoint p1, ODCoordinate dist,
                  ODPoint &delta );

IntersectionStatus  IntersectLines( const ODPoint &p0, const ODPoint &p1,
                    const ODPoint &q0, const ODPoint &q1,
                    ODPoint *sect );
          
ODBoolean           IntersectSegments( const ODPoint &p0, const ODPoint &p1,
                     const ODPoint &q0, const ODPoint &q1,
                     ODPoint *sect );

ODCoordinate    GetLineXAtY( const ODPoint &p0, const ODPoint &p1,
                     ODCoordinate y );

ODFract        GetIntersectionT( const ODPoint &p0, const ODPoint &p1,
                    const ODPoint &sect );

ODBoolean           InRange( ODCoordinate n,  ODCoordinate r0, ODCoordinate r1 );

ODBoolean           RangesDisjoint( ODCoordinate a0, ODCoordinate a1,
                  ODCoordinate b0, ODCoordinate b1 );

}

#endif //_LINEOPS_
