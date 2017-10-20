#***********************************************************************
#
#  File Name  :  PUBUTILS.MAK
#
#  Description:  Makefile for the OpenDoc Public Utilities Library.
#
#  Notes      :  This makefile should be run in this sample's subdirectory.
#
#                Regarding the DEBUG macro statements:
#                The file is shipped so that the build will not produce
#                debug information ("DEBUG = /Ti-" is uncommented and
#                "DEBUG = /T+" is commented out).  If you prefer to build
#                with debug information, comment out the first DEBUG macro
#                and uncomment the second one.
#
#      (C) COPYRIGHT International Business Machines Corp. 1995
#      All Rights Reserved
#      Licensed Materials - Property of IBM
#
#      US Government Users Restricted Rights - Use, duplication or
#      disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
#
#      DISCLAIMER OF WARRANTIES.  The following [enclosed] code is
#      sample code created by IBM Corporation. This sample code is not
#      part of any standard or IBM product and is provided to you solely
#      for the purpose of assisting you in the development of your
#      applications.  The code is provided "AS IS", without
#      warranty of any kind.  IBM shall not be liable for any damages
#      arising out of your use of the sample code, even if they have
#      been advised of the possibility of such damages.
#
#***********************************************************************

#===========================================================================
#  MACROS DEFINITION
#===========================================================================

FILENAME = pubutils

# DEBUG = /Ti-
DEBUG = /Ti+

#---------------------------------------------------------------------------
# OBJLIST is the list of .OBJ files that will be linked with the .LIB file.
#---------------------------------------------------------------------------
OBJLIST = altpoint.obj altpoly.obj except.obj focuslib.obj \
          lineops.obj oddebug.obj odutils.obj tempobj.obj ordcoll.obj \
          mlogimpl.obj storutil.obj

#===========================================================================
#  PSEUDOTARGETS:
#===========================================================================

.all: $(FILENAME).lib

.SUFFIXES: .cpp .obj

.cpp.obj:
      @echo WF::COMPILE::C Set ++ Compile
      icc.exe /Gm+ /Ge- /Tl- /Fd /Gd+ /Sp4 /Q $(DEBUG) /N30 /C %s

altpoint.obj: altpoint.cpp altpoint.h except.h oddebug.h lineops.h

altpoly.obj: altpoly.cpp altpoint.h altpoly.h except.h oddebug.h lineops.h

except.obj: except.cpp except.h errstrs.h

focuslib.obj: focuslib.cpp focuslib.h

lineops.obj: lineops.cpp lineops.h altpoint.h except.h oddebug.h

oddebug.obj: oddebug.cpp oddebug.h except.h

odutils.obj: odutils.cpp odutils.h except.h oddebug.h

tempobj.obj: tempobj.cpp tempobj.h oddebug.h

ordcoll.obj: ordcoll.cpp ordcoll.h

storutil.obj: storutil.cpp storutil.h

mlogimpl.obj: mlogimpl.cpp mlogimpl.h

#-----------------------------------------------------------------------------
# The following rule builds the file that contains the error code string
# representation in sort order.
#-----------------------------------------------------------------------------
errstrs.h: ..\..\..\..\h\errordef.xh
    @echo /**/                       > $(TMP)\parseerr.cmd
    @echo parse upper arg errfile .   >>$(TMP)\parseerr.cmd
    @echo do while lines(errfile) ^> 0 >>$(TMP)\parseerr.cmd
    @echo   line = linein(errfile)     >>$(TMP)\parseerr.cmd
    @echo   parse var line a b c .     >>$(TMP)\parseerr.cmd
    @echo   if (a = '#define' ^& substr(b,1,6) = 'kODErr') then   >>$(TMP)\parseerr.cmd
    @echo     call lineout , '{'^|^|format(c,6)^|^|', "'||b||'"},'>>$(TMP)\parseerr.cmd
    @echo end                                             >>$(TMP)\parseerr.cmd
    @echo /*                                              > $@
    @echo  * This is a machine generated file.  The routine     >>$@
    @echo  * that uses the array declared in this file requires >>$@
    @echo  * that the array members be maintained in sort order >>$@
    @echo  * by error code.                                     >>$@
    @echo  */                                                   >>$@
    @echo struct ODErrorString {                                >>$@
    @echo    ODError err;                                       >>$@
    @echo    const char* str;                                   >>$@
    @echo };                                                    >>$@
    @echo static const ODErrorString ErrorStrings[] = {         >>$@
    $(TMP)\parseerr   $** | sort                                >>$@
    @echo };                                                    >>$@
    @del $(TMP)\parseerr.cmd

$(FILENAME).lib: $(OBJLIST)
      !libv2r1 $@ -+$?;
      copy pubutils.lib d:\ibmcpp\lib\.
