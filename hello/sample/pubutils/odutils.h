/**********************************************************************/
/*  Licensed Materials - Property of IBM                              */
/*                                                                    */
/*                                                                    */
/* Copyright (C) International Business Machines Corp., 1994.         */
/* Copyright (C) Apple Computer, Inc., 1994                           */
/*                                                                    */
/*  US Government Users Restricted Rights -                           */
/*  Use, duplication, or disclosure restricted                        */
/*  by GSA ADP Schedule Contract with IBM Corp.                       */
/*                                                                    */
/*  File:     ODUtils.h                                               */
/*                                                                    */
/*  Contains: Object utility functions                                */
/*                                                                    */
/*  Theory Of Operation:                                              */
/*                                                                    */
/*  These are useful utilities for use with objects, particularly     */
/*  counted objects. Some are functions rather than macros, to reduce */
/*  code size, and can be made inline later if necessary.             */
/*                                                                    */
/*  ODDeleteObject deletes an object (which doesn't need to           */
/*  need to be a SOM object) and sets the variable pointing to it to  */
/*  NULL.                                                             */
/*                                                                    */
/*  ODReleaseObject is similar, but releases the object instead of    */
/*  deleting it.  Use this one for ref-counted objects.               */
/*                                                                    */
/*  ODFinalReleaseObject is similar to ODReleaseObject, except that   */
/*  it is meant to be used in code which is releasing the last        */
/*  reference to that object.  I.E.  it does an ASSERT(refcount == 1) */
/*  before calling release.                                           */
/*                                                                    */
/*  ODAcquireObject bumps the ref-count of an object, unless it's     */
/*  NULL.                                                             */
/*                                                                    */
/*  ODSafeReleaseObject also releases an object, but requires no      */
/*  Environment parameter and can't throw an exception.  It's for use */
/*  _only_ in somUninit methods, where no Environment is available.   */
/*                                                                    */
/*  ODTransferReference lowers one object's refCount while            */
/*  incrementing another's.  It's useful in things like setters where */
/*  you want to forget about one object and remember another.  It     */
/*  does the right thing if either pointer is NULL or if they point   */
/*  to the same object.  ** This function may throw an exception in   */
/*  the unlikely case that the Acquire or Release call fails.         */
/*                                                                    */
/*  ODCopyAndRelease returns a copy of an object while releasing the  */
/*  object.  This is useful when transferring control of an object to */
/*  the caller, but the caller has permission to modify the object.   */
/*  It's optimized for the case where the ref-count of the object is  */
/*  1, in which case it just returns the original object since no     */
/*  copying is needed.  (This function works only on ODShapes and     */
/*  ODTransforms, which are the only classes that provide a Copy      */
/*  method.)  ** This function may throw an exception in the unlikely */
/*  case that the GetRefCount or Release call fails.                  */
/*                                                                    */
/*                                                                    */
/*  The "do...while(0)" blocks wrapped around the macros are there to */
/*  prevent syntactic problems if a macro call is immediately         */
/*  followed by an "else" statement, to keep the "else" from binding  */
/*  to the "if" in the macro body.  The "while" loop is optimized out */
/*  by the compiler and has no effect on the flow of control or code  */
/*  quality.                                                          */
/*                                                                    */
/**********************************************************************/

#ifndef _ODUTILS_
#define _ODUTILS_

#ifndef _ODTYPES_
#include "ODTypes.h"
#endif


struct Environment;
class ODObject;
class ODShape;
class ODTransform;
class ODRefCntObject;


#define  ODDeleteObject(object)  \
   do{                     \
      if (object!=kODNULL) {  \
         delete object;    \
         object = kODNULL; \
      }                 \
   }while(0)

#ifndef INCL_ODDTS 

#define  ODReleaseObject(ev, object) \
   do{                        \
      if (object!=kODNULL) {     \
         object->Release(ev); \
         object = kODNULL;    \
      }                    \
   }while(0)

#define  ODFinalReleaseObject(ev, object) \
   do{                        \
      if (object!=kODNULL) {     \
         WASSERT(object->GetRefCount(ev) == 1); \
         object->Release(ev); \
         object = kODNULL;    \
      }                    \
   }while(0)

#else

#define  ODReleaseObject(object) \
   do{                        \
      if (object!=kODNULL) {     \
         object->Release(); \
         object = kODNULL;    \
      }                    \
   }while(0)

#define  ODFinalReleaseObject(object) \
   do{                        \
      if (object!=kODNULL) {     \
         WASSERT(object->GetRefCount() == 1); \
         object->Release(); \
         object = kODNULL;    \
      }                    \
   }while(0)

#endif // ! INCL_ODDTS

void ODAcquireObject(Environment* ev, ODRefCntObject* object);

void     ODSafeReleaseObject( ODRefCntObject* );   // Needs no ev, throws no exceptions

void     ODTransferReference( Environment*, ODRefCntObject *oldObj,
                                    ODRefCntObject *newObj );

ODShape* ODCopyAndRelease( Environment*, ODShape* );

ODTransform*ODCopyAndRelease( Environment*, ODTransform* );

ODBoolean ODObjectsAreEqual(Environment* ev, ODObject* a, ODObject* b);

#endif //_ODUTILS_
