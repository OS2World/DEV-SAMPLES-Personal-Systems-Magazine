/*
        File:           StorUtil.cpp

        Contains:       Utilities for working with ODStorageUnits.

        Owned by:       Vincent Lo

        Copyright:      © 1994 - 1995 by Apple Computer, Inc., all rights reserved.


        In Progress:

*/

#include <mlogimpl.h>

#ifndef  _STORUTIL_
#include <StorUtil.h>
#endif

#ifndef  SOM_ODDraft_xh
#include <Draft.xh>
#endif

#ifndef  SOM_ODSession_xh
#include <ODSessn.xh>
#endif

#ifndef  SOM_ODStorageSystem_xh
#include <ODStor.xh>
#endif

#ifndef  SOM_ODDocument_xh
#include <Document.xh>
#endif

#ifndef  SOM_ODContainer_xh
#include <ODCtr.xh>
#endif

#ifndef  SOM_ODStorageUnit_xh
#include <StorageU.xh>
#endif

#ifndef  SOM_ODStorageUnitView_xh
#include <SUView.xh>
#endif

#ifndef  SOM_Module_OpenDoc_StdDefs_defined
#include <StdDefs.xh>
#endif

#ifndef  SOM_Module_OpenDoc_StdProps_defined
#include <StdProps.xh>
#endif

#ifndef  SOM_Module_OpenDoc_StdTypes_defined
#include "StdTypes.xh"
#endif

#ifndef  SOM_ODTranslation_xh
#include <Translt.xh>
#endif

#ifndef  _BARRAY_
#include <BArray.h>
#endif

//#ifndef  _STDTYPIO_
//#include <StdTypIO.h>
//#endif

//#ifndef  _PLFMFILE_
//#include <PlfmFile.h>
//#endif

//#ifndef  _EXCEPT_
//#include <Except.h>
//#endif

//#ifndef  _ODMEMORY_
//#include <ODMemory.h>
//#endif

//#ifndef  __TEXTEDIT__
//#include <TextEdit.h>
//#endif

//#ifndef  _TEMPOBJ_
//#include <TempObj.h>
//#endif

void ODSUAddPropValue(Environment *ev,ODStorageUnit *su,ODPropertyName prop,ODValueType val)
{
   DebugLog("Adding property %s value %s", (char *) prop, (char *) val);
   su->AddProperty(ev, prop)->AddValue(ev, val);
}

void ODSUForceFocus(Environment *ev,ODStorageUnit *su,ODPropertyName prop,ODValueType val)
{
   if (prop != kODNULL) {
      DebugLog("ForceFocus checking property %s", (char *) prop);
      if (su->Exists(ev, prop, kODNULL, 0) == kODFalse) {
         su->AddProperty(ev, prop);
         DebugLog("ForceFocus adding property %s", (char *) prop);
      }
      else {
         su->Focus(ev, prop, kODPosUndefined, kODNULL, 0, kODPosUndefined);
         DebugLog("ForceFocus focus on property %s", (char *) prop);
      }
   }
   if (val != kODNULL) {
      DebugLog("ForceFocus checking value %s", (char *) val);
      if (su->Exists(ev, prop, val, 0) == kODFalse) {
         su->AddValue(ev, val);
         DebugLog("ForceFocus adding value %s", (char *) val);
      }
      else {
         su->Focus(ev, prop, kODPosSame, val, 0, kODPosUndefined);
         DebugLog("ForceFocus focus on value %s", (char *) val);
      }
   }
   if (su->Exists(ev, prop, val, 0) == kODFalse) {
      DebugLog("Adding property and value failed");
   }
        // else; // Presumably the caller has already focussed the su somewhere,
                 // perhaps midstream. -TC
}

ODBoolean ODSUExistsThenFocus(Environment *ev,ODStorageUnit *su,ODPropertyName prop,ODValueType val)
{
   if (prop == kODNULL && val == kODNULL) return  kODTrue;
        // else; // Presumably the caller has already focussed the su somewhere,
                 // perhaps midstream. Property:kODNULL && ValueType:kODNULL always 'exist'. -TC

   if (su->Exists(ev, prop, val, 0) == kODTrue) {
      DebugLog("ExistsThenFocus Focusing on %s %s", (char *) prop, (char *) val);
      if (val) su->Focus(ev, prop, kODPosSame, val, 0, kODPosSame);
      else     su->Focus(ev, prop, kODPosSame, kODNULL, 0, kODPosAll);
      return  kODTrue;
   }
   else {
      DebugLog("Property %s value %s did not exist", (char *) prop, (char *) val);
      return  kODFalse;
   }
}

void ODSURemoveProperty(Environment *ev,ODStorageUnit *su,ODPropertyName prop)
{
   DebugLog("Removing property %s", (char *) prop);
   if (ODSUExistsThenFocus(ev, su, prop, kODNULL)) su->Remove(ev);
}

