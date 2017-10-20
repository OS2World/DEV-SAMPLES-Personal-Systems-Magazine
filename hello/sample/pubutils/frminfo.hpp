#ifndef _FRAMEINFO_
#define _FRAMEINFO_

#ifndef SOM_ODPart_xh
#include <part.xh>
#endif

#define INCL_OD
#include <od.h>

#ifndef _ISTRING_
#include <istring.hpp>
#endif

#ifdef DEBUG
#include <mlogimpl.h>
#endif

// implicitly all methods are inline since their definition is here.
class FrameInfoRec
{
   public:
      FrameInfoRec(Environment * ev, ODFrame * frame, ODStorageUnitView * suv = 0)
      : partEv(ev), thisFrame(frame)
      {
         // dangerous to call member functions if you plan to derive/use v.fn
         if (suv != 0)
            InitFromStorage(suv);

         thisFrame->SetPartInfo(ev, (ODInfoType) this);
      }

      virtual ~FrameInfoRec()
      {
         thisFrame->SetPartInfo(partEv, kODNULL);
      }
      // make sure derived class calls these first!
      virtual void InitFromStorage(ODStorageUnitView * suv)
      {
         if (suv->GetSize(partEv)) // if not 0 then data is in StorageUnitView
            StorageUnitViewGetValue(suv, partEv, sizeof(FrameInfoData), &FrameInfoData);
      }
      virtual void Externalize(ODStorageUnitView * suv)
      {
      }

   private:
      void SetNeedToExternalize()
      {
         thisFrame->SetPartInfo(partEv, (ODInfoType) this);      // make frame dirty
      }

      Environment * partEv;
      ODFrame * thisFrame;
      struct FRAMEINFODATA
      {
      }  FrameInfoData;
};
#endif
