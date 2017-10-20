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
/*  File:    Except.cpp                                             */
/*                                                                  */
/*  Contains:  Exception-handling, assertion and debugging utilities*/
/*                                                                  */
/********************************************************************/

#include <builtin.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#ifndef __EXCEPT__
#include "Except.h"
#endif

#ifndef somapi_h
#include <somapi.h>
#endif

#ifndef _PLATFORM_OS2_PPC_
#define BREAK(msg)  _interrupt(3)
#else
#define BREAK(msg)  // How do you break into debugger on PPC???
#endif

#include "ErrStrs.h"

extern "C" {
static int _Optlink CompareErrors(const void *element1, const void* element2);
}

const char* ODGetErrStringRep(ODError error)
{
   /* Search the array for the specified error.*/
   /* Use a binary search technique since the  */
   /* array is sorted by error code.           */

   int numerrors = sizeof(ErrorStrings) / sizeof(ODErrorString);
   ODErrorString key = {error, NULL};
   ODErrorString* match = (ODErrorString*)
           bsearch(&key, ErrorStrings, numerrors, sizeof(ODErrorString),
                   CompareErrors);
   return (match ? match->str : "Unknown Error");
}

int CompareErrors(const void *element1, const void* element2)
{
   return ((ODErrorString*)element1)->err - ((ODErrorString*)element2)->err;
}



ODNativeException::ODNativeException(ODError error, const char* msg)
{
   fEv = 0;
   fError = error;
   if (!msg)
      msg = "";

   fMessage = (char*)SOMMalloc(strlen(msg)+1);
   if (fMessage)
      strcpy(fMessage, msg);
}

ODNativeException::ODNativeException(const ODNativeException& x)
{
   fEv = 0;
   fError = x.fError;
   fMessage = (char *)SOMMalloc(strlen(x.fMessage)+1);
   if (fMessage)
      strcpy(fMessage, x.fMessage);
}

ODNativeException::~ODNativeException()
{
   if (fEv) {
      ODSetSOMException(fEv, *this);
   }
   if (fMessage)
      SOMFree(fMessage);
}


//===================================================================================
// THROW, et al
//===================================================================================

static const char fmtstring[] = "%s%sException %d:%s raised by %s in %s(%d).\n";

void THROWproc( ODError error, const char* msg, int linenum, const char*functionname, const char*filename )
{
  if (linenum)
  {
     /* Append debug info to message */

     const char* errstr = ODGetErrStringRep(error);

     int strsize = sizeof(fmtstring) + strlen(functionname) + strlen(errstr) +
                   strlen(filename) + 22 + (msg ? (strlen(msg)+4) : 0);

     char* buffer = (char*)_alloca(strsize);

     if (buffer) {
        sprintf( buffer,fmtstring, msg ? msg : "", msg ? "\n\n" : "",
                 error, errstr, functionname, filename, linenum);
        msg = buffer;
     }
     somPrintf("%s",msg);
  }
  else
  {
    if( msg )
      somPrintf("** THROW(%d) called\n", error);
    else
      somPrintf("** THROW(%d,%s) called\n", error,msg);
  }
  if( getenv("ODBREAKONTHROW") )
     BREAK(msg);

  ODNativeException x(error, msg);
  throw(x);
}

void RAISEproc(Environment* ev, ODError error, const char* msg, int linenum, const char* functionname, const char* filename)
{
   if (linenum)
   {
      const char* errstr = ODGetErrStringRep(error);

      int strsize = sizeof(fmtstring) + strlen(functionname) + strlen(errstr) +
                    strlen(filename) + 22 + (msg ? (strlen(msg)+4) : 0);

      char* buffer = (char*)_alloca(strsize);

      if (buffer) {
         sprintf( buffer,fmtstring, msg ? msg : "", msg ? "\n\n" : "",
                  error, errstr, functionname, filename, linenum);

         msg = buffer;
      }
      somPrintf("%s", msg);
   }
   else
   {
     if( msg )
       somPrintf("** RAISE(%d) called\n", error);
     else
       somPrintf("** RAISE(%d,%s) called\n", error,msg);
   }
   if( getenv("ODBREAKONTHROW") )
      BREAK(msg);

   ODSetSOMException(ev, error, msg);
}


//===================================================================================
// SOM EXCEPTIONS
//===================================================================================


void
ODSetSOMException( Environment *ev, ODError error, const char *message /*=NULL*/ )
{
  if( error ) {
    ODException *x = (ODException*) SOMMalloc(sizeof(ODException));
    x->error = error;
    if( message )
      strcpy(x->message,message);
    else
      x->message[0] = '\0';
    somSetException(ev,USER_EXCEPTION,ex_ODException,x);
  } else {
    somExceptionFree(ev);
    ev->_major = NO_EXCEPTION;
  }
}


void
ODSetSOMException( Environment *ev, ODNativeException& except )
{
  ODSetSOMException(ev,except.error(), except.message());
}


ODError
ODGetSOMException( Environment *ev )
{
  if( ev->_major ) {
    const char *excpName = somExceptionId(ev);
    if( strcmp(excpName,ex_ODException) == 0 ) {
      ODException *x = (ODException*)somExceptionValue(ev);
      return x->error;
    } else {
      somPrintf("Env has non-OpenDoc err: %s",excpName);
      return kODErrSOMException;
    }
  } else
    return kODNoError;
}


void CHECK_ENV( Environment *ev )
{
  if( ev->_major ) {
    const char *excpName = somExceptionId(ev);
    if( strcmp(excpName,ex_ODException) == 0 ) {
      ODException x = *(ODException*)somExceptionValue(ev);
      somExceptionFree(ev);
      ev->_major = NO_EXCEPTION;
      ODNativeException ex(x.error, x.message[0] ? x.message : kODNULL);
      throw(ex);
    } else {
      somPrintf("Env has non-OpenDoc err: %s",excpName);
      somExceptionFree(ev);
      ev->_major = NO_EXCEPTION;
      ODNativeException ex(kODErrSOMException, "Unknown SOM exception");
      throw(ex);
    }
  }
}
