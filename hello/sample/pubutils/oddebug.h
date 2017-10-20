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
/*  File:    ODDebug.h                                              */
/*                                                                  */
/*  Contains:  Useful debugging macros and functions.               */
/*                                                                  */
/********************************************************************/


#ifndef _ODDEBUG_
#define _ODDEBUG_

#ifndef INCL_ODDTS // include non-DTS C++ headers

#include <somobj.xh>
#include <somcls.xh>

#ifndef SOM_Module_OpenDoc_Global_TypesB_defined
#include "ODTypesB.xh"    
#endif

#ifndef SOM_Module_ErrorDef_OpenDoc_Errors_defined
#include "ErrorDef.xh"
#endif

#else // include DTS C++ headers

#include <somobj.hh>
#include <somcls.hh>
#ifndef _DTS_HH_INCLUDED_ODTypesB
#include <ODTypesB.hh>
#endif
#ifndef _DTS_HH_INCLUDED_ErrorDef
#include <ErrorDef.hh>
#endif

#endif // ! INCL_ODDTS

#ifdef __cplusplus
extern "C" {
#endif

//=====================================================================================
// Warnings
//=====================================================================================

// WARN has the same syntax as printf but produces a SysBreakStr.
// Warnings are disabled (and generate no code) when ODDebug is off.

#define WARN  if(!ODDebug) ; else _Warn


//=====================================================================================
// Assertions
//=====================================================================================

// These all cause a debugger break if the condition evaluates to false or 0.
// Leading "W" is a Warning: it doesn't raise an exception.
// Trailing "M" means special Message displayed instead of condition.

#if ODDebug
#define ASSERT( COND, ERR ) \
  if(COND) ; else _AssertionFailed( #COND, ERR, 0, __LINE__, __FUNCTION__, __FILE__)
#define ASSERTM( COND, ERR, MSG )  \
  if(COND) ; else _AssertionFailed( #COND, ERR, MSG, __LINE__, __FUNCTION__, __FILE__)
#define WASSERT( COND )  \
  if(COND) ; else _AssertionFailed( #COND, 0, 0, __LINE__, __FUNCTION__, __FILE__)
#define WASSERTM( COND, MSG )  \
  if(COND) ; else _AssertionFailed( #COND, 0, MSG, __LINE__, __FUNCTION__, __FILE__)
#else
#define ASSERT( COND, ERR )
#define ASSERTM( COND, ERR, MSG )
#define WASSERT( COND )
#define WASSERTM( COND, MSG )
#endif

// ASSERT_NOT_NULL causes a debugger break and an exception if the parameter
// is NULL. Use this in functions that take a pointer as a parameter but do
// not allow the parameter to be NULL.
// Do **NOT** use this macro to make sure memory allocation succeeded! It
// has no effect in non-debug builds. Use THROW_IF_NULL instead.

#define ASSERT_NOT_NULL(PARAM) \
  ASSERT((PARAM)!=kODNULL,kODErrInvalidParameter)


//=====================================================================================
// Logging
//=====================================================================================

// PRINT writes to the standard output via somPrintf if ODDebug is on. Use
// SetOutputMode (or the ODDebug menu) to direct output to a file or to the
// DebugWindow app.

#define PRINT  if(!ODDebug) ; else somPrintf

// LOG is like PRINT but can easily be turned on or off on a per-file basis.
// To enable logging in a source file, you must redefine the symbol LOGGING
// as "1" in that file, otherwise LOG statements will not be compiled. Make
// sure to #undef the symbol before you re-#define it, as some compilers
// (e.g. Symantec) won't redefine an already-defined symbol.

// PRINT and LOG statements do not generate any code if logging is off.

#define LOGGING 0    // Redefine as 1 in source file to enable logging

#define LOG    if(!ODDebug || !LOGGING) ; else somPrintf


//==============================================================================
// Safe Type-Casting
//==============================================================================

/*  Use CAST as a safe way to cast a SOM object from one class to another.
  For instance, if "o" is declared as an ODObject*, but your code knows
  it's an ODPart*, you can say:
      ODPart *part = CAST(o,ODPart);
  If ODDebug is turned on, this will do a runtime check and cause an assertion
  failure if o does not point to an ODPart (or subclass). Without ODDebug,
  it degenerates into a simple C-style cast that generates no code.

  ASSERT_IS_A is similar to CAST but is used when you just want to assert
  that the pointer points to the right kind of object.
*/

#if ODDebug
  #define CAST(OBJ, CLASS)  ( (CLASS*)_Cast((OBJ),CLASS##ClassData.classObject,__LINE__, __FILE__, __FUNCTION__) )
  #define ASSERT_IS_A(OBJ,CLASS)  ( (void) CAST(OBJ,CLASS) )
#else
  #define CAST(OBJ, CLASS)  ( (CLASS*) (OBJ) )
  #define ASSERT_IS_A(OBJ,CLASS)  /* */
#endif


//=====================================================================================
// Internal goop...
//=====================================================================================

void _Warn        ( char *fmt, ... );
void _AssertionFailed  ( const char *cond,
              ODError, const char *msg, int, const char*, const char*);
#if ODDebug
SOMObject* _Cast( SOMObject *obj, SOMClass *cls, int line, const char* file, const char* function );
#endif


#ifdef __cplusplus
}
#endif

#endif /*_ODDEBUG_*/
