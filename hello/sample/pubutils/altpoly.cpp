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
/*  File:    AltPoly.cpp                                            */
/*                                                                  */
/*  Contains:  OpenDoc polygon: optional C++ savvy classes          */
/*                                                                  */
/*  To Do:                                                          */
/*                                                                  */
/*    Improve the equality tests for contours and polygons. See     */
/*    comments in the two "operator==" methods for details.         */
/*                                                                  */
/********************************************************************/

#ifndef _ALTPOINT_
#include "AltPoint.h"
#endif

#ifndef _ALTPOLY_
#include "AltPoly.h"
#endif

#ifndef _EXCEPT_
#include "Except.h"
#endif

#ifndef _ODDEBUG_
#include "ODDebug.h"
#endif

#ifndef _LINEOPS_
#include "LineOps.h"
#endif

#define INCL_GPI
#define INCL_ODAPI
#define INCL_ODSTORAGEUNIT
#define INCL_ODTRANSFORM
#include <os2.h>

#ifndef _STDTYPIO_
#include <StdTypIO.h>
#endif

#include <stddef.h>          // Defines offsetof() macro


const ODSLong kMaxLong  = 0x7FFFFFFF;

// private error code:
const ODError kODErrShapeTooComplex        = 125;    // Polygon shape is too huge for QuickDraw
// This error code was not used anywhere else, and since
// AltPoly is private utility, its error codes should not be public

//==============================================================================
// Destructos
//==============================================================================


ODTempPolygonPtr::ODTempPolygonPtr( )
  :fPoly(kODNULL)
{
}


ODTempPolygonPtr::ODTempPolygonPtr( ODPolygon *p )
  :fPoly(p)
{
}


ODTempPolygonPtr::~ODTempPolygonPtr( )
{
  delete fPoly;
  fPoly = kODNULL;
}

/******************************************************************************/
//**  ALLOCATION
/******************************************************************************/


ODPolygon::ODPolygon( )
  :_maximum(0),
   _length(0),
   _buf(kODNULL)
{
}

void
ODPolygon::Delete( )
{
  SOMFree(_buf);
  delete this;
}


void
ODPolygon::Clear( )
{
  SOMFree(_buf);
  _buf = kODNULL;
  _length = _maximum = 0;
}


static ODULong
CalcDataSize( ODSLong nVertices )
{
  if( nVertices==0 )
    return 0;
  else
    return sizeof(ODPolygonData)+(nVertices-1)*sizeof(ODPoint);
//  return offsetof(ODPolygonData,firstContour.vertex[nVertices]);
}


void
ODPolygon::Realloc( ODULong dataSize )
{
  if( _buf!=kODNULL && dataSize>=_length && dataSize<=_maximum )
    _length = dataSize;
  else {
    ODPtr newData;
    if( dataSize!=0 )
      newData = (ODPtr)SOMMalloc(dataSize);
    SOMFree(_buf);
    if( dataSize!=0 )
      _buf = (ODPolygonData*)newData;
    else
      _buf = kODNULL;
    _length = _maximum = dataSize;
  }
}


void
ODPolygon::SetData( const ODPolygonData *data )
{
  _length = sizeof(ODULong) * (1+data->nContours);
  const ODContour *c = &data->firstContour;
  for( ODULong i=data->nContours; i!=0; i-- ) {
    _length += c->nVertices * sizeof(ODPoint);
    c = c->NextContour();
  }

  SOMFree(_buf);
  _buf = (ODPolygonData*)data;
  _maximum = _length;
}


ODPolygon*
ODPolygon::SetNVertices( ODSLong nVertices )
{
  ASSERT(nVertices>=0,kODErrInvalidValue);

  this->Realloc(CalcDataSize(nVertices));
  if( nVertices>0 ) {
    _buf->nContours = 1;
    _buf->firstContour.nVertices = nVertices;
  }
  return this;
}


ODPolygon*
ODPolygon::SetVertices( ODSLong nVertices, const ODPoint *vertices )
{
  ASSERT(nVertices>=0,kODErrInvalidValue);
  ASSERT(vertices!=kODNULL,kODErrInvalidValue);

  this->SetNVertices(nVertices);
  if( nVertices>0 )
    memcpy( _buf->firstContour.vertex, (void *) vertices, nVertices*sizeof(ODPoint) );
  return this;
}


ODPolygon*
ODPolygon::SetContours( ODSLong nContours, const ODSLong *contourVertices )
{
  ASSERT(nContours>=0,kODErrInvalidValue);
  if( nContours==0 )
    return this->SetNVertices(0);
  else {
    ASSERT(contourVertices!=kODNULL,kODErrInvalidValue);
    ODULong totalVertices = 0;
    ODSLong i;
    for( i=nContours-1; i>=0; i-- )
      totalVertices += contourVertices[i];
    this->Realloc( offsetof(ODPolygonData,firstContour)
           + offsetof(ODContour,vertex[0]) * nContours
           + sizeof(ODPoint)*totalVertices );
    _buf->nContours = nContours;
    ODContour *cont = this->FirstContour();
    for( i=0; i<nContours; i++ ) {
      cont->nVertices = contourVertices[i];
      cont = cont->NextContour();
    }
    return this;
  }
}


