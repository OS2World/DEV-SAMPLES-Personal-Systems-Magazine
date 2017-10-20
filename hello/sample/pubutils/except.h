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
/*  OpenDoc C++ macros for handling SOM Exceptions.  Be sure to     */
/*  include this file BEFORE any .xh file because the bindings      */
/*  use the SOMCHKEXCEPT macro defined here.                        */
/*                                                                  */
/********************************************************************/

#ifndef _EXCEPT_
#define _EXCEPT_

#ifndef INCL_ODDTS // include non-DTS C++ headers

#ifndef SOM_Module_OpenDoc_Global_TypesB_defined
#include "ODTypesB.xh"
#endif

#ifndef _ERRORDEF_
#include "ErrorDef.xh"  // Clients probably need the error codes as well
#endif

#else // include DTS C++ headers

#ifndef SOM_HH_DTS_Included
#include <som.hh>
#endif
#ifndef _DTS_HH_INCLUDED_ODTypesB
#include <ODTypesB.hh>
#endif
#ifndef _DTS_HH_INCLUDED_ErrorDef
#include <ErrorDef.hh>
#endif

#endif // ! INCL_ODDTS

const char* ODGetErrorStringRep(ODError err);  // returns the string rep of the err code

//=====================================================================================
// Exception Handling Macros (native C++ exceptions)
//=====================================================================================

class ODNativeException {
  ODError fError;
  char* fMessage;
  Environment* fEv;
public:
  ODNativeException(ODError error, const char* msg);
  ODNativeException(const ODNativeException& x);
  ~ODNativeException();
  int error() const {return fError;}
  const char* message() const {return fMessage;}
  void SetEnvironmentPtr( Environment *ev ) { fEv = ev; }
};
#define ErrorCode()       (_exception.error())
#define ErrorMessage()      (_exception.message())

#define TRY                         \
        try { 

#define CATCH_ALL                     \
        } catch(ODNativeException _exception) { 

#define RERAISE                       \
        throw

#define ENDTRY                        \
            }


//=====================================================================================
// Raising Exceptions
//=====================================================================================

#ifdef _PLATFORM_OS2_PPC_  /*PPC-@SK*/
#define __FUNCTION__  ""   /* CED */
#endif
#if ODDebug
#define __XFILE__     __FILE__
#define __XFUNCTION__ __FUNCTION__
#define __XLINE__     __LINE__
#else
#define __XFILE__     0
#define __XFUNCTION__ 0
#define __XLINE__     0
#endif

void THROWproc(ODError, const char*, int, const char*, const char*);
#define THROW_IF_ERROR(a)      THROW_IF_ERROR_M((a), kODNULL)
#define THROW_IF_NULL(a)       THROW_IF_NULL_M((a), kODNULL)
#define THROW(a)               THROW_M((a), kODNULL)
#define THROW_IF_ERROR_M(a,b)  if (a) {THROWproc((a), (b), __XLINE__, __XFUNCTION__, __XFILE__ );} else {}
#define THROW_IF_NULL_M(a,b)   if (!(a)){THROWproc(kODErrOutOfMemory,(b), __XLINE__, __XFUNCTION__, __XFILE__ );} else {}
#define THROW_M(a,b)           THROWproc((a),(b),  __XLINE__, __XFUNCTION__, __XFILE__ )

// These macros should be used to raise an OpenDoc SOM exception without causing
// transfer of control to a catch handler.  These macros do essentially the same
// thing as ODSetSOMException, with the added feature of logging the location
// (line #, function & filename) where the exception was raised.

void RAISEproc(Environment*, ODError, const char*, int, const char*, const char*);
#define SOM_RAISE(ev, a)             SOM_RAISE_M(ev, (a), kODNULL)
#define SOM_RAISE_M(ev, a, b)        RAISEproc(ev, (a), (b), __XLINE__, __XFUNCTION__, __XFILE__ )

//=====================================================================================
// SOM Exception Utilities
//=====================================================================================

// This modified TRY block should be used in SOM methods. It's just like a
// regular TRY...CATCH_ALL...ENDTRY except that the exception code will be
// stored in the Environment. Needless to say you should _not_ reraise!

#define SOM_TRY                         \
      TRY 

#define SOM_CATCH_ALL                     \
      CATCH_ALL                     \
        _exception.SetEnvironmentPtr(ev); 

#define SOM_ENDTRY                        \
      ENDTRY 

// ODSetSOMException stores an OD error code in the environment.
// ODGetSOMException returns the OD error code (if any) from an environment.

void  ODSetSOMException( Environment*, ODError, const char *msg =kODNULL );
void  ODSetSOMException( Environment*, ODNativeException& );
ODError  ODGetSOMException( Environment *ev );

const char * ODGetErrStringRep(ODError error);// so docshell can call it. 139921
// CHECK_ENV throws an exception if the environment indicates an error.

void  CHECK_ENV( Environment* );

// SOMCHKEXCEPT is a macro that is called in a .xh file if the ev variable
// indicates an exception is set.
#define SOMCHKEXCEPT { CHECK_ENV(ev); }

#endif // _EXCEPT_
