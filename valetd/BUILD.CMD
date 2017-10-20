call cleanall.cmd
cd car
nmake car.mak
cd..\ticket
nmake ticket.mak
cd..\garage
nmake garage.mak
cd..\tktbooks
nmake tktbooks.mak
cd..\valet
nmake valet.mak
cd..\client
nmake client.mak
cd..
getdlls.cmd

