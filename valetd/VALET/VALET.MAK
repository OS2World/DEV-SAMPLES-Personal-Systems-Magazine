OBJS = valet.obj valinit.obj
.SUFFIXES: .cpp .obj .idl .xih .xh .def .dll

all: valet.dll valet.lib

.cpp.obj:
  icc /c /Tdp /Gd /Gm /Sa /Ti /Su4 /Q /Ge- /Ft- /Tx $<

.idl.xih:
  sc -sxih;xc;xh $*.idl

#dependencies
valet.dll : valet.obj valinit.obj
  icc /Fe"valet.dll" /B"/DE /NOI /ALIGN:16 /EXEPACK " $(OBJS) SOMTK1.LIB GARAGE.LIB TICKET.LIB TKTBOOKS.LIB VALET.DEF

valet.obj : valet.cpp valet.xih
valet.xih : valet.idl
valet.cpp : valet.idl
valinit.obj : valinit.cpp

valet.lib : valet.def
  implib valet.lib valet.def

