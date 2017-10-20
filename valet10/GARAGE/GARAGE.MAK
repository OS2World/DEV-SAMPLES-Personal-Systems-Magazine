OBJS = garage.obj garinit.obj
.SUFFIXES: .cpp .obj .idl .xih .xh .def .dll

all: garage.dll garage.lib

.cpp.obj:
  icc /c /Tdp /Gd /Gm /Sa /Ti /Su4 /Q /Ge- /Ft- /Tx $<

.idl.xih:
  sc -sxih;xc;xh $*.idl

#dependencies
garage.dll : garage.obj garinit.obj
  icc /Fe"garage.dll" /B"/DE /NOI /ALIGN:16 /EXEPACK " $(OBJS) SOMTK.LIB TICKET.LIB CAR.LIB garage.DEF

garage.obj : garage.cpp garage.xih
garage.xih : garage.idl
garage.cpp : garage.idl
garinit.obj : garinit.cpp

garage.lib : garage.def
  implib garage.lib garage.def