ODPolygon*
ODPolygon::SetRect( const ODRect &r )
{
  if( r.IsEmpty() )
    return this->SetNVertices(0);
  else {
    this->SetNVertices(4);
    _buf->firstContour.vertex[0] = r.BotLeft();
    _buf->firstContour.vertex[1].Set(r.left,r.top);
    _buf->firstContour.vertex[2] = r.TopRight();
    _buf->firstContour.vertex[3].Set(r.right,r.bottom);
  }
  return this;
}


ODPolygon*
ODPolygon::CopyFrom( const ODPolygon &poly )
{
  if( poly._buf != _buf ) {
    ODULong size = poly.GetDataSize();
    this->Realloc(size);
    memcpy(_buf,poly.GetData(),size);         
  }
  return this;
}


ODPolygon*
ODPolygon::MoveFrom( ODPolygon &poly )
{
  if( poly._buf != _buf ) {
    SOMFree(_buf);
    _buf = poly._buf;
    _length = poly._length;
    _maximum = poly._maximum;
  }
  if( &poly._buf != &_buf ) {      // Don't clear poly if it's myself!
    poly._buf = kODNULL;
    poly._length = poly._maximum = 0;
  }
  return this;
}

ODPolygon*
ODPolygon::ReadFrom( Environment *ev, ODStorageUnit *su )
{

  if( !su->Exists(ev,kODNULL,kODPolygon,kODPosUndefined) ) {
    this->Clear();
  } else {
    ODPropertyName propName = su->GetProperty(ev);
    ODGetPolygonProp(ev, su, propName, kODPolygon, this);
    SOMFree(propName);
  }

  return this;
}


ODPolygon*
ODPolygon::WriteTo( Environment *ev, ODStorageUnit *su )  const
{
  ODPropertyName propName = su->GetProperty(ev);
  ODSetPolygonProp(ev, su, propName, kODPolygon, this);
  SOMFree(propName);

  return (ODPolygon*)this;
}


/******************************************************************************/
//**  POLYGON STUFF
/******************************************************************************/


ODSLong
ODPolygon::GetNContours( )  const
{
  return _length>0 ?_buf->nContours :0;
}


const ODContour*
ODPolygon::FirstContour( )  const
{
  return _length>0 ?&_buf->firstContour :kODNULL;
}


ODContour*
ODPolygon::FirstContour( )
{
  return _length>0 ?&_buf->firstContour :kODNULL;
}


ODPolygon*
ODPolygon::Copy( ) const
{
  ODTempPolygonPtr poly = new ODPolygon;
    poly->CopyFrom(*this);
  return poly.DontDelete();
}


void
ODPolygon::ComputeBoundingBox( ODRect *bbox ) const
{
  ASSERT(bbox!=kODNULL,kODErrInvalidValue);

  if( _buf==kODNULL || _buf->nContours <= 0 ) {
    bbox->Clear();
    return;
  }

  // Start bbox out as maximally empty:
  bbox->left  = bbox->bottom  =  kMaxLong;
  bbox->right = bbox->top     = -kMaxLong;

  const ODContour *c = this->FirstContour();
  for( ODSLong i=this->GetNContours(); i>0; i-- ) {
    ODPoint *pt = (ODPoint*)c->vertex;
    for( ODSLong v=c->nVertices; v>0; v--,pt++ ) {
      if( pt->x < bbox->left )  bbox->left  = pt->x;
      if( pt->x > bbox->right )  bbox->right  = pt->x;
      if( pt->y > bbox->top )    bbox->top  = pt->y;
      if( pt->y < bbox->bottom )  bbox->bottom= pt->y;
    }
    if( i>1 )
      c = c->NextContour();
  }
}


ODBoolean
ODContour::operator== ( const ODContour &cont ) const
{
  /*  This test is complicated by the fact that the two contours might have the
    same points, but out of phase. Therefore we have to compare the points
    in sequence, once per possible phase difference.  */

  ODSLong nv = this->nVertices;
  if( nv != cont.nVertices )
    return kODFalse;

  for( ODSLong phase=0; phase<nv; phase++ ) {
    const ODPoint *p0 = &this->vertex[0];
    const ODPoint *p1 = &cont.vertex[phase];
    ODSLong i;
    for( i=nVertices; i>0; i-- ) {
      if( i==phase )
        p1 = &cont.vertex[0];
      if( ! (p0++)->ApproxEquals(*p1++) )    // Coords may differ very slightly
        break;
    }
    if( i==0 )
      return kODTrue;
  }
  return kODFalse;
}


