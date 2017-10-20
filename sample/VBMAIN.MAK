# VBMain.mak
# Created by IBM WorkFrame/2 MakeMake at 11:20:21 on 12 July 1996
#
# The actions included in this make file are:
#  Compile::C++ Compiler
#  Link::Linker
#  Bind::Resource Bind
#  Compile::Resource Compiler

.SUFFIXES: .CPP .RC .obj .res 

.all: \
    G:\GREG\WORK\VB_ARTCL\SAMPLE\CFUNCS.DLL \
    G:\GREG\WORK\VB_ARTCL\SAMPLE\VBMain.exe

.CPP.obj:
    @echo " Compile::C++ Compiler "
    icc.exe /I. /Tdp /Ti /Gm /Gd /Fo"%|dpfF.obj" /C %s

.RC.res:
    @echo " Compile::Resource Compiler "
    rc.exe -r %s %|dpfF.RES

G:\GREG\WORK\VB_ARTCL\SAMPLE\VBMain.exe: \
    G:\GREG\WORK\VB_ARTCL\SAMPLE\SAMPLE.obj \
    G:\GREG\WORK\VB_ARTCL\SAMPLE\VBMAIN.obj \
    G:\GREG\WORK\VB_ARTCL\SAMPLE\SAMPLE.res \
    {$(LIB)}os2386.lib \
    {$(LIB)}cfuncs.lib
    @echo " Link::Linker "
    @echo " Bind::Resource Bind "
    icc.exe @<<
     /Tdp 
     /B" /de /pmtype:pm"
     /FeG:\GREG\WORK\VB_ARTCL\SAMPLE\VBMain.exe 
     os2386.lib 
     cfuncs.lib 
     G:\GREG\WORK\VB_ARTCL\SAMPLE\SAMPLE.obj
     G:\GREG\WORK\VB_ARTCL\SAMPLE\VBMAIN.obj
<<
    rc.exe G:\GREG\WORK\VB_ARTCL\SAMPLE\SAMPLE.res G:\GREG\WORK\VB_ARTCL\SAMPLE\VBMain.exe

G:\GREG\WORK\VB_ARTCL\SAMPLE\SAMPLE.obj: \
    G:\GREG\WORK\VB_ARTCL\SAMPLE\SAMPLE.CPP \
    {G:\GREG\WORK\VB_ARTCL\SAMPLE;.;$(INCLUDE);}Sample.hpp \
    {G:\GREG\WORK\VB_ARTCL\SAMPLE;.;$(INCLUDE);}Sample.h \
    {G:\GREG\WORK\VB_ARTCL\SAMPLE;.;$(INCLUDE);}cfuncs.h

G:\GREG\WORK\VB_ARTCL\SAMPLE\VBMAIN.obj: \
    G:\GREG\WORK\VB_ARTCL\SAMPLE\VBMAIN.CPP \
    {G:\GREG\WORK\VB_ARTCL\SAMPLE;.;$(INCLUDE);}Sample.hpp \
    {G:\GREG\WORK\VB_ARTCL\SAMPLE;.;$(INCLUDE);}Sample.h

G:\GREG\WORK\VB_ARTCL\SAMPLE\SAMPLE.res: \
    G:\GREG\WORK\VB_ARTCL\SAMPLE\SAMPLE.RC \
    {$(INCLUDE)}Sample.rci \
    {$(INCLUDE)}Sample.h
