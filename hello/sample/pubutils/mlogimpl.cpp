#ifndef _MLOGIMPL_H_
#include "mlogimpl.h"
#endif

#include <string.h>
#include <stdarg.h>
#include <stdio.h>

MessageLog* MessageLog::_instance = 0;

MessageLog* MessageLog::Instance() {  // Ensure that there is only one instance of this class
   if (_instance == 0) {              // If the static variable _instance is 0, then create an instance
      _instance = new MessageLog;     // else return the instance that exists
   }
   return _instance;
}

MessageLog::~MessageLog()             // Since I am destructing, reset the the static variable to 0
{
   _instance = 0;
}

// This is the main logging function
void MessageLog::Log(long msgno, unsigned long rc, char * file, long line)
{
   printf("(%d) RC = %d, from %s line %d \n", msgno, rc, file, line);
}

void MessageLog::Log(long msgno, char * text)
{
   printf("(%d) >>> %s \n", msgno, text);
}

void MessageLog::Log(char msgno, char * text)
{
   printf("(%c) >>> %s \n", msgno, text);
}

void MessageLog::trace(char * file, long line)
{
   printf("Location >>> File %s line %d", file, line);
}

void MessageLog::debugLog(char *formatString,...)
{
   va_list args;
   va_start(args, formatString);

   vsprintf(ErrorBuf, formatString, args);
   va_end(args);

   Log((long) DEBUG_MSGNO, ErrorBuf);

   return;
}

void MessageLog::errorLog(char *formatString,...)
{
   va_list args;
   va_start(args, formatString);

   vsprintf(ErrorBuf, formatString, args);
   va_end(args);

   Log((long) ERROR_MSGNO, ErrorBuf);

   return;
}

void MessageLog::fatalLog(char *formatString,...)
{
   va_list args;
   va_start(args, formatString);

   vsprintf(ErrorBuf, formatString, args);
   va_end(args);

   Log((long) FATAL_MSGNO, ErrorBuf);

   return;
}

void MessageLog::dumpLog(char *formatString,...)
{
   va_list args;
   va_start(args, formatString);

   vsprintf(ErrorBuf, formatString, args);
   va_end(args);

   Log((char) 'D', ErrorBuf);

   return;
}

void MessageLog::dump(ODStorageUnit * su)
{
    Environment * ev = somGetGlobalEnvironment();
    ODULong numProp;
    ODULong numVal;
    ODULong i, j, size;
    ODPtr buff;
    ODValueType valType;

    su->Focus(ev, kODNULL, kODPosAll, kODNULL, 0, kODPosUndefined);
    numProp = su->CountProperties(ev);
    dumpLog("Begin Storage Unit Dump of ID %d Name >>>%s<<< of %d properties\n",
            (long) su->GetID(ev), (char *) su->GetName(ev), numProp);
    for (i = 1; i <= numProp; i++)
    {
       su->Focus(ev, kODNULL, kODPosNextSib, kODNULL, 0, kODPosUndefined);
       dumpLog("Property: Sequence(%d) is \n(...) === >>>%s<<<", i, (char *) su->GetProperty(ev));
       su->Focus(ev, kODNULL, kODPosSame, kODNULL, 0, kODPosAll);
       numVal = su->CountValues(ev);
       for (j = 1; j <= numVal; j++)
       {
          su->Focus(ev, kODNULL, kODPosSame, kODNULL, 0, kODPosNextSib);
          valType = su->GetType(ev);
          dumpLog("Type: Sequence(%d - %d)  Generation (%d) Length (%d) is \n(...) === >>>%s<<<",
                   i, j, (long) su->GetGenerationNumber(ev), su->GetSize(ev), (char *) valType);
          if (strcmp(valType, kODTime_T) == 0) {
             ODTime lTime;
             StorageUnitGetValue(su, ev, sizeof(ODTime), (ODPtr) &lTime);
             char * charTime = (char *) SOMMalloc(32);
             charTime = ctime((const time_t *)&lTime);
             dumpLog("Value: %s", charTime);
             SOMFree(charTime);
          }
          else if (strcmp(valType, kODPoint) == 0) {
             ODPoint point;
             StorageUnitGetValue(su, ev, sizeof(ODPoint), (ODPtr) &point);
             dumpLog("Value:   X Coord = %d    Y Coord = %d\n", (long) (point.x >> 16), (long) (point.y >> 16));
          }
          else if (strcmp(valType, kODOS2IText) == 0) {
             char * charIString = (char *) SOMMalloc(su->GetSize(ev) + 1);
             StorageUnitGetValue(su, ev, su->GetSize(ev), (ODPtr) charIString);
             dumpLog("Value:  %x  %x  %x\n", (long) *(charIString), (long) *(charIString + 4), (long) *(charIString + 8));
             SOMFree(charIString);
          }
          else if (strcmp(valType, kODStrongStorageUnitRef) == 0) {
             ODStorageUnitRefIterator * refIter = su->CreateStorageUnitRefIterator(ev);
             ODStorageUnitRef suRef = {0};
             refIter->First(ev, suRef);
             dumpLog("Storage Unit Ref %x is %d\n", (long) suRef, (long) su->IsValidStorageUnitRef(ev, suRef));
          }
          else if (strcmp(valType, kODStrongStorageUnitRefs) == 0) {
             ODStorageUnitRefIterator * refIter = su->CreateStorageUnitRefIterator(ev);
             ODStorageUnitRef suRef = {0};
             refIter->First(ev, suRef);
             dumpLog("Storage Unit Refs %x is %d\n", (long) suRef, (long) su->IsValidStorageUnitRef(ev, suRef));
          }
          else {
             size = su->GetSize(ev);
             buff = (ODPtr) SOMMalloc(size + 1);
             memset(buff, 0, size + 1);
             if (size > 0) StorageUnitGetValue(su, ev, size, buff);
             dumpLog("Value: >>>%s<<<\n", (char *) buff);
             SOMFree(buff);
          }
       }
    }
    dumpLog("End Storage Unit Dump of ID %d", su->GetID(ev));
}

