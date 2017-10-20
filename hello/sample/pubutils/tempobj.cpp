/*
  File:    TempObj.cpp

  Contains:  Template utilities for exception-safe temporary object references
*/

#ifndef _TEMPOBJ_
#include "TempObj.h"
#endif

#define INCL_ODAPI
#define INCL_ODFRAME
#define INCL_ODPART
#define INCL_ODREFCNTOBJECT
#define INCL_ODSHAPE
#define INCL_ODSTORAGEUNIT
#define INCL_ODTRANSFORM
#include <os2.h>

#ifndef _ODDEBUG_
#include "ODDebug.h"
#endif

BaseTempObj::~BaseTempObj( )
{
  delete fObj;
}

void
BaseTempRef::Release( )
{
  if( fObj ) {
    ASSERT_IS_A(fObj,ODRefCntObject);
    fObj->Release(somGetGlobalEnvironment());
    fObj = kODNULL;
  }
}

BaseTempRef::~BaseTempRef( )
{
  if( fObj ) {
    ASSERT_IS_A(fObj,ODRefCntObject);
    fObj->Release(somGetGlobalEnvironment());
    fObj = kODNULL;
  }
}
