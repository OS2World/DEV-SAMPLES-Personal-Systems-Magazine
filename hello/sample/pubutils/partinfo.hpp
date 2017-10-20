#ifndef _PARTINFO_
#define _PARTINFO_

#ifndef SOM_ODPart_xh
#include <part.xh>
#endif

#define INCL_OD
#include <od.h>

#ifndef _ISTRING_
#include <istring.hpp>
#endif

#include <mlogimpl.h>


// implicitly all methods are inline since their definition is here.
class PartInfoRec
{
   public:
      PartInfoRec(Environment * ev, ODPart * part, ODType kValueType = "PartInfoData") 
      : partEv(ev), thisPart(part), kValueType(kValueType), needToExternalize(kODFalse)
      {
        // null body, see initialization list
      }
      virtual ~PartInfoRec() { }

      // make sure derived class calls these first!
      virtual void InitFromStorage()
      {
         ReadPropPreferredKind();
         ReadPropPreferredEditor();
         ReadPropContents();
      }

      virtual void Externalize()
      {
      if (needToExternalize == kODTrue){
         WritePropPreferredKind();
         WritePropPreferredEditor();
         WritePropContents(); };
      }

//      void SetPropPreferredKind(const char * kind) { preferredKind = kind; SetDirty();}
//      void SetPropPreferredEditor(const char * editor) { preferredEditor = editor; SetDirty();}
      void SetPropContents(ODByteArray * data) 
      {
      DebugLog("in SetPropContents");
        if (propContents)
            DisposeByteArray(propContents);
        propContents = CopyByteArray(data);
        SetDirty();
      }

//      char * GetPropPreferredKind() { return preferredKind; }
//      char * GetPropPreferredEditor() { return preferredEditor; }
      ODByteArray * GetPropContents() { return propContents; }

      void WritePropPreferredKind()
      {
//         ODStorageUnit* su = thisPart->GetStorageUnit(partEv);
//         if(!su->Exists(partEv, kODPropPreferredKind, preferredKind, 0))
//            if (preferredKind.length())
//                su->AddProperty(partEv, kODPropPreferredKind)->AddValue(partEv, preferredKind);
      }
      void WritePropPreferredEditor()
      {
//         ODStorageUnit* su = thisPart->GetStorageUnit(partEv);
//         if(!su->Exists(partEv, kODPropPreferredEditor, preferredEditor, 0))
//            if (preferredEditor.length())
//                su->AddProperty(partEv, kODPropPreferredEditor)->AddValue(partEv, preferredEditor);
      }
      void WritePropContents()
      {
      DebugLog("in WritePropContents");
         ODStorageUnit* su = thisPart->GetStorageUnit(partEv);
       if(!su->Exists(partEv, kODPropContents, 0, 0))
{      DebugLog("exists");
          su->AddProperty(partEv, kODPropContents);}
       if(!su->Exists(partEv, kODPropContents, kValueType, 0))
          su->AddValue(partEv, kValueType)->SetValue(partEv, propContents);
       else {
          su->Focus(partEv, kODPropContents, 0, kValueType, 0, 0)->SetValue(partEv, propContents); }
      }
      void ReadPropPreferredKind()
      {
//         ODStorageUnit* su = thisPart->GetStorageUnit(partEv);
//         if(su->Exists(partEv, kODPropPreferredKind, preferredKind, 0))
//         {
//            if(!su->Focus(partEv, kODPropPreferredKind, 0, kODNULL, 0, 0))
//                preferredKind = su->GetType(partEv);
//         }
      }
      void ReadPropPreferredEditor()
      {
//         ODStorageUnit* su = thisPart->GetStorageUnit(partEv);
//         if(su->Exists(partEv, kODPropPreferredKind, preferredKind, 0))
//         {
//            if(!su->Focus(partEv, kODPropPreferredEditor, 0, kODNULL, 0, 0))
//                preferredEditor = su->GetType(partEv);
//         }
      }
      void ReadPropContents()
      {
         ODStorageUnit* su = thisPart->GetStorageUnit(partEv);
         if(!su->Focus(partEv, kODPropContents, 0, kValueType, 0, 0)) 
         {
            CreateByteArray(propContents, su->GetSize(partEv));
            su->GetValue(partEv, su->GetSize(partEv), propContents);
         }
      }

   private:
      void SetDirty()
      {
         needToExternalize = kODTrue;
      }
      ODType          kValueType;
      Environment   * partEv;
      ODPart        * thisPart;
      ODBoolean       needToExternalize;
//      IString         preferredKind;
//      IString         preferredEditor;
      ODByteArray   * propContents;

      struct PARTINFODATA
      {
      }  PartInfoData;
};
#endif
