OBJS = ticket.obj tktinit.obj
.SUFFIXES: .cpp .obj .idl .xih .xh .def .dll

all: ticket.dll ticket.lib

.cpp.obj:
  icc /c /Tdp /Gd /Gm /Sa /Ti /Su4 /Q /Ge- /Ft- /Tx $<

.idl.xih:
  sc -sxih;xc;xh $*.idl

#dependencies
ticket.dll : ticket.obj tktinit.obj
  icc /Fe"ticket.dll" /B"/DE /NOI /ALIGN:16 /EXEPACK " $(OBJS) SOMTK.LIB ticket.DEF

ticket.obj : ticket.cpp ticket.xih
ticket.xih : ticket.idl
ticket.cpp : ticket.idl
tktinit.obj : tktinit.cpp

ticket.lib : ticket.def
  implib ticket.lib ticket.def

