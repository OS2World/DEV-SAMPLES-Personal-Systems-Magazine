PUBLIC       = d:\ibmcpp\beta\samples\opendoc\parts\public
UTILS        = d:\ibmcpp\beta\samples\opendoc\pubutils
DLLFLAGS     = /Gm+ /Ge- /Gd+ /Sp4 /Q /D_OD_ /C /Ti

.all: idl hello2.dll

idl:  hello.xh

.SUFFIXES: .idl .xh .cpp .rc .c .lib .dll

.idl.xh:
       sc -D _PLATFORM_OS2_ -I. -I$(PUBLIC) -I$(UTILS) "-sxh;xih;xc" -p $(@B).idl
       copy $(@B).idl $(PUBLIC)
       copy $(@B).xh  $(PUBLIC)

hello.obj: hello.cpp
       icc.exe $(DLLFLAGS) /I$(PUBLIC) /I$(UTILS) %s

hello2.dll: hello.obj makefile hello.def
       icc.exe @<<
       /Ge- /Gd+ /Sp4 /Q /Tdp /Ti
       /B"/DE /PMTYPE:PM /NOD /NOI /NOE /NOP /NOPACKD /DLL"
       /Fehello2.dll
       /Fmhello.map
       .\hello.obj
       $(UTILS)\printf.obj
       pubutils.lib
       os2cmbsi.lib
       os2386.lib
       somtk.lib
       opendoc.lib
       hello.def
<<
       copy hello2.dll d:\ibmcpp\dll
       copy hello.cpp d:\ibmcpp\dll
