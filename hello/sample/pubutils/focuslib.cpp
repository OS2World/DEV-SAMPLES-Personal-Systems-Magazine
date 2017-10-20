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
/*  File:       FocusLib.cpp                                        */
/*                                                                  */
/*  Contains:   Library routines for focusing (setting up for       */
/*              drawing into a facet)                               */
/*                                                                  */
/*  Change History (most recent first):                             */
/*  138899 11/12/95 ced  Add fWindowClipRgn for DrgGetPS workaround */
/********************************************************************/

#ifndef _EXCEPT_
#include "Except.h"
#endif

#ifndef _ALTPOINT_
#include <AltPoint.h>
#endif

#ifndef _ALTPOLY_
#include <AltPoly.h>
#endif

#ifndef _ODUTILS_
#include <ODUtils.h>
#endif

#define INCL_GPI
#define INCL_WIN
#define INCL_ODAPI
#define INCL_ODCANVAS
#define INCL_ODFACET
#define INCL_ODSHAPE
#define INCL_ODTRANSFORM
#define INCL_ODWINDOW
#include <os2.h>     // [129073] - ced

#ifndef _FOCUSLIB_
#include <FocusLib.h>
#endif

#include <string.h>
#include <stdio.h>

HPS   APIENTRY DrgGetClipPS(HWND hwnd,               // Not yet in PMWIN.H
                            HWND hwndClip,
                            ULONG fl);

//------------------------------------------------------------------------------
// FocusState::BeginFocus
//------------------------------------------------------------------------------
void
FocusState::BeginFocus( Environment* ev, ODFacet *facet, ODShape* invalShape, ODBoolean setClipRgn,
                    ODBoolean toWindow, ODBoolean lockedWindowUpdate, ODBoolean dragPS)
{
    // Remember toWindow
    fToWindow = toWindow;

    // Remember the facet
    fFacet      = facet;
    fDragPS     = dragPS;
    fClipRgn    = 0;
    fEv         = ev;
    MATRIXLF    mtx;
    HRGN        hrgnOld;

   ODCanvas* canvas = facet->GetCanvas(ev);

   if (toWindow) {
       fHWND = WinWindowFromID(facet->GetWindow(ev)->GetPlatformWindow(ev), FID_CLIENT);
       ULONG ulClipFlags = 0, ulStyle;
       ulStyle = WinQueryWindowULong(fHWND, QWL_STYLE);
       if (ulStyle & WS_CLIPCHILDREN) ulClipFlags |= PSF_CLIPCHILDREN;
       if (ulStyle & WS_CLIPSIBLINGS) ulClipFlags |= PSF_CLIPSIBLINGS;
       if (ulStyle & WS_PARENTCLIP)   ulClipFlags |= PSF_PARENTCLIP;
       if (!fDragPS)
       {
          if (lockedWindowUpdate)     ulClipFlags |= PSF_LOCKWINDOWUPDATE;
          fHPS = WinGetClipPS(fHWND, 0, ulClipFlags);
       }
       else
       {
           ulClipFlags |= PSF_LOCKWINDOWUPDATE;
           fHPS = WinGetClipPS(fHWND, 0, ulClipFlags); // jp no DrgGetClipPS
       }

       ODShape* clipShape = facet->AcquireWindowFrameAggregateClipShape(ev, canvas);
       fClipRgn = clipShape->CopyRegion(ev);
       ODReleaseObject(ev, clipShape);
       ODTransform* xform = facet->AcquireWindowContentTransform(ev, canvas);
       xform->GetMATRIXLF(ev, &mtx);
       ODReleaseObject(ev, xform);
   } else {
       fHPS = facet->GetCanvas(ev)->GetPlatformCanvas(ev, kODGPI)->GetPS(ev, facet);
       ODShape* clipShape = facet->AcquireFrameAggregateClipShape(ev, canvas);
       fClipRgn = clipShape->CopyRegion(ev);
       ODReleaseObject(ev, clipShape);
       ODTransform* xform = facet->AcquireContentTransform(ev, canvas);
       xform->GetMATRIXLF(ev, &mtx);
       ODReleaseObject(ev, xform);
   }

   if (fClipRgn && invalShape) {
      ODShape* tempShape = invalShape->Copy(ev);
      ODTransform* xform = toWindow ? facet->AcquireWindowFrameTransform(ev, canvas) :
                                      facet->AcquireFrameTransform(ev, canvas);
      tempShape->Transform(ev, xform);
      GpiCombineRegion(fHPS, fClipRgn, fClipRgn, tempShape->GetRegion(ev), CRGN_AND);
      ODReleaseObject(ev, xform);
      ODReleaseObject(ev, tempShape);
   }


   GpiSavePS(fHPS);
   GpiResetPS(fHPS, GRES_ATTRS);
   GpiSetDefaultViewMatrix(fHPS, 9, &mtx, TRANSFORM_REPLACE);

   if (fClipRgn && setClipRgn)
      GpiSetClipRegion(fHPS, fClipRgn, &hrgnOld);

   /*
    * If the facets highlight mode is full highlight, then set the mix mode
    * for inverted drawing.  This type of highlighting may not be appropriate
    * for some parts.
    */
   if (facet->GetHighlight(ev) == kODFullHighlight) {
      LINEBUNDLE   lbundle;
      CHARBUNDLE   cbundle;
      MARKERBUNDLE mbundle;
      AREABUNDLE   abundle;
      IMAGEBUNDLE  ibundle;

      /*
       * Set the current default mix modes of each pritive to inverted color
       */
      lbundle.usMixMode = FM_NOTCOPYSRC;
      cbundle.usMixMode = FM_NOTCOPYSRC;
      mbundle.usMixMode = FM_NOTCOPYSRC;
      abundle.usMixMode = FM_NOTCOPYSRC;
      ibundle.usMixMode = FM_NOTCOPYSRC;
      GpiSetDefAttrs(fHPS, PRIM_LINE,   LBB_MIX_MODE, (PBUNDLE)&lbundle);
      GpiSetDefAttrs(fHPS, PRIM_CHAR,   CBB_MIX_MODE, (PBUNDLE)&cbundle);
      GpiSetDefAttrs(fHPS, PRIM_MARKER, MBB_MIX_MODE, (PBUNDLE)&mbundle);
      GpiSetDefAttrs(fHPS, PRIM_AREA,   ABB_MIX_MODE, (PBUNDLE)&abundle);
      GpiSetDefAttrs(fHPS, PRIM_IMAGE,  IBB_MIX_MODE, (PBUNDLE)&ibundle);
   }
}

