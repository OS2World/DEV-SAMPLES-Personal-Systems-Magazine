OBJS = client.obj 
.SUFFIXES: .cpp .obj .idl .xih .xh .def .dll

all: client.exe

.cpp.obj:
  icc /c /Tdp /Gd /Gm /Sa /Ti /Su4 /Q /Ft- /Tx $<

#dependencies
client.exe : client.obj 
  icc /Fe"client.exe" /B"/DE /NOI /ALIGN:16 /EXEPACK " $(OBJS) valet.lib ticket.lib SOMTK.LIB client.def


