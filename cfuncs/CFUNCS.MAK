# cfuncs.mak
# Created by IBM WorkFrame/2 MakeMake at 9:45:57 on 14 Feb 1996
#
# The actions included in this make file are:
#  Compile::C++ Compiler
#  Link::Linker
#  Lib::Import Lib

.SUFFIXES: .C .LIB .dll .obj 

.all: \
    g:\greg\work\ti96\sample\cfuncs\cfuncs.LIB

.C.obj:
    @echo " Compile::C++ Compiler "
    icc.exe /DSTATIC_LINK /Ss /Ti /Ge- /Fo"%|dpfF.obj" /C %s

.dll.LIB:
    @echo " Lib::Import Lib "
    implib.exe %|dpfF.LIB %s

g:\greg\work\ti96\sample\cfuncs\cfuncs.dll: \
    g:\greg\work\ti96\sample\cfuncs\CFuncs.obj \
    g:\greg\work\ti96\sample\cfuncs\cfuncs.def
    @echo " Link::Linker "
    icc.exe @<<
     /B" /de /noe"
     /Feg:\greg\work\ti96\sample\cfuncs\cfuncs.dll
     g:\greg\work\ti96\sample\cfuncs\cfuncs.def
     g:\greg\work\ti96\sample\cfuncs\CFuncs.obj
<<

g:\greg\work\ti96\sample\cfuncs\CFuncs.obj: \
    g:\greg\work\ti96\sample\cfuncs\CFuncs.C \
    {g:\greg\work\ti96\sample\cfuncs;$(INCLUDE);}CFuncs.h

g:\greg\work\ti96\sample\cfuncs\cfuncs.LIB: \
    g:\greg\work\ti96\sample\cfuncs\cfuncs.dll