//------------------------------------------------------------------------------
// FocusState::EndFocus
//------------------------------------------------------------------------------

void
FocusState::EndFocus( )
{
   GpiRestorePS(fHPS, -1);

   // Release the PS
   if (fToWindow) {
       if (fDragPS)
          DrgReleasePS(fHPS);
       else
          DrgReleasePS(fHPS); // jp problem with Drg
   } else {
       fFacet->GetCanvas(fEv)->GetPlatformCanvas(fEv, kODGPI)->ReleasePS(fEv, fFacet);
   }
}

//------------------------------------------------------------------------------
// CFocus
//------------------------------------------------------------------------------

CFocus::CFocus( Environment* ev, ODFacet *facet, ODShape* invalShape, HPS *theHPS )
{
    f.BeginFocus( ev, facet, invalShape, kODTrue, kODFalse, kODFalse, kODFalse);
    *theHPS = f.fHPS;
}

CFocus::CFocus( Environment* ev, ODFacet *facet, ODShape* invalShape, HPS *theHPS, HRGN *theClipRgn)
{
    f.BeginFocus( ev, facet, invalShape, kODFalse, kODFalse, kODFalse, kODFalse);
   *theHPS = f.fHPS;
   *theClipRgn = f.fClipRgn;
}

CFocus::~CFocus( )
{
  TRY
    f.EndFocus();
  CATCH_ALL
  ENDTRY
}

CFocusWindow::CFocusWindow( Environment* ev, ODFacet *facet, ODShape* invalShape,
                            HPS *theHPS, HWND *theHWND, TypePS typePS)
{
    f.BeginFocus( ev, facet, invalShape, kODTrue, kODTrue,
                        (typePS == LockedWindowUpdatePS), (typePS == DragPS));
   *theHPS = f.fHPS;
   if (theHWND) *theHWND = f.fHWND;
}

CFocusWindow::CFocusWindow( Environment* ev, ODFacet *facet, ODShape* invalShape,
                            HPS *theHPS, HWND *theHWND, TypePS typePS, HRGN *theClipRgn)
{
   f.BeginFocus( ev, facet, invalShape, kODFalse, kODTrue,
                        (typePS == LockedWindowUpdatePS), (typePS == DragPS));
   *theHPS = f.fHPS;
   if (theHWND) *theHWND = f.fHWND;
   *theClipRgn = f.fClipRgn;
}

CFocusWindow::~CFocusWindow( )
{
  TRY
    f.EndFocus();
  CATCH_ALL
  ENDTRY
}

//------------------------------------------------------------------------------
// BeginFocus and EndFocus
//------------------------------------------------------------------------------
void
BeginFocus( Environment* ev, FocusState* foc, ODFacet *facet, ODShape* invalShape, ODBoolean setClipRgn,
            ODBoolean toWindow, ODBoolean lockedWindowUpdate, ODBoolean dragPS)
{
   foc->BeginFocus(ev, facet, invalShape, setClipRgn,
                   toWindow, lockedWindowUpdate, dragPS);
}

void
EndFocus( FocusState* foc)
{
    foc->EndFocus();
}

