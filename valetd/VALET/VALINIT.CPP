#include "valet.xih"

#ifdef __IBMC__
  #pragma linkage(SOMInitModule,system)
#endif

SOMEXTERN void SOMLINK SOMInitModule(long majorV,
                                     long minorV,
                                     string className)
{
   SOM_IgnoreWarning(majorV);
   SOM_IgnoreWarning(minorV);
   SOM_IgnoreWarning(className);

   ValetNewClass(Valet_MajorVersion, Valet_MinorVersion);
}
