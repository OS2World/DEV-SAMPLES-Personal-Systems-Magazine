#ifndef  _STORUTIL_
#define  _STORUTIL_

#ifndef  SOM_ODDraft_xh
#include <Draft.xh>
#endif

#ifndef  _ODTYPES_
#include <ODTypes.h>
#endif

// #ifndef  _ODMEMORY_
// #include <ODMemory.h>
// #endif

//#ifndef  _PLFMFILE_
//#include <PlfmFile.h>
//#endif

#ifndef  SOM_ODPart_xh
#include <Part.xh>
#endif

class ODContainer;
class ODSession;
class ODStorageUnitView;
class ODStorageUnit;

   void           ODSUAddPropValue(Environment *ev, ODStorageUnit *su, ODPropertyName prop, ODValueType val);
   void           ODSUForceFocus(Environment *ev, ODStorageUnit *su, ODPropertyName prop, ODValueType val);
   ODBoolean      ODSUExistsThenFocus(Environment *ev, ODStorageUnit *su, ODPropertyName prop, ODValueType val);
   void           ODSURemoveProperty(Environment *ev, ODStorageUnit *su, ODPropertyName prop);

   ODBoolean      FW_SUExistsThenFocus(Environment *ev,ODStorageUnit *su,ODPropertyName prop, ODValueType val);
   void           FW_SUForceFocus(Environment *ev,ODStorageUnit *su,ODPropertyName prop,ODValueType val);
   void           FW_SUAddPropValue(Environment *ev,ODStorageUnit *su,ODPropertyName prop, ODValueType val);
   void           FW_SURemoveProperty(Environment *ev,ODStorageUnit *su,ODPropertyName prop);

//   PlatformFile*  GetPlatformFileFromContainer(Environment *ev, ODContainer *container);
//   ODFileSpec     GetODFileSpecFromContainer(Environment *ev, ODContainer *container);
//   ODContainer*   CreateFileContainer(Environment *ev, ODSession *session, FSSpec *fsSpec);
//   ODContainer*   GetFileContainer(Environment *ev, ODSession *session, FSSpec *fsSpec);
//   ODContainer*   CreateMemoryContainer(Environment *ev, ODSession *session, ODHandle handle, ODContainerType containerType);
//   ODContainer*   GetMemoryContainer(Environment *ev, ODSession *session, ODHandle handle, ODContainerType containerType);
//   void           StorageUnitSetPromiseValue(ODStorageUnit *su, Environment *ev, ODValueType value,
//                                                ODULong offset, ODULong size, const void *buffer, ODPart *sourcePart);
//   ODBoolean      StorageUnitGetStylFromStyledText(ODStorageUnit *su, Environment *ev, ODULong *size, ODPtr *styl);
//   ODCloneKind    GetOriginalCloneKind(Environment *ev, ODDraft *draft);
//   void           RemoveDataInterchangeProperties(Environment *ev, ODStorageUnit *su, ODBoolean clonedToFileContainer);
#endif                       // _STORUTIL_
