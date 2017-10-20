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
/* File:    AltPoly.h                                               */
/*                                                                  */
/* Contains:  OpenDoc polygon: optional C++ savvy classes           */
/*                                                                  */
/* Theory of Operation:                                             */
/*                                                                  */
/*  This is an alternate definition of ODPolygon and ODContour.     */
/*  The data format is identical, but the structs defined here      */
/*  have a lot of useful methods including constructors, accessors  */
/*  and conversion operators.                                       */
/*                                                                  */
/*  To use these instead of the regular structs defined in          */
/*  Polygon.h, just include this header file _before_ Polygon.h.    */
/*  An easy way to do this is to include it first.                  */
/*                                                                  */
/********************************************************************/
#ifndef _ALTPOLY_
#define _ALTPOLY_

#ifndef INCL_ODDTS // include non-DTS C++ headers

// Make sure that built-in structs do not get re-defined
#ifndef SOM_Module_Polygon_OpenDoc_Polygon_defined
  #define SOM_Module_Polygon_OpenDoc_Polygon_defined 2
#else
  #error "Must include AltPoly.h *before* os2.h!"
#endif

#else // include DTS C++ headers

// Make sure that built-in structs do not get re-defined
#ifndef _DTS_HH_INCLUDED_Polygon
  #define _DTS_HH_INCLUDED_Polygon 2
#else
  #error "Must include AltPoly.h *before* os2.h!"
#endif

#endif // ! INCL_ODDTS

#ifndef _ALTPOINT_
#include "AltPoint.h"
#endif

#include <stddef.h>          // for size_t

//==============================================================================
// Classes used in this interface
//==============================================================================

struct ODRect;
class ODStorageUnit;
class ODTransform;
typedef struct _POLYGON POLYGON;    // defined in pmgpi.h

//==============================================================================
// ODContour
//==============================================================================

struct ODContour
{
  public:
  
  ODSLong  nVertices;
  ODPoint  vertex[1];    // Array size is actually nVertices
  
  ODContour* NextContour( )        const  {return (ODContour*)&vertex[nVertices];}
  ODBoolean  IsRectangular( )      const;
  ODBoolean  AsRectangle( ODRect* )    const;
  void AsPOLYGON( POLYGON& )        const;
  ODBoolean  HasExactRegion( )      const;

  ODBoolean  operator== ( const ODContour& )      const;
  ODBoolean  operator!= ( const ODContour &c )      const  {return !(*this==c);}
};

//==============================================================================
// ODPolygonData
//==============================================================================

struct ODPolygonData {
  ODSLong  nContours;            // Number of contours
  ODContour  firstContour;        // Rest of contours follow after first
};

//==============================================================================
// ODPolygon
//==============================================================================

class ODPolygon
{
  public:
  
  ODPolygon( );
//~ODPolygon ( );          // IBM CSet++ doesn't treat ODPolygons exactly the same
                           // as ODByteArrays when used as a return value if the 
                           // class has a destructor.

  void      Delete( );      // Delete myself & my data
  void      Clear( );      // Just deletes my data
  
  // ACCESSORS:
  
  ODBoolean    HasData( )            const  {return _length!=0;}
  ODULong      GetDataSize( )          const  {return _length;}
  ODPolygonData*  GetData( )            const  {return _buf;}
  
  void      SetData( const ODPolygonData* );  // Does not copy the data!
  
  ODSLong      GetNContours( )          const;
  ODContour*    FirstContour( );
  const ODContour*FirstContour( )          const;
  
  // GEOMETRY:
  
  void    ComputeBoundingBox( ODRect* )    const;
  ODBoolean  IsRectangular( )          const;
  void    Transform( Environment*, ODTransform* );
  
  ODBoolean  operator== ( ODPolygon& )      const;
  ODBoolean  operator!= ( ODPolygon& p )      const  {return !(*this==p);}
  
  ODSLong  Contains( ODPoint )            const;
  ODBoolean  IsEmpty( )              const;
  
  // CONVERSIONS:
  
  ODBoolean  AsRectangle( ODRect* )        const;  // False if nonrectangular
  HRGN       AsRegion( HPS hps ) const;
  ODBoolean  HasExactRegion( )          const;

  // ALLOCATION:
  
  ODPolygon*  SetNVertices( ODSLong nVertices );
  ODPolygon*  SetVertices( ODSLong nVertices, const ODPoint *vertices );
  ODPolygon*  SetContours( ODSLong nContours, const ODSLong *contourVertices );
  ODPolygon*  SetRect( const ODRect& );

  ODPolygon*  Copy( )                const;
  ODPolygon*  CopyFrom( const ODPolygon& );
  ODPolygon*  MoveFrom( ODPolygon& );    // Justs adjusts pointers, no copying
  
  // INPUT/OUTPUT:
  
  ODPolygon*  ReadFrom( Environment*, ODStorageUnit* );
  ODPolygon*  WriteTo( Environment*, ODStorageUnit* )    const;
  
  private:
  
  void    Realloc( ODULong dataSize );
  
  // DATA MEMBERS:
  
    unsigned long _maximum;            // Exact same data as an ODByteArray
    unsigned long _length;
    ODPolygonData *_buf;
};


//==============================================================================
// ODTempPolygon
//==============================================================================

/*  ODTempPolygon is a polygon whose destructor disposes of its data.
  This is useful if you have a local variable that's a temporary polygon
  and you want to make sure the data gets disposed.
*/

class ODTempPolygon :public ODPolygon
{
public:
  ODTempPolygon( )  { }          // Just to avoid warnings
   ~ODTempPolygon( )  {this->Clear();}
};

/*  ODTempPolygonPtr is a _pointer_ to a polygon, whose destructor deletes
  the polygon structure (and its data.) Yes, it is a class, but due to the
  magic of operator overloading it can be used just as a pointer. See the
  implementation of ODPolygon::Copy in AltPoly.cpp for an example. */

class ODTempPolygonPtr
{
public:
  ODTempPolygonPtr( );
  ODTempPolygonPtr( ODPolygon* );
  ~ODTempPolygonPtr( );
  operator ODPolygon* ( )          {return fPoly;}
  ODPolygon* operator-> ( )        {return fPoly;}
  ODPolygon* operator= ( ODPolygon *p )  {return (fPoly=p);}
  ODPolygon* DontDelete( )        {ODPolygon* temp=fPoly; fPoly=kODNULL; return temp;}
  
private:
  ODPolygon *fPoly;
};


//==============================================================================
// Polygon Edge Iterator
//==============================================================================

class PolyEdgeIterator {
  public:

  PolyEdgeIterator( const ODPolygon* );
  
  void    CurrentEdge( const ODPoint* &v1, const ODPoint* &v2 );
  const ODContour* CurrentContour( )      {return fCurContour;}
  long    CurrentContourIndex( )      {return fCurContourIndex;}
  long    CurrentEdgeIndex( )        {return fCurVertex;}
  
  ODBoolean  Next( );
  ODBoolean  IsNotComplete( );
  
  private:
  const ODPolygon*  const fPoly;
  const ODContour*      fCurContour;
  long            fCurContourIndex;
  long            fCurVertex;
};


#endif //_ALTPOLY_