void MessageLog::dump(ODDraft * draft)
{
    Environment * ev = somGetGlobalEnvironment();

    dumpLog("Start Dump of Draft");
    ODDraftPermissions dp = draft->GetPermissions(ev);
    if (dp == kODDPReadOnly)            dumpLog("  Draft is in read only status");
    else if (dp == kODDPExclusiveWrite) dumpLog("  Draft is in exclusive write status");
    else if (dp == kODDPSharedWrite)    dumpLog("  Draft is in shared write status");
    else if (dp == kODDPTransient)      dumpLog("  Draft is in navigation only status");
    else if (dp == kODDPNone)           dumpLog("  Draft is not accessable");

    dumpLog("  Draft ID is %d", (long) draft->GetID(ev));
}

void MessageLog::dump(ODDocument * document)
{
    Environment * ev = somGetGlobalEnvironment();

    dumpLog("Start Dump of Document");
    dumpLog("  Document ID is %d", (long) document->GetID(ev));
    dumpLog("  Document Name is >>>%s<<<", document->GetName(ev).text._buffer);
}

void MessageLog::dump(ODContainer * container)
{
    Environment * ev = somGetGlobalEnvironment();

    dumpLog("Start Dump of Container");
    if (container->GetID(ev)._length == 4)
       dumpLog("  Container ID is %x (Memory Container)", (long) container->GetID(ev)._buffer);
    else dumpLog("  Container ID is >>>%s<<< (File Container)", (char *) container->GetID(ev)._buffer);
    dumpLog("  Container Name is >>>%s<<<", container->GetName(ev).text._buffer);
}

void MessageLog::dump(ODSession * session)
{
    Environment * ev = somGetGlobalEnvironment();

    dumpLog("Start Dump of Session");
    dump(session, session->GetArbitrator(ev));
    dump(session->GetBinding(ev));
    dump(session->GetClipboard(ev));
    dumpLog("  Dispatcher       reference %x", (long) session->GetDispatcher(ev));
    dump(session->GetDragAndDrop(ev));
    dumpLog("  MessageInterface reference %x", (long) session->GetMessageInterface(ev));
    dumpLog("  NameResolver     reference %x", (long) session->GetNameResolver(ev));
    dumpLog("  NameSpaceManager reference %x", (long) session->GetNameSpaceManager(ev));
    dumpLog("  StorageSystem    reference %x", (long) session->GetStorageSystem(ev));
    dump(session->GetTranslation(ev));
//  dumpLog("  ToolSpaceManager reference %x", (long) session->GetToolSpaceManager(ev));
    dumpLog("  Undo             reference %x", (long) session->GetUndo(ev));
/*
    ODIText userName;
    session->GetUserName(ev, &userName);
    if (userName.text._length == 4)
       dumpLog("  UserName is %x", userName.text._buffer);
    else dumpLog("  UserName is >>>%s<<<", userName.text._buffer);
*/
    dump(session->GetWindowState(ev));
}

