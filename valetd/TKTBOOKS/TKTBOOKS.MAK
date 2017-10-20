OBJS = tktbooks.obj tktbinit.obj
.SUFFIXES: .cpp .obj .idl .xih .xh .def .dll

all: tktbooks.dll tktbooks.lib

.cpp.obj:
  icc /c /Tdp /Gd /Gm /Sa /Ti /Su4 /Q /Ge- /Ft- /Tx $<

.idl.xih:
  sc -sxih;xc;xh $*.idl

#dependencies
tktbooks.dll : tktbooks.obj tktbinit.obj
  icc /Fe"tktbooks.dll" /B"/DE /NOI /ALIGN:16 /EXEPACK " $(OBJS) GARAGE.LIB TICKET.LIB SOMTK1.LIB tktbooks.DEF

tktbooks.obj : tktbooks.cpp tktbooks.xih
tktbooks.xih : tktbooks.idl
tktbooks.cpp : tktbooks.idl
tktbinit.obj : tktbinit.cpp

tktbooks.lib : tktbooks.def
  implib tktbooks.lib tktbooks.def