ODBoolean
ODPolygon::operator== ( ODPolygon &poly ) const
{
  /*  This test is complicated by the fact that the two polygons may not have their
    contours in the same order. Our approach is to step through my contours in
    order, trying to match each to a unique contour in the target. To ensure
    uniqueness, the sign of the nVertices field in a target contour is flipped
    after it's matched.  */

  if( this->GetNContours() != poly.GetNContours() )
    return kODFalse;
  if( &poly == this )
    return kODTrue;

  ODBoolean result = kODTrue;
  const ODContour * c = this->FirstContour();
  ODContour *pc;

  ODSLong i;
  for( i=this->GetNContours(); i>0; i-- ) {
    pc = poly.FirstContour();
    ODSLong j;
    for( j=poly.GetNContours(); j>0; j-- ) {
      if( pc->nVertices>0 && *c==*pc ) {    // Compare contours!
        pc->nVertices = -pc->nVertices;    // Use sign bit as a flag (yech)
        break;
      }
      if( j>1 )
        pc = pc->NextContour();
    }
    if( j<=0 ) {
      result = kODFalse;          // No match for contour
      break;
    }

    if( i>1 )
      c = c->NextContour();
  }

  // Now that we know, clear all the sign bits:
  pc = poly.FirstContour();
  for( i=poly.GetNContours(); i>0; i-- ) {
    if( pc->nVertices<0 )
      pc->nVertices = -pc->nVertices;
    pc = pc->NextContour();
  }
  return result;
}


ODBoolean
ODPolygon::IsEmpty( ) const
{
  // FIX: This is not very smart. It will probably be necessary to compute the area
  // of each contour...

  return _buf==kODNULL || _buf->nContours==0;
}


ODBoolean
ODPolygon::IsRectangular( ) const
{
  return _buf==kODNULL || _buf->nContours==0 ||
      (_buf->nContours==1  && _buf->firstContour.IsRectangular());
}


ODBoolean
ODPolygon::AsRectangle( ODRect *r ) const
{
  if( _buf==kODNULL || _buf->nContours==0 ) {
    r->Clear();
    return kODTrue;
  } else if( _buf->nContours==1 )
    return _buf->firstContour.AsRectangle(r);
  else
    return kODFalse;
}


ODBoolean
ODContour::IsRectangular( ) const
{
  if( nVertices != 4 )
    return kODFalse;
  else if( vertex[0].x == vertex[1].x )      // 1st edge is vertical
    return vertex[1].y==vertex[2].y
        && vertex[2].x==vertex[3].x
        && vertex[3].y==vertex[0].y;
  else if( vertex[0].y == vertex[1].y )      // 1st edge is horizontal
    return vertex[1].x==vertex[2].x
        && vertex[2].y==vertex[3].y
        && vertex[3].x==vertex[0].x;
  else
    return kODFalse;
}


ODBoolean
ODContour::AsRectangle( ODRect *r ) const
{
  ASSERT(r!=kODNULL,kODErrInvalidValue);

  if( this->IsRectangular() ) {
    ODRect r2(vertex[0],vertex[2]);      // C'tor properly orders the coords
    *r = r2;
    return kODTrue;
  } else
    return kODFalse;
}


/******************************************************************************/
//**  REGION CONVERSION
/******************************************************************************/


ODBoolean
ODContour::HasExactRegion( ) const
{
  const ODPoint *b = &vertex[0];
  for( ODSLong i=nVertices; i>=0; i-- ) {
    const ODPoint *a = &vertex[i];
    if( (a->x & 0xFFFF) || (a->y & 0xFFFF) )
      return kODFalse;            // Non-integer coordinates
    if( a->x!=b->x && a->y!=b->y )
      return kODFalse;            // Diagonal line
    b = a;
  }
  return kODTrue;
}


ODBoolean
ODPolygon::HasExactRegion( ) const
{
  const ODContour *c = this->FirstContour();
  for( long i=this->GetNContours(); i>0; i-- ) {
    if( !c->HasExactRegion() )
      return kODFalse;
    if( i>1 )
      c = c->NextContour();
  }
  return kODTrue;
}


void ODContour::AsPOLYGON( POLYGON& p) const
{
  p.aPointl = (PPOINTL)SOMMalloc(nVertices * sizeof(POINTL));

  const ODPoint *src = &vertex[0];
  POINTL *dst = p.aPointl;
  for( ODSLong i=nVertices; i>0; i-- ) {
    *dst = (src++)->AsPOINTL();
    (dst++)->y -= 1;              // OS/2 polygons are bottom-right exclusive
                                  // while OpenDoc polygons are top-right
                                  // exclusive.
  }
  p.ulPoints = nVertices;
}


