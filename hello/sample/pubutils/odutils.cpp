/********************************************************************/
/*                                                                  */
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
/*  File:    ODUtils.cpp                                            */
/*                                                                  */
/*  Contains:  Utility functions for objects                        */
/*                                                                  */
/********************************************************************/

#ifndef _EXCEPT_
#include <Except.h>
#endif

#define INCL_ODAPI
#define INCL_ODSHAPE
#define INCL_ODTRANSFORM
#include <os2.h>

#ifndef _ODDEBUG_
#include <ODDebug.h>
#endif

#ifndef _ODUTILS_
#include "ODUtils.h"
#endif


void ODAcquireObject(Environment* ev, ODRefCntObject* object)
{
  if (object != kODNULL) 
  {
    object->Acquire(ev);
  }
}

void ODSafeReleaseObject( ODRefCntObject *obj )
{
  if( obj ) {
      TRY{
        obj->Release(somGetGlobalEnvironment());
      }CATCH_ALL{
        // do nothing
      }ENDTRY
  }
}


void ODTransferReference( Environment *ev, ODRefCntObject *oldObj, ODRefCntObject *newObj )
{
  if( oldObj != newObj ) {
    if( newObj )
      newObj->Acquire(ev);
    if( oldObj ) {
      TRY{
        oldObj->Release(ev);
      }CATCH_ALL{
        if( newObj ) newObj->Release(ev);
        RERAISE;
      }ENDTRY
    }
  }
}


ODShape* ODCopyAndRelease( Environment *ev, ODShape *s )
{
  ASSERT(s!=kODNULL,kODErrInvalidParameter);
  if( s->GetRefCount(ev) == 1 )
    return s;
  ODShape *copy = s->Copy(ev);
  s->Release(ev);
  return copy;
}

ODTransform* ODCopyAndRelease( Environment *ev, ODTransform *t )
{
  ASSERT(t!=kODNULL,kODErrInvalidParameter);
  if( t->GetRefCount(ev) == 1 )
    return t;
  ODTransform *copy = t->Copy(ev);
  t->Release(ev);
  return copy;
}

ODBoolean ODObjectsAreEqual(Environment* ev, ODObject* a, ODObject* b)
{
  return (a == b) || (a && a->IsEqualTo(ev,b));
  // Alternative:
  // return a ? a->IsEqualTo(ev, b) : !b;
}
