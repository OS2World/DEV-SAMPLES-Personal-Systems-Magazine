OBJS = car.obj carinit.obj
.SUFFIXES: .cpp .obj .idl .xih .xh .def .dll

all: car.dll car.lib

.cpp.obj:
  icc /c /Tdp /Gd /Gm /Sa /Ti /Su4 /Q /Ge- /Ft- /Tx $<

.idl.xih:
  sc -sxih;xc;xh $*.idl

#dependencies
car.dll : car.obj carinit.obj
  icc /Fe"car.dll" /B"/DE /NOI /ALIGN:16 /EXEPACK " $(OBJS) SOMTK1.lib SOMTK2.lib car.DEF

car.obj : car.cpp car.xih
car.xih : car.idl
car.cpp : car.idl
carinit.obj : carinit.cpp

car.lib : car.def
  implib car.lib car.def