void MessageLog::dump(ODSession * session, ODArbitrator * arbitrator)
{
    Environment * ev = somGetGlobalEnvironment();

    ODTypeToken  token[8] =
    { session->Tokenize(ev, kODKeyFocus),
      session->Tokenize(ev, kODMenuFocus),
      session->Tokenize(ev, kODSelectionFocus),
      session->Tokenize(ev, kODModalFocus),
      session->Tokenize(ev, kODScrollingFocus),
      session->Tokenize(ev, kODClipboardFocus),
      session->Tokenize(ev, kODMouseFocus),
      session->Tokenize(ev, kODStatusLineFocus) };

    dumpLog("Start Dump of Arbitrator");

    for (int i = 0; i < 8; i++)
    {
       ODType type;
       if (session->GetType(ev, token[i], &type) == kODTrue)
       {
          if (arbitrator->IsFocusRegistered(ev, token[i]) == kODTrue)
          {
             ODFrame * ownerFrame = arbitrator->AcquireFocusOwner(ev, token[i]);
             dumpLog("  Focus >>>%s<<< is registered by module %x and owned by frame %x",
                     (char *) type, (long) arbitrator->GetFocusModule(ev, token[i]), (long) ownerFrame);
             if (ownerFrame) ownerFrame->Release(ev);
          }
          else
          {
             dumpLog("  Focus >>>%s<<< is not registered", (char *) type);
          }
       } else dumpLog("  Focus type is not in table");
    }
}
void MessageLog::dump(ODBinding * binding)
{
    Environment * ev = somGetGlobalEnvironment();

    dumpLog("Start Dump of Binding");
    dumpLog("  Bento File Container Suite is >>>%s<<<", (char *) binding->GetContainerSuite(ev, kODBentoFileContainer));
    dumpLog("  Bento Memory Container Suite is >>>%s<<<", (char *) binding->GetContainerSuite(ev, kODBentoMemoryContainer));
    dumpLog("  Default File Container Suite is >>>%s<<<", (char *) binding->GetContainerSuite(ev, kODDefaultFileContainer));
    dumpLog("  Default Memory Container Suite is >>>%s<<<", (char *) binding->GetContainerSuite(ev, kODDefaultMemoryContainer));

}
void MessageLog::dump(ODClipboard * clipboard)
{
    Environment * ev = somGetGlobalEnvironment();

    ODUpdateID clpbrdID= clipboard->GetUpdateID(ev);

    dumpLog("Start Dump of Clipboard at Generation %d", (long) clpbrdID);

    if (clpbrdID == 0)
       dumpLog("No clipboard data");
    else
       dump(clipboard->GetContentStorageUnit(ev));
}
void MessageLog::dump(ODDispatcher * dispatcher)
{
}
void MessageLog::dump(ODDragAndDrop * draganddrop)
{
    Environment * ev = somGetGlobalEnvironment();

    dumpLog("Start Dump of DragAndDrop");

    dump(draganddrop->GetContentStorageUnit(ev));
}
void MessageLog::dump(ODFrame * frame)
{
    Environment * ev = somGetGlobalEnvironment();

    dumpLog("Start Dump of Frame");

    WindowProperties winProps = {0};
    if (BeginGetWindowProperties(ev, frame, &winProps) == kODTrue) {
       dumpLog("  WindowProperty title           is >>>%s<<<", winProps.title);
       dumpLog("  WindowProperty createFlags     is >>>%x<<<", winProps.createFlags);
       dumpLog("  WindowProperty swpFlags        is >>>%x<<<", winProps.swpFlags);
       dumpLog("  WindowProperty wasVisible      is >>>%d<<<", winProps.wasVisible);
       dumpLog("  WindowProperty isResizable     is >>>%d<<<", winProps.isResizable);
       dumpLog("  WindowProperty isFloating      is >>>%d<<<", winProps.isFloating);
       dumpLog("  WindowProperty isRootWindow    is >>>%d<<<", winProps.isRootWindow);
       dumpLog("  WindowProperty shouldShowLinks is >>>%d<<<", winProps.shouldShowLinks);
       EndGetWindowProperties(ev, &winProps);
    }
    else dumpLog("  There are no Window Properties");
}
void MessageLog::dump(ODMessageInterface * messageinterface)
{
}
void MessageLog::dump(ODNameResolver * nameresolver)
{
}
void MessageLog::dump(ODNameSpaceManager * namespacemanager)
{
}
void MessageLog::dump(ODStorageSystem * storagesystem)
{
}
void MessageLog::dump(ODTranslation * translation)
{
   Environment * ev = somGetGlobalEnvironment();

   dumpLog("Start Dump of Translation");

   ODValueType valueType[35] =
   {  kODBoolean,
      kODCloneKind,
      kODDate,
      kODDragOperation,
      kODEditor,
      kODFileType,
      kODFileTypeEA,
      kODFontNameSize,
      kODIconFamily,
      kODIconFamilyWin,
      kODIconFamilyOS2,
      kODIconFamilyAIX,
      kODIntlText,
      kODISOStr,
      kODISOStrList,
      kODLinkSpec,
      kODObjectType,
      kODPoint,
      kODOS2IText,
      kODPolygon,
      kODRect,
      kODRGB2,
      kODSelectedKind,
      kODSelectedRMF,
      kODSLong,
      kODSShort,
      kODStrongStorageUnitRef,
      kODStrongStorageUnitRefs,
      kODTime_T,
      kODTransform,
      kODULong,
      kODUShort,
      kODWeakStorageUnitRef,
      kODWeakStorageUnitRefs };

    for (int i = 0; i < 34; i++)
    {
//     ODTypeList * typeList = translation->GetTranslationOf(ev, valueType[i]);

       dumpLog("  Translation for %s", (char *) valueType[i]); //typeList->Count(ev));
       dumpLog("    There are no entries");
    }
}
void MessageLog::dump(ODUndo * undo)
{
}
void MessageLog::dump(ODWindowState * windowstate)
{
    Environment * ev = somGetGlobalEnvironment();

    dumpLog("Start Dump of Window State");
    dumpLog("  Total Root Window Count is %d", (long) windowstate->GetTotalRootWindowCount(ev));
    dumpLog("  Window Count is %d", (long) windowstate->GetWindowCount(ev));
}