void FW_SUAddPropValue(Environment *ev,ODStorageUnit *su,ODPropertyName prop, ODValueType val)
{
   if (!su->Exists(ev, prop, NULL, 0))
   {
      su->AddProperty(ev, prop)->AddValue(ev, val);
   }
   else
   {
      su->Focus(ev, prop, kODPosUndefined, NULL, 0, kODPosUndefined);
      if (!su->Exists(ev, prop, val, 0))
         su->AddValue(ev, val);
   }
}

void FW_ReleaseODObject(Environment *ev,ODRefCntObject *odObject)
{
   if (odObject) odObject->Release(ev);
}

void FW_SUForceFocus(Environment *ev,ODStorageUnit *su,ODPropertyName prop, ODValueType val)
{
   ODSUForceFocus(ev, su, prop, val);
}

ODBoolean FW_SUExistsThenFocus(Environment *ev,ODStorageUnit *su,ODPropertyName prop,ODValueType val)
{
   return  ODSUExistsThenFocus(ev, su, prop, val);
}

void FW_SURemoveProperty(Environment *ev,ODStorageUnit *su,ODPropertyName prop)
{
   ODSURemoveProperty(ev, su, prop);
}
/*
ODULong StorageUnitGetValue(ODStorageUnit *su,Environment *ev,ODULong size,ODPtr buffer)
{
   ODByteArray    ba;
   ODULong        bytesRead = su->GetValue(ev, size, &ba);

   ODBlockMove(ba._buffer, buffer, bytesRead);
   ODDisposePtr(ba._buffer);

   return  bytesRead;
}

ODULong StorageUnitViewGetValue(ODStorageUnitView *suv,Environment *ev,ODULong size,ODPtr buffer)
{
   ODByteArray    ba;
   ODULong        bytesRead = suv->GetValue(ev, size, &ba);

   ODBlockMove(ba._buffer, buffer, ba._length);
   ODDisposePtr(ba._buffer);
   return  bytesRead;
}

void StorageUnitSetValue(ODStorageUnit *su,Environment *ev,ODULong size,const void *buffer)
{
   ODByteArray    ba;

   ba._length = size;
   ba._maximum = size;
   ba._buffer = (octet *)buffer;
   su->SetValue(ev, &ba);
}

void StorageUnitViewSetValue(ODStorageUnitView *suv,Environment *ev,ODULong size,const void *buffer)
{
   ODByteArray    ba;

   ba._length = size;
   ba._maximum = size;
   ba._buffer = (octet *)buffer;
   suv->SetValue(ev, &ba);
}


PlatformFile   *GetPlatformFileFromContainer(Environment *ev,ODContainer *container)
{
   ODByteArray    ba = container->GetID(ev);
   PlatformFile   *file = new PlatformFile();

   file->Specify((ODFileSpec *)ba._buffer);
   ODDisposePtr(ba._buffer);
   return  file;
}


ODFileSpec GetODFileSpecFromContainer(Environment *ev,ODContainer *container)
{
   ODByteArray    ba = container->GetID(ev);
   ODFileSpec     documentSpec = *((ODFileSpec *)ba._buffer);

   ODDisposePtr(ba._buffer); // DMc dispose when done

   return  documentSpec;
}

ODContainer    *CreateFileContainer(Environment *ev,ODSession *session,ODFileSpec *fsSpec)
{
   ODByteArray    *ba = CreateByteArray(fsSpec, sizeof(short)+sizeof(long)+fsSpec->name[0]+1);
   ODContainer    *newContainer = session->GetStorageSystem(ev)->CreateContainer(ev,
         kODDefaultFileContainer      // file->GetContainerType()
         , ba);

   DisposeByteArray(ba);
   return  newContainer;
}

//----------------------------------------------------------------------------------------
// GetFileContainer
//----------------------------------------------------------------------------------------

ODContainer    *GetFileContainer(Environment *ev,ODSession *session,ODFileSpec *fsSpec)
{
   ODByteArray    *ba = CreateByteArray(fsSpec, sizeof(short)+sizeof(long)+fsSpec->name[0]+1);
   ODContainer    *newContainer = session->GetStorageSystem(ev)->AcquireContainer(ev,
         kODDefaultFileContainer    // file->GetContainerType()
         , ba);

   DisposeByteArray(ba);
   return  newContainer;
}

//----------------------------------------------------------------------------------------
// CreateMemoryContainer
//----------------------------------------------------------------------------------------

ODContainer    *CreateMemoryContainer(Environment *ev,ODSession *session,ODHandle handle,
                                       ODContainerType containerType)
{
   ODByteArray    *ba;

   TRY            ODLockHandle(handle);

   ba = CreateByteArray(&handle, sizeof(ODHandle));
   ODUnlockHandle(handle);
   CATCH_ALL      ODUnlockHandle(handle);
   RERAISE;
   ENDTRY ODContainer *newContainer = session->GetStorageSystem(ev)->CreateContainer(ev,

         containerType, ba);

   DisposeByteArray(ba);
   return  newContainer;
}

//----------------------------------------------------------------------------------------
// GetMemoryContainer
//----------------------------------------------------------------------------------------

ODContainer    *GetMemoryContainer(Environment *ev,ODSession *session,ODHandle handle,
                                    ODContainerType containerType)
{
   ODByteArray    *ba;

   TRY            ODLockHandle(handle);

   ba = CreateByteArray(&handle, sizeof(ODHandle));
   ODUnlockHandle(handle);
   CATCH_ALL      ODUnlockHandle(handle);
   RERAISE;
   ENDTRY ODContainer *newContainer = session->GetStorageSystem(ev)->AcquireContainer(ev,

         containerType, ba);

   DisposeByteArray(ba);
   return  newContainer;
}

//----------------------------------------------------------------------------------------
// StorageUnitSetPromiseValue
//----------------------------------------------------------------------------------------

void StorageUnitSetPromiseValue(ODStorageUnit *su,Environment *ev,ODValueType valueType,ODULong
                                 offset,ODULong size,const void *buffer,ODPart *sourcePart)
{
   ODByteArray    ba;

   ba._length = size;
   ba._maximum = size;
   ba._buffer = (octet *)buffer;
   su->SetPromiseValue(ev, valueType, offset, &ba, sourcePart);
}

//----------------------------------------------------------------------------------------
// StorageUnitGetStylFromStyledText
//----------------------------------------------------------------------------------------

ODBoolean StorageUnitGetStylFromStyledText(ODStorageUnit *su,Environment *ev,ODULong *size,ODPtr
                                            *styl)
{
   ODBoolean      result = kODFalse;
   ODULong        stxtSize;
   ODULong        stylSize;
   ODUShort       scrpNStyles;
   ODType         applestxt = kODNULL;

   const ODPlatformType kODScrapType_stxt = 0x73747874; // 'stxt'

   ODVolatile(applestxt);
   ODVolatile(result);
   SOM_TRY ODTranslation *translation = su->GetSession(ev)->GetTranslation(ev);

   applestxt = translation->GetISOTypeFromPlatformType(ev, kODScrapType_stxt, kODPlatformDataType);
   if (ODSUExistsThenFocus(ev, su, kODPropContents, applestxt))
   {
      stxtSize = su->GetSize(ev);
      if (stxtSize >= sizeof(ODUShort))
      {
         StorageUnitGetValue(su, ev, sizeof(ODUShort), &scrpNStyles);
         stylSize = (scrpNStyles *sizeof(ScrpSTElement))+sizeof(ODUShort);
         if (stxtSize >= stylSize)
         {
            *styl = ODNewPtr(stylSize);
            *size = stylSize;
            su->SetOffset(ev, 0);
            StorageUnitGetValue(su, ev, stylSize, *styl);
            result = kODTrue;
         }
      }
   }
   delete         applestxt;

   SOM_CATCH_ALL if (applestxt)
      delete         applestxt;

   result = kODFalse;
   SOM_ENDTRYreturn result;

}

//------------------------------------------------------------------------------
// GetOriginalCloneKind
//------------------------------------------------------------------------------

ODCloneKind GetOriginalCloneKind(Environment *ev,ODDraft *draft)
{
        // If content was put on the clipboard without cloning, there will be no
        // kODPropOriginalCloneKind property in the draft's preferences storage
        // unit.  Assume the original operation was a copy in this case. Since
        // no links cannot be placed on the clipboard directly, there won't be
        // links needing fixup on a paste.
   TempODStorageUnit draftProperties = draft->AcquireDraftProperties(ev);

   ODCloneKind    cloneKind = ODGetULongProp(ev, draftProperties, kODPropOriginalCloneKind, kODULong
         );

   if ((ODULong)(cloneKind) != 0)
      return  cloneKind;
   else
      return  kODCloneCopy;
}

//------------------------------------------------------------------------------
// RemoveDataInterchangeProperties
//------------------------------------------------------------------------------

void RemoveDataInterchangeProperties(Environment *ev,ODStorageUnit *su,ODBoolean
                                      clonedToFileContainer)
{
   ODSURemoveProperty(ev, su, kODPropLinkSpec);
   ODSURemoveProperty(ev, su, kODPropMouseDownOffset);
   ODSURemoveProperty(ev, su, kODPropCloneKindUsed);
   if (!clonedToFileContainer)
   {
      ODSURemoveProperty(ev, su, kODPropProxyContents);
      ODSURemoveProperty(ev, su, kODPropContentFrame);
      ODSURemoveProperty(ev, su, kODPropSuggestedFrameShape);
   }
}
*/
