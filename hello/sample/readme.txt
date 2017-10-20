Hello Sample from Personal Systems Magazine

This sample differs from the code reviewed in the magazine article
due to changes from OpenDoc on the Developer Connection 8 CD-ROM
and the Developer Connection 9 SE CD-ROM.  The part will now function 
correctly on Developer Connection 9 SE.

The source consists of a directory for the Hello part and a new PUBUTILS
directory (from \IBMCPP\BETA\SAMPLES\OPENDOC\PUBUTILS) that contains
additional support classes and a simple dump/trace class that uses Mike
Cowlishaws PMPRINTF utility.

The hello source part can be compiled with possibly minor changes to paths
in the make file.  It relies on the PUBUTILS.LIB file from the PUBUTILS 
directory, which should be placed where the linker can find it.  The part
is documented with notes from the OpenDoc programmers guide that outlines
the correct functions to perform in each ODPart method.

HELOINST.CMD will install the part and create a template in the OpenDoc
Templates directory.  If trace output is desired, simply start PMPRINTF.EXE.
The template can then be dragged to an OpenDoc Container part to create
a new instance.

I will try to provide additional documentation and support classes as time 
is found.  I hope this is helpful.  

John Pape
