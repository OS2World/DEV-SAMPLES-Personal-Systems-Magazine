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
/*  IBM Change History (most recent first):                         */
/*  138899 11/12/95 ced  Add fWindowClipRgn for DrgGetPS workaround */
/*  124371  6/02/95 ced  Added support for drag PS in CFocusWindow  */
/*  120604  4/28/95 ced  Merge with B1C16 Apple code.               */
/*                                                                  */
/********************************************************************/
/*
  File:    FocusLib.h

  THEORY OF OPERATION:

    FocusLib sets up the drawing environment for a part so that
    it can start drawing into a facet. It provides both a one-time change of
    focus (via the Focus call) and a way to set the focus but restore it later
    when the current scope exits (via the CFocus object.)

    Focus() and FocusWindow() are slightly different for offscreen facets. If the
    facet is in an offscreen canvas, Focus will set up to draw into that canvas;
    this is what one usually wants to do. On the other hand, FocusWindow will
    always focus onto an on-screen canvas, even for an off-screen facet. Use
    FocusWindow for things like rubber-banding where your drawing needs to show
    up immediately, even if the facet is offscreen.

    The CFocus object deserves an example:

    void DrawSomething( ODFacet *facet )
    {
      CFocus f(facet);    // Creates a CFocus object on the stack, which
                          // changes the focus but stores the old state
      ....drawing code...
    }

    Declaring the CFocus object calls its constructor, which sets the focus and
    stores the old state in the CFocus object itself (on the stack.) When its
    scope exists, the CFocus object's destructor is called, which restores the
    old focus.

*/


#ifndef _FOCUSLIB_
#define _FOCUSLIB_

#ifndef __OS2DEF__
#include <os2def.h>
#endif

#ifndef INCL_ODDTS // include non-DTS C++ headers

#ifndef SOM_Module_OpenDoc_Global_TypesB_defined
#include "ODTypesB.xh"    
#endif

#else // include DTS C++ headers

#ifndef SOM_HH_DTS_Included
#include <som.hh>
#endif
#ifndef _DTS_HH_INCLUDED_ODTypesB
#include <ODTypesB.hh>
#endif

#endif // ! INCL_ODDTS

#ifdef __cplusplus

class ODCanvas;
class ODShape;
class ODFacet;
struct Environment;
#else
  #ifndef som_h
  #include <som.h>
  #endif
  #ifndef SOM_ODCanvas_h
  #include <Canvas.h>
  #endif
  #ifndef SOM_ODShape_h
  #include <Shape.h>
  #endif
  #ifndef SOM_ODFacet_h
  #include <Facet.h>
  #endif
#endif

/*  FocusState stores the state data for QD focusing.
  C users should allocate one on the stack and call BeginFocus and
  EndFocus (q.v.) to do the focusing.
  C++ users should ignore FocusState and simply allocate a CFocus object
  (see below).
*/

struct FocusState
{
  HPS          fHPS;
  HWND         fHWND;
  HWND         fToWindow;
  HRGN         fClipRgn;
  ODFacet*     fFacet;
  ODBoolean    fDragPS;
  Environment* fEv;

#ifdef __cplusplus
  void    BeginFocus( Environment* ev, ODFacet *facet, ODShape* invalShape, ODBoolean setClipRgn,
                    ODBoolean toWindow, ODBoolean lockedWindowUpdate, ODBoolean dragPS);
  void    EndFocus( );
#endif
};
typedef struct FocusState FocusState;


/*  CFocus is a class for C++ users. Just allocate one as a local variable:
  the constructor will focus, and the destructor (called when it goes out
  of scope or an exception is thrown) will unfocus.
  CFocusWindow is just like CFocus, but focuses to the window instead of
  the facet's canvas (if they're different.)
*/

#ifdef __cplusplus

class CFocus {
  public:
  CFocus( Environment* ev, ODFacet *facet, ODShape* invalShape, HPS *theHPS );
  CFocus( Environment* ev, ODFacet *facet, ODShape* invalShape, HPS *theHPS, HRGN* theClipRgn );
  virtual ~CFocus();

protected:
  FocusState f;
};

class CFocusWindow {
  public:
  enum TypePS {
    NormalPS,
    DragPS,
    LockedWindowUpdatePS
  };
    
  CFocusWindow( Environment* ev, ODFacet *facet, ODShape* invalShape,
                            HPS *theHPS, HWND *theHWND = kODNULL, TypePS typePS = NormalPS);

  CFocusWindow( Environment* ev, ODFacet *facet, ODShape* invalShape,
                HPS *theHPS, HWND *theHWND, TypePS typePS, HRGN *theClipRgn);
  virtual ~CFocusWindow();
  
  protected:
  FocusState f;
};
#endif


#ifdef __cplusplus
extern "C" {
#endif

// For C only:
void BeginFocus( Environment* ev, FocusState* foc, ODFacet *facet, ODShape* invalShape, ODBoolean setClipRgn,
         ODBoolean toWindow, ODBoolean lockedWindowUpdate, ODBoolean dragPS);

void EndFocus( FocusState* );

#ifdef __cplusplus
}
#endif

#endif //_FOCUSLIB_