void MessageLog::dump(POINTL * pointl)
{
    dumpLog("Start Dump of POINTL    X = %d  Y = %d", pointl->x, pointl->y);
}

void MessageLog::dump(ODEventData * eventdata)
{
    if (eventdata->msg == WM_NULL)                       DebugLog("Msg Rcv WM_NULL   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_CREATE)                     DebugLog("Msg Rcv WM_CREATE   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_DESTROY)                    DebugLog("Msg Rcv WM_DESTROY   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_ENABLE)                     DebugLog("Msg Rcv WM_ENABLE   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_SHOW)                       DebugLog("Msg Rcv WM_SHOW   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_MOVE)                       DebugLog("Msg Rcv WM_MOVE   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_SIZE)                       DebugLog("Msg Rcv WM_SIZE   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_ADJUSTWINDOWPOS)            DebugLog("Msg Rcv WM_ADJUSTWINDOWPOS   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_CALCVALIDRECTS)             DebugLog("Msg Rcv WM_CALCVALIDRECTS   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_QUERYWINDOWPARAMS)          DebugLog("Msg Rcv WM_QUERYWINDOWPARAMS   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_HITTEST)                    DebugLog("Msg Rcv WM_HITTEST   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_ACTIVATE)                   DebugLog("Msg Rcv WM_ACTIVATE   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_SETFOCUS)                   DebugLog("Msg Rcv WM_SETFOCUS   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_SETSELECTION)               DebugLog("Msg Rcv WM_SETSELECTION   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_PPAINT)                     DebugLog("Msg Rcv WM_PPAINT   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_PSYSCOLORCHANGE)            DebugLog("Msg Rcv WM_PSYSCOLORCHANGE   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_PSIZE)                      DebugLog("Msg Rcv WM_PSIZE   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_PACTIVATE)                  DebugLog("Msg Rcv WM_PACTIVATE   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_PCONTROL)                   DebugLog("Msg Rcv WM_PCONTROL   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_COMMAND)                    DebugLog("Msg Rcv WM_COMMAND   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_SYSCOMMAND)                 DebugLog("Msg Rcv WM_SYSCOMMAND   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_HELP)                       DebugLog("Msg Rcv WM_HELP   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_PAINT)                      DebugLog("Msg Rcv WM_PAINT   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_TIMER)                      DebugLog("Msg Rcv WM_TIMER   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_SEM1)                       DebugLog("Msg Rcv WM_SEM1   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_SEM2)                       DebugLog("Msg Rcv WM_SEM2   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_SEM3)                       DebugLog("Msg Rcv WM_SEM3   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_SEM4)                       DebugLog("Msg Rcv WM_SEM4   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_CLOSE)                      DebugLog("Msg Rcv WM_CLOSE   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_QUIT)                       DebugLog("Msg Rcv WM_QUIT   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_SYSCOLORCHANGE)             DebugLog("Msg Rcv WM_SYSCOLORCHANGE   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_SYSVALUECHANGED)            DebugLog("Msg Rcv WM_SYSVALUECHANGED   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_APPTERMINATENOTIFY)         DebugLog("Msg Rcv WM_APPTERMINATENOTIFY   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_PRESPARAMCHANGED)           DebugLog("Msg Rcv WM_PRESPARAMCHANGED   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_CONTROL)                    DebugLog("Msg Rcv WM_CONTROL   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_VSCROLL)                    DebugLog("Msg Rcv WM_VSCROLL   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_HSCROLL)                    DebugLog("Msg Rcv WM_HSCROLL   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_INITMENU)                   DebugLog("Msg Rcv WM_INITMENU   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_MENUSELECT)                 DebugLog("Msg Rcv WM_MENUSELECT   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_MENUEND)                    DebugLog("Msg Rcv WM_MENUEND   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_DRAWITEM)                   DebugLog("Msg Rcv WM_DRAWITEM   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_MEASUREITEM)                DebugLog("Msg Rcv WM_MEASUREITEM   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_CONTROLPOINTER)             DebugLog("Msg Rcv WM_CONTROLPOINTER   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_QUERYDLGCODE)               DebugLog("Msg Rcv WM_QUERYDLGCODE   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_INITDLG)                    DebugLog("Msg Rcv WM_INITDLG   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_SUBSTITUTESTRING)           DebugLog("Msg Rcv WM_SUBSTITUTESTRING   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_MATCHMNEMONIC)              DebugLog("Msg Rcv WM_MATCHMNEMONIC   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_SAVEAPPLICATION)            DebugLog("Msg Rcv WM_SAVEAPPLICATION   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_HELPBASE)                   DebugLog("Msg Rcv WM_HELPBASE   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_HELPTOP)                    DebugLog("Msg Rcv WM_HELPTOP   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_USER)                       DebugLog("Msg Rcv WM_USER   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_VRNDISABLED)                DebugLog("Msg Rcv WM_VRNDISABLED   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_VRNENABLED)                 DebugLog("Msg Rcv WM_VRNENABLED   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_MOUSEFIRST)                 DebugLog("Msg Rcv WM_MOUSEFIRST   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_MOUSELAST)                  DebugLog("Msg Rcv WM_MOUSELAST   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_BUTTONCLICKFIRST)           DebugLog("Msg Rcv WM_BUTTONCLICKFIRST   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_BUTTONCLICKLAST)            DebugLog("Msg Rcv WM_BUTTONCLICKLAST   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_MOUSEMOVE)                  DebugLog("Msg Rcv WM_MOUSEMOVE   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_BUTTON1DOWN)                DebugLog("Msg Rcv WM_BUTTON1DOWN   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_BUTTON1UP)                  DebugLog("Msg Rcv WM_BUTTON1UP   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_BUTTON1DBLCLK)              DebugLog("Msg Rcv WM_BUTTON1DBLCLK   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_BUTTON2DOWN)                DebugLog("Msg Rcv WM_BUTTON2DOWN   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_BUTTON2UP)                  DebugLog("Msg Rcv WM_BUTTON2UP   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_BUTTON2DBLCLK)              DebugLog("Msg Rcv WM_BUTTON2DBLCLK   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_BUTTON3DOWN)                DebugLog("Msg Rcv WM_BUTTON3DOWN   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_BUTTON3UP)                  DebugLog("Msg Rcv WM_BUTTON3UP   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_BUTTON3DBLCLK)              DebugLog("Msg Rcv WM_BUTTON3DBLCLK   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_MOUSEMAP)                   DebugLog("Msg Rcv WM_MOUSEMAP   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_EXTMOUSEFIRST)              DebugLog("Msg Rcv WM_EXTMOUSEFIRST   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_EXTMOUSELAST)               DebugLog("Msg Rcv WM_EXTMOUSELAST   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_CHORD)                      DebugLog("Msg Rcv WM_CHORD   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_BUTTON1MOTIONSTART)         DebugLog("Msg Rcv WM_BUTTON1MOTIONSTART   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_BUTTON1MOTIONEND)           DebugLog("Msg Rcv WM_BUTTON1MOTIONEND   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_BUTTON1CLICK)               DebugLog("Msg Rcv WM_BUTTON1CLICK   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_BUTTON2MOTIONSTART)         DebugLog("Msg Rcv WM_BUTTON2MOTIONSTART   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_BUTTON2MOTIONEND)           DebugLog("Msg Rcv WM_BUTTON2MOTIONEND   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_BUTTON2CLICK)               DebugLog("Msg Rcv WM_BUTTON2CLICK   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_BUTTON3MOTIONSTART)         DebugLog("Msg Rcv WM_BUTTON3MOTIONSTART   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_BUTTON3MOTIONEND)           DebugLog("Msg Rcv WM_BUTTON3MOTIONEND   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_BUTTON3CLICK)               DebugLog("Msg Rcv WM_BUTTON3CLICK   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_MOUSETRANSLATEFIRST)        DebugLog("Msg Rcv WM_MOUSETRANSLATEFIRST   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_MOUSETRANSLATELAST)         DebugLog("Msg Rcv WM_MOUSETRANSLATELAST   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_BEGINDRAG)                  DebugLog("Msg Rcv WM_BEGINDRAG   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_ENDDRAG)                    DebugLog("Msg Rcv WM_ENDDRAG   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_SINGLESELECT)               DebugLog("Msg Rcv WM_SINGLESELECT   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_OPEN)                       DebugLog("Msg Rcv WM_OPEN   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_CONTEXTMENU)                DebugLog("Msg Rcv WM_CONTEXTMENU   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_CONTEXTHELP)                DebugLog("Msg Rcv WM_CONTEXTHELP   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_TEXTEDIT)                   DebugLog("Msg Rcv WM_TEXTEDIT   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_BEGINSELECT)                DebugLog("Msg Rcv WM_BEGINSELECT   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_PICKUP)                     DebugLog("Msg Rcv WM_PICKUP   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_PENFIRST)                   DebugLog("Msg Rcv WM_PENFIRST   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_PENLAST)                    DebugLog("Msg Rcv WM_PENLAST   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_MMPMFIRST)                  DebugLog("Msg Rcv WM_MMPMFIRST   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_MMPMLAST)                   DebugLog("Msg Rcv WM_MMPMLAST   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_BIDI_FIRST)                 DebugLog("Msg Rcv WM_BIDI_FIRST   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_BIDI_LAST)                  DebugLog("Msg Rcv WM_BIDI_LAST   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_CHAR)                       DebugLog("Msg Rcv WM_CHAR   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_VIOCHAR)                    DebugLog("Msg Rcv WM_VIOCHAR   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_JOURNALNOTIFY)              DebugLog("Msg Rcv WM_JOURNALNOTIFY   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_MSGBOXINIT)                 DebugLog("Msg Rcv WM_MSGBOXINIT   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_MSGBOXDISMISS)              DebugLog("Msg Rcv WM_MSGBOXDISMISS   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_FLASHWINDOW)                DebugLog("Msg Rcv WM_FLASHWINDOW   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_FORMATFRAME)                DebugLog("Msg Rcv WM_FORMATFRAME   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_UPDATEFRAME)                DebugLog("Msg Rcv WM_UPDATEFRAME   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_FOCUSCHANGE)                DebugLog("Msg Rcv WM_FOCUSCHANGE   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_SETBORDERSIZE)              DebugLog("Msg Rcv WM_SETBORDERSIZE   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_TRACKFRAME)                 DebugLog("Msg Rcv WM_TRACKFRAME   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_MINMAXFRAME)                DebugLog("Msg Rcv WM_MINMAXFRAME   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_SETICON)                    DebugLog("Msg Rcv WM_SETICON   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_QUERYICON)                  DebugLog("Msg Rcv WM_QUERYICON   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_SETACCELTABLE)              DebugLog("Msg Rcv WM_SETACCELTABLE   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_QUERYACCELTABLE)            DebugLog("Msg Rcv WM_QUERYACCELTABLE   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_TRANSLATEACCEL)             DebugLog("Msg Rcv WM_TRANSLATEACCEL   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_QUERYTRACKINFO)             DebugLog("Msg Rcv WM_QUERYTRACKINFO   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_QUERYBORDERSIZE)            DebugLog("Msg Rcv WM_QUERYBORDERSIZE   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_NEXTMENU)                   DebugLog("Msg Rcv WM_NEXTMENU   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_ERASEBACKGROUND)            DebugLog("Msg Rcv WM_ERASEBACKGROUND   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_QUERYFRAMEINFO)             DebugLog("Msg Rcv WM_QUERYFRAMEINFO   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_QUERYFOCUSCHAIN)            DebugLog("Msg Rcv WM_QUERYFOCUSCHAIN   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_OWNERPOSCHANGE)             DebugLog("Msg Rcv WM_OWNERPOSCHANGE   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_CALCFRAMERECT)              DebugLog("Msg Rcv WM_CALCFRAMERECT   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_WINDOWPOSCHANGED)           DebugLog("Msg Rcv WM_WINDOWPOSCHANGED   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_ADJUSTFRAMEPOS)             DebugLog("Msg Rcv WM_ADJUSTFRAMEPOS   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_QUERYFRAMECTLCOUNT)         DebugLog("Msg Rcv WM_QUERYFRAMECTLCOUNT   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_QUERYHELPINFO)              DebugLog("Msg Rcv WM_QUERYHELPINFO   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_SETHELPINFO)                DebugLog("Msg Rcv WM_SETHELPINFO   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_ERROR)                      DebugLog("Msg Rcv WM_ERROR   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_REALIZEPALETTE)             DebugLog("Msg Rcv WM_REALIZEPALETTE   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_RENDERFMT)                  DebugLog("Msg Rcv WM_RENDERFMT   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_RENDERALLFMTS)              DebugLog("Msg Rcv WM_RENDERALLFMTS   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_DESTROYCLIPBOARD)           DebugLog("Msg Rcv WM_DESTROYCLIPBOARD   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_PAINTCLIPBOARD)             DebugLog("Msg Rcv WM_PAINTCLIPBOARD   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_SIZECLIPBOARD)              DebugLog("Msg Rcv WM_SIZECLIPBOARD   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_HSCROLLCLIPBOARD)           DebugLog("Msg Rcv WM_HSCROLLCLIPBOARD   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_VSCROLLCLIPBOARD)           DebugLog("Msg Rcv WM_VSCROLLCLIPBOARD   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_DRAWCLIPBOARD)              DebugLog("Msg Rcv WM_DRAWCLIPBOARD   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_DDE_FIRST)                  DebugLog("Msg Rcv WM_DDE_FIRST   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_DDE_INITIATE)               DebugLog("Msg Rcv WM_DDE_INITIATE   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_DDE_REQUEST)                DebugLog("Msg Rcv WM_DDE_REQUEST   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_DDE_ACK)                    DebugLog("Msg Rcv WM_DDE_ACK   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_DDE_ADVISE)                 DebugLog("Msg Rcv WM_DDE_ADVISE   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_DDE_UNADVISE)               DebugLog("Msg Rcv WM_DDE_UNADVISE   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_DDE_POKE)                   DebugLog("Msg Rcv WM_DDE_POKE   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_DDE_EXECUTE)                DebugLog("Msg Rcv WM_DDE_EXECUTE   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_DDE_TERMINATE)              DebugLog("Msg Rcv WM_DDE_TERMINATE   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_DDE_INITIATEACK)            DebugLog("Msg Rcv WM_DDE_INITIATEACK   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_DDE_LAST)                   DebugLog("Msg Rcv WM_DDE_LAST   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
    if (eventdata->msg == WM_QUERYCONVERTPOS)            DebugLog("Msg Rcv WM_QUERYCONVERTPOS   HWND %x  MP1 %x  MP2 %x", eventdata->hwnd, eventdata->mp1, eventdata->mp2);
}
