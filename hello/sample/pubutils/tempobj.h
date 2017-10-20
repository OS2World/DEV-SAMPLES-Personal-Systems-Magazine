/*
  File:    TempObj.h

  Contains:  Template utilities for exception-safe temporary object references

  Theory Of Operation:

    *** See the Tech Note "Temporary References/Objects" for full documentation.


    TempObj<T>    T :ODObject        A pointer to a temporary object
    TempRef<T>    T :ODRefCntObject    A pointer to a temporary reference

    Specific instantiations:
    TempRefs:
      TempODPart
      TempODFrame
      TempODShape
      TempODTransform
      TempODStorageUnit
    TempObjs:
      TempODFrameFacetIterator
      TempFocusSetIterator
    (you can extend this set.)

    These are simple template classes that act as a transparent wrapper around
    an OpenDoc object pointer. The temp object can be used wherever a pointer
    to the object would be used. When the temp object goes out of scope, the
    object it wraps will be deleted (in the case of TempObj) or released (in
    the case of TempRef.) This includes the case where an exception is thrown:
    the wrapper is exception-safe.

    Example:

      RgnHandle r;
      {
        TempODShape s = frame->GetUsedShape(ev,kODNULL);
        r = s->GetQDRegion(ev);
      }

    In this example, s is wrapped around the actual ODShape returned by
    the GetUsedShape method. It is used just as an ODShape* in the next
    line. In the normal course of events, the shape will be released after
    GetQDRegion returns, since s goes out of scope. If GetUsedShape runs
    out of memory and throws an exception, the shape will also be safely
    released.

    There is a certain amount of overhead associated with the TempRef
    object. However, the only safe alternative to using it would be to
    wrap an exception handler around the call to GetUsedShape and release
    the shape in the CATCH block (and then reraise the exception) as well
    as after the ENDTRY. This ends up using significantly more code and
    execution time than the TempRef object -- exception handlers are
    quite expensive.

  In Progress:

*/

#ifndef _TEMPOBJ_
#define _TEMPOBJ_

#ifndef _EXCEPT_
#include <Except.h>
#endif

#ifndef INCL_ODDTS // include non-DTS C++ headers

#ifndef SOM_ODRefCntObject_xh
#include "RefCtObj.xh"
#endif

#else // include DTS C++ headers

#ifndef _DTS_HH_INCLUDED_RefCtObj
#include <RefCtObj.hh>
#endif

#endif // ! INCL_ODDTS

class TempPtr
{
  public:
  TempPtr( );
  TempPtr( void *block );
  ~TempPtr( );
  operator void* ( )        {return fBlock;}
  void* operator= ( void *b )    {return (fBlock = b);}

  protected:
  void* fBlock;
};


class BaseTempObj
{
  public:
  virtual ~BaseTempObj();

  protected:
  ODObject *fObj;
};


class BaseTempRef
{
  public:
  void Release();
  virtual ~BaseTempRef();

  protected:
  ODRefCntObject *fObj;
};


//===========================================================================
//  TempObj <T>
//===========================================================================


template<class T> class TempObj :public BaseTempObj
{
  public:
  TempObj( T* );
  T* operator-> ()    {return (T*)fObj;}
  operator T* ()      {return (T*)fObj;}

  T* operator= (T* t)    {fObj=t; return t;}

  T* DontDelete()      {T* temp=(T*)fObj; fObj=kODNULL; return temp;}
};

// Implementation of the TempObj constructor:
template<class T> TempObj<T>::TempObj( T *obj )
{
  fObj = obj;
}


//===========================================================================
//  TempRef <T>
//
//  Supports a few extra goodies:
//    Release()    Releases the object and sets the pointer to NULL.
//    DontRelease()  Sets the pointer to NULL (so the destructor will not
//              release the object) but returns the old pointer.
//
//===========================================================================


template<class T> class TempRef :public BaseTempRef
{
  public:
  TempRef( T* );
  T* operator-> ()    {return (T*)fObj;}
  operator T* ()      {return (T*)fObj;}
  T* operator=( T *t )  {fObj=t; return t;}

  T* DontRelease()    {T* temp=(T*)fObj; fObj=kODNULL; return temp;}
};


// Implementation of the TempRef constructor:
template<class T> TempRef<T>::TempRef( T *obj )
{
  fObj = obj;
}

//===========================================================================
//  Instantiations of TempObj and TempRef. Add your own if necessary.
//===========================================================================

class ODFrameFacetIterator;
class ODFocusSetIterator;
class ODFrame;
class ODPart;
class ODShape;
class ODTransform;
class ODStorageUnit;
class ODWindow;

typedef TempObj<ODFrameFacetIterator>  TempODFrameFacetIterator;
typedef TempObj<ODFocusSetIterator>    TempODFocusSetIterator;

typedef TempRef<ODFrame>        TempODFrame;
typedef TempRef<ODPart>          TempODPart;
typedef TempRef<ODShape>        TempODShape;
typedef TempRef<ODTransform>      TempODTransform;
typedef TempRef<ODStorageUnit>      TempODStorageUnit;
typedef TempRef<ODWindow>        TempODWindow;

#endif /*_TEMPOBJ_*/
