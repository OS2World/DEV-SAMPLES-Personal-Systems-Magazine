/*************************************************************************/
/*  Licensed Materials - Property of IBM                                 */
/*                                                                       */
/*                                                                       */
/* Copyright (C) International Business Machines Corp., 1994.            */
/* Copyright (C) Apple Computer, Inc., 1994                              */
/*                                                                       */
/*  US Government Users Restricted Rights -                              */
/*  Use, duplication, or disclosure restricted                           */
/*  by GSA ADP Schedule Contract with IBM Corp.                          */
/*                                                                       */
/*  File:    ODDebug.cpp                                                 */
/*                                                                       */
/*  Contains:  Useful debugging macros and functions.                    */
/*                                                                       */
/*  133493  08/18/95  ced Use SOM_WarnMsg instead of somPrintf in _Warn  */
/*                                                                       */
/*************************************************************************/

#include <builtin.h>

#ifndef _ODDEBUG_
#include "ODDebug.h"
#endif

#ifndef _EXCEPT_
#include "Except.h"
#endif

#include <stdio.h>
#include <stdarg.h>
#include <string.h>


//=====================================================================================
// BREAK() 
//=====================================================================================
#ifndef _PLATFORM_OS2_PPC_
#define BREAK(msg) _interrupt(3);
#else
#define BREAK(msg) // How do you break into debugger on PPC???
#endif

//===================================================================================
// ASSERTION-FAILED
//===================================================================================

#if ODDebug

void _AssertionFailed( const char *cond, ODError error, const char *msg, 
                       int linenum, const char* function, const char* file )
{
  char dbg[512];
  if (error)
  {
     sprintf(dbg,"%s: %s ...NOT!\n", msg ? msg : "Assertion failed",cond);
     THROWproc(error, dbg, linenum, function, file);  // THROWproc prints message
  } 
  else
     somPrintf(dbg, "%s: %s ...NOT! in %s - %s(%d)\n", msg ? msg : "Assertion failed",
               cond, function, file, linenum);
}

#else

void _AssertionFailed( const char *cond, ODError error, const char *msg, 
                       int linenum, const char* function, const char* file )
{
}

#endif /*ODDebug*/


//===================================================================================
// WARN
//===================================================================================

#if ODDebug

void _Warn( char *fmt, ... )
{
  char msg[512];
  strcpy(msg, "ODWarning: ");
  va_list args;
  va_start(args,fmt);
  vsprintf(msg+strlen(msg),fmt,args);
  va_end(args);
  SOM_WarnMsg(msg);                    // [133493] - ced
  if (getenv("ODBREAKONWARN"))
     BREAK(msg);
}

#else

extern "C" void _Warn(  char *fmt, ... );
void _Warn(  char *fmt, ... )
{
}

#endif /*ODDebug*/

//==============================================================================
// SAFE CAST
//==============================================================================


#if ODDebug

SOMObject*
_Cast( SOMObject *obj, SOMClass *cls, int line, const char* file, const char* function )
{
  char dbg[256];
  if( !somIsObj(obj) )
    sprintf(dbg, "Can't cast: %p is not a SOM object",obj);
  else if( !somIsObj(cls) )
    sprintf(dbg, "Can't cast: %p is not a SOM class",cls);
  else if( !obj->somIsA(cls) )
    sprintf(dbg, "Can't cast: %p is an %s, not an %s",obj, obj->somGetClassName(), cls->somGetName());
  else
    return obj;
  THROWproc(kODErrAssertionFailed, dbg, line, file, function);
  return NULL; /* keeps compiler quiet */
}

#else

extern "C" void _Cast( );
void _Cast( )
{
}

#endif /*ODDebug*/