// The canvas (HPS) is assumed to be associated with the target device
// for the which the region will be used, and already set up with the
// desired transforms.

HRGN
ODPolygon::AsRegion(HPS hps) const
{
  ODRgnHandle rgn;
  POLYGON poly;

  if( !this->HasData() )
    return GpiCreateRegion(hps, 0, 0);

  GpiSavePS(hps);
  GpiBeginPath(hps, 1);
  TRY {
    const ODContour *cont = this->FirstContour();
    for( ODSLong i=_buf->nContours; i>0; i-- ) {
      POLYGON poly;
      cont->AsPOLYGON( poly );
      GpiSetCurrentPosition(hps, &poly.aPointl[poly.ulPoints-1]);
      GpiPolyLine(hps, poly.ulPoints, poly.aPointl);
      delete poly.aPointl;
      cont = cont->NextContour();
    }
  }CATCH_ALL{
    delete[] poly.aPointl;
    GpiEndPath(hps);
    GpiRestorePS(hps, -1);
    RERAISE;
  }ENDTRY

  GpiEndPath(hps);
  rgn = GpiPathToRegion(hps, 1, FPATH_WINDING | FPATH_EXCL);
  GpiRestorePS(hps, -1);
  THROW_IF_NULL(rgn);
  return rgn;
}

void
ODPolygon::Transform( Environment *ev, ODTransform *xform )
{
  if( this->HasData() ) {
    ODContour *c = &_buf->firstContour;
    for( ODSLong i=_buf->nContours; i>0; i-- ) {
      ODByteArray verticies;
      UseByteArray(&verticies, c->vertex, c->nVertices * sizeof(ODPoint));
        xform->TransformPoints(ev,&verticies, c->nVertices);
      if( i>1 )
        c = c->NextContour();
    }
  }
}


/******************************************************************************/
//**  CONTAINMENT TEST
/******************************************************************************/


//------------------------------------------------------------------------------
// ODPolygon::Contains
//
// Does a polygon contain a point?
// We determine this by drawing a ray from the point to the right towards
// infinity, and finding the polygon edges that intersect this ray. For each
// such edge, count it as 1 if its y value is increasing, -1 if decreasing.
// The sum of these values is 0 if the point is outside the polygon.
//------------------------------------------------------------------------------

ODSLong
ODPolygon::Contains( ODPoint point ) const
{
  if( !this->HasData() )
    return kODFalse;

  ODSLong count = 0;
  const ODPoint *pp1, *pp2;
  ODPoint p1, p2;
  ODPoint ray = point;

  for( PolyEdgeIterator polyIter (this); polyIter.IsNotComplete(); polyIter.Next() ) {
    polyIter.CurrentEdge(pp1,pp2);
    p1 = *pp1;
    p2 = *pp2;

    if( p1.y==p2.y ) {                      // Horizontal line: ignore
      if( p1.y==point.y && InRange(point.x, p1.x,p2.x) ) {  // unless point is on it
        return 0;
      }
    } else {
      ray.x = Max(p1.x,p2.x);
      ODPoint sect;
      if( ray.x >= point.x )
        if( IntersectSegments(p1,p2, point,ray, &sect) ) {
          if( WithinEpsilon(point.x,sect.x) && WithinEpsilon(point.y,sect.y) ) {
            return 0;
          }
          if( p2.y > p1.y )
            count++;
          else
            count--;
        }
    }
  }

  return count;
}


/******************************************************************************/
//**  POLYGON EDGE ITERATOR
/******************************************************************************/


PolyEdgeIterator::PolyEdgeIterator( const ODPolygon *poly )
  :fPoly (poly)
{
  fCurContour = poly->FirstContour();
  fCurContourIndex = 0;
  fCurVertex = 0;
}


void
PolyEdgeIterator::CurrentEdge( const ODPoint* &v1, const ODPoint* &v2 )
{
  v1 = &fCurContour->vertex[fCurVertex];
  if( fCurVertex+1 < fCurContour->nVertices )
    v2 = v1+1;
  else
    v2 = &fCurContour->vertex[0];
}


ODBoolean
PolyEdgeIterator::Next( )
{
  if( !fCurContour )                  // Was already finished
    return kODFalse;
  if( ++fCurVertex >= fCurContour->nVertices )    // Next vertex; if past contour:
    if( ++fCurContourIndex >= fPoly->GetNContours() ) {    // Next contour; if past end:
      fCurContour = kODNULL;
      return kODFalse;                //...we're done.
    } else {
      fCurContour = fCurContour->NextContour();      // Else go to start of contour
      fCurVertex = 0;
    }
  return kODTrue;
}


ODBoolean
PolyEdgeIterator::IsNotComplete( )
{
  return fCurContour!=kODNULL;
}
